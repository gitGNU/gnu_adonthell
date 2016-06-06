/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Code partly taken from libvorbis (vorbisfile.c)
   THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001
   by the XIPHOPHORUS Company http://www.xiph.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#if defined (OGG_MUSIC)

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL_mixer.h>

#include "types.h"
#include "fileops.h"
#include "audio.h"
#include "audio_loop.h"
#include "vorbis/codec.h"

// structure to store our looping point information
loop_info::loop_info (OggVorbis_File *vf)
{
    // read stuff from the vorbis comment
    vorbis_comment *vc = ov_comment (vf, -1);
    s_int32 offset = 0;
    char *tmp;

    // since adding those information to the file header changes the
    // raw offsets, we have to restore them from the original and the 
    // current file size.
    tmp = vorbis_comment_query (vc, "OldHeaderSize", 0);
    if (tmp) 
    {
        int new_size = ov_raw_total (vf, -1);
        int old_size = atoi (tmp);

        offset = old_size - new_size;
    }

    // The PCM position of the page with the loop's start
    tmp = vorbis_comment_query (vc, "StartPagePCM", 0);
    if (tmp) start_page_pcm = atoi (tmp);
    else start_page_pcm = -1;

    // The raw position of the page containing the loop's start
    // in the file stream
    tmp = vorbis_comment_query (vc, "StartPageRaw", 0);
    if (tmp) start_page_raw = atoi (tmp) + offset;
    else start_page_raw = 0;

    // The actual PCM position of the loop's start
    tmp = vorbis_comment_query (vc, "Start", 0);
    if (tmp) start = atoi (tmp);
    else start = 0;

    // The raw position of the loop's end point
    tmp = vorbis_comment_query (vc, "End", 0);
    if (tmp) end = atoi (tmp) + offset;
    else end = ov_raw_total (vf, -1);
}

#define CHUNKSIZE 4096

