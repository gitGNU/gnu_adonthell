/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.


   Based on the seeking-example from libvorbis-1.0beta4:
   
 ********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2001             *
 * by the XIPHOPHORUS Company http://www.xiph.org/                  *

 ********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vorbis/vorbisfile.h"

int main (int argc, char *argv[]) {
  OggVorbis_File ov;
  char *infoname;
  FILE *oggfile, *infofile;
  double start = 0;
  double end = 0;
  int i, s, e;

  /* open the file, our last argument */
  oggfile = fopen (argv[argc-1], "r");

  if (!oggfile) {
    printf ("File \"%s\" not found.\n\n", argv[argc-1]);
    exit (1);
  }
  
  if (ov_open (oggfile, &ov, NULL, -1) < 0) {
    printf ("Could not open input as an OggVorbis file.\n\n");
    exit (1);
  }

  /* try to get start and end time */
  for (i = 1; i < argc-1; i++) {
    if (!strcmp ("-s", argv[i]) && i+1 < argc)
      start = atof (argv[++i]);
    if (!strcmp ("-e", argv[i]) && i+1 < argc)
      end = atof (argv[++i]);
  }
  
  /* print details about each logical bitstream in the input */
  if (ov_seekable (&ov)) {
    if (!end) end = ov_time_total (&ov, -1);

    /* We want to loop a tune in ogg format the following way:

              start           end
       |------- + ------------ + -------|
         intro        loop        outro

       Get the starting point as raw bytes, since that makes the seeking
       most efficient */
    ov_time_seek (&ov, start);
    s =  ov_raw_tell (&ov);
    printf ("\nStart (%gs):\t %i (raw bytes)\n", start, s);

    /* Get the end point as raw bytes */
    ov_time_seek (&ov, end);
    e = ov_raw_tell (&ov);
    printf ("End (%gs):\t %i (raw bytes)\n", end, e);
    fflush (stdout);      
    
   /* write stuff to file */
   infoname = strdup (argv[argc-1]);
   memcpy (infoname+strlen(infoname)-4, ".lpp", 4);

   infofile = fopen (infoname, "w");
   if (infofile) {
     fwrite (&s, sizeof(s), 1, infofile);
     fwrite (&e, sizeof(e), 1, infofile);
     fclose (infofile);
     printf ("\nOK. Output written to %s.\n\n", infoname);

   } else {
     printf ("\nError: couldn't write to %s.\n\n", infoname);
   }
   
   free (infoname);
   
  } else {
    printf ("Input was not seekable.\n");
  }

  ov_clear (&ov);
  return 0;
}