extern "C" {
static long _get_data(OggVorbis_File *vf){
  errno=0;
  if(vf->datasource){
    char *buffer=ogg_sync_buffer(&vf->oy,CHUNKSIZE);
    long bytes=(vf->callbacks.read_func)(buffer,1,CHUNKSIZE,vf->datasource);
    if(bytes>0)ogg_sync_wrote(&vf->oy,bytes);
    if(bytes==0 && errno)return(-1);
    return(bytes);
  }else
    return(0);
}

static long _get_next_page(OggVorbis_File *vf,ogg_page *og,int boundary){
  if(boundary>0)boundary+=vf->offset;
  while(1){
    long more;

    if(boundary>0 && vf->offset>=boundary)return(OV_FALSE);
    more=ogg_sync_pageseek(&vf->oy,og);
    
    if(more<0){
      /* skipped n bytes */
      vf->offset-=more;
    }else{
      if(more==0){
	/* send more paramedics */
	if(!boundary)return(OV_FALSE);
	{
	  long ret=_get_data(vf);
	  if(ret==0)return(OV_EOF);
	  if(ret<0)return(OV_EREAD);
	}
      }else{
	/* got a page.  Return the offset at the page beginning,
           advance the internal offset past the page end */
	long ret=vf->offset;
	vf->offset+=more;
	return(ret);
	
      }
    }
  }
}

static void _make_decode_ready(OggVorbis_File *vf){
  if(vf->ready_state)return;
  if(vf->seekable){
    vorbis_synthesis_init(&vf->vd,vf->vi+vf->current_link);
  }else{
    vorbis_synthesis_init(&vf->vd,vf->vi);
  }    
  vorbis_block_init(&vf->vd,&vf->vb);
  vf->ready_state=1;
  return;
}

static void _decode_clear(OggVorbis_File *vf){
  ogg_stream_clear(&vf->os);
  vorbis_dsp_clear(&vf->vd);
  vorbis_block_clear(&vf->vb);
  vf->ready_state=0;

  vf->bittrack=0.f;
  vf->samptrack=0.f;
}

static int _process_packet(OggVorbis_File *vf,int readp){
  ogg_page og;

  /* handle one packet.  Try to fetch it from current stream state */
  /* extract packets from page */
  while(1){
    
    /* process a packet if we can.  If the machine isn't loaded,
       neither is a page */
    if(vf->ready_state){
      ogg_packet op;
      int result=ogg_stream_packetout(&vf->os,&op);
      ogg_int64_t granulepos;
      
      if(result==-1)return(OV_HOLE); /* hole in the data. */
      if(result>0){
	/* got a packet.  process it */
	granulepos=op.granulepos;
	if(!vorbis_synthesis(&vf->vb,&op)){ /* lazy check for lazy
                                               header handling.  The
                                               header packets aren't
                                               audio, so if/when we
                                               submit them,
                                               vorbis_synthesis will
                                               reject them */

	  /* suck in the synthesis data and track bitrate */
	  {
	    int oldsamples=vorbis_synthesis_pcmout(&vf->vd,NULL);
	    vorbis_synthesis_blockin(&vf->vd,&vf->vb);
	    vf->samptrack+=vorbis_synthesis_pcmout(&vf->vd,NULL)-oldsamples;
	    vf->bittrack+=op.bytes*8;
	  }

	  /* update the pcm offset. */
	  if(granulepos!=-1 && !op.e_o_s){
	    int link=(vf->seekable?vf->current_link:0);
	    int i,samples;
	    
	    // printf ("Updating the pcm offset\n");
	    /* this packet has a pcm_offset on it (the last packet
	       completed on a page carries the offset) After processing
	       (above), we know the pcm position of the *last* sample
	       ready to be returned. Find the offset of the *first*

	       As an aside, this trick is inaccurate if we begin
	       reading anew right at the last page; the end-of-stream
	       granulepos declares the last frame in the stream, and the
	       last packet of the last page may be a partial frame.
	       So, we need a previous granulepos from an in-sequence page
	       to have a reference point.  Thus the !op.e_o_s clause
	       above */
	    
	    samples=vorbis_synthesis_pcmout(&vf->vd,NULL);
	    
	    granulepos-=samples;
	    for(i=0;i<link;i++)
	      granulepos+=vf->pcmlengths[i];
	    vf->pcm_offset=granulepos;
	  }
	  return(1);
	}
      }
    }

    if(!readp)return(0);
    if(_get_next_page(vf,&og,-1)<0)return(OV_EOF); /* eof. leave unitialized */

    /* bitrate tracking; add the header's bytes here, the body bytes
       are done by packet above */
    vf->bittrack+=og.header_len*8;

    /* has our decoding just traversed a bitstream boundary? */
    if(vf->ready_state){
      if(vf->current_serialno!=ogg_page_serialno(&og)){
	_decode_clear(vf);
      }
    }

    /* Do we need to load a new machine before submitting the page? */
    /* This is different in the seekable and non-seekable cases.  

       In the seekable case, we already have all the header
       information loaded and cached; we just initialize the machine
       with it and continue on our merry way.

       In the non-seekable (streaming) case, we'll only be at a
       boundary if we just left the previous logical bitstream and
       we're now nominally at the header of the next bitstream
    */

    if(!vf->ready_state){
      int link;
      if(vf->seekable){
	vf->current_serialno=ogg_page_serialno(&og);
	
	/* match the serialno to bitstream section.  We use this rather than
	   offset positions to avoid problems near logical bitstream
	   boundaries */
	for(link=0;link<vf->links;link++)
	  if(vf->serialnos[link]==vf->current_serialno)break;
	if(link==vf->links)return(OV_EBADLINK); /* sign of a bogus
						   stream.  error out,
						   leave machine
						   uninitialized */
	
	vf->current_link=link;
	
	ogg_stream_init(&vf->os,vf->current_serialno);
	ogg_stream_reset(&vf->os); 
	
      }/* else{
	// we're streaming
	// fetch the three header packets, build the info struct
	
	_fetch_headers(vf,vf->vi,vf->vc,&vf->current_serialno,&og);
	vf->current_link++;
	link=0;
      } */ // No streaming in Adonthell
      
      _make_decode_ready(vf);
    }
    ogg_stream_pagein(&vf->os,&og);
  }
}

static int _seek_in_page (OggVorbis_File *vf, int pos)
{
    /* discard samples until we reach the desired position. Crossing a
       logical bitstream boundary with abandon is OK. */
    while (vf->pcm_offset < pos)
    {
        float **pcm;
        long target=pos-vf->pcm_offset;
        long samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);

        if (samples > target) samples = target;
        vorbis_synthesis_read(&vf->vd,samples);
        vf->pcm_offset+=samples;
    
        //printf ("Current offset %i", vf->pcm_offset);
        //printf (", needed %i, retrieved %i\n", target, samples);
        
        if(samples<target)
            if(_process_packet(vf,1)==0)
	           vf->pcm_offset=ov_pcm_total(vf,-1); /* eof */
    }

    return 0;
}

// Callback passed to OggVorbis to read/and loop our background music
size_t ogg_read_callback (void *ptr, size_t size, size_t nmemb, void *datasource)
{
    // get the current position
    size_t cur_pos = ftell ((FILE*) datasource);
    u_int32 end_pos = audio::get_loop_end ();

    // check whether we'll reach the looping point with the next read
    if (cur_pos + nmemb > end_pos)
    {
        // In case we do:
        int read = 0;
        int to_read = end_pos - cur_pos;
        OggVorbis_File *vf = audio::get_vorbisfile ();
        nmemb -= to_read;

        // read up to the looping point, ...
        read = fread (ptr, size, to_read, (FILE*) datasource);
        // jump back in the stream ...
        vf->pcm_offset = audio::get_start_page_pcm ();
        fseek ((FILE*) datasource, audio::get_start_page_raw (), SEEK_SET);
        _seek_in_page (vf, audio::get_loop_start ());
        // and read the remaining data from there
        read += fread (((char*) ptr)+read, size, nmemb, (FILE*) datasource);

        return read;
    }
    
    // otherwise just read the next chunk of data
    return fread (ptr, size, nmemb, (FILE*) datasource);
}

// Wrapper around fread for to prevent some problems with glibc2.2 based systems
size_t fread_wrap (void *ptr, size_t size, size_t nmemb, void *datasource)
{
    return fread (ptr, size, nmemb, (FILE*) datasource);
}
}
#endif // OGG_MUSIC

