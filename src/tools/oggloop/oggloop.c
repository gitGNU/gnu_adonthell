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
#include <unistd.h>

#include "vorbis/vorbisfile.h"
#include "vorbis/codec.h"
#include "vcedit.h"

/* Write the changed OggVorbis comments */
void write_file (char *file, vcedit_state *state)
{
    char tmpfile[512];
    FILE *outfile;

    /* Move the original to a temposrary file */
    sprintf (tmpfile, "%s.tmp", file);
    rename (file, tmpfile);
    
    outfile = fopen (file, "wb");
    if (!outfile)
    {
        printf ("\nError: Couldn't open '%s' for writing\n", file);
        exit (1);
    }

    /* write the changed file */
    if (vcedit_write (state, outfile) == -1)
    {
        printf ("\nError: %s\n", vcedit_error (state));

        /* cleanup */
        fclose (outfile);
        unlink (file);
        rename (tmpfile, file);
        vcedit_clear (state);
        free (state);
        exit (1);
    }

    /* Everything went well -> delete the temporary file */
    unlink (tmpfile);

    fclose (outfile);
}

/* Remove any existing Adonthell related comments */
void remove_comments (char *file)
{
    vcedit_state *state;
    vorbis_comment *vc;
    char changed = 0;
    FILE *oggfile;
    int i;

    /* open the tune for vcedit */
    oggfile = fopen (file, "rb");
    if (!oggfile)
    {
        printf ("\nError: Couldn't open '%s' for reading\n", file);
        exit (1);
    }

    /* add stuff to the oggvorbis comment header */
    state = vcedit_new_state ();
    if (vcedit_open (state, oggfile) == -1)
    {
        printf ("\nError: %s\n", vcedit_error (state));
        exit (1);
    }

    vc = vcedit_comments (state);
    
    /* Remove Adonthell related comments, else we might get duplicates */
    for (i = 0; i < vc->comments; i++)
        if (strncmp (vc->user_comments[i], "OldHeaderSize", 13) == 0 ||
            strncmp (vc->user_comments[i], "StartPagePCM", 12) == 0 ||
            strncmp (vc->user_comments[i], "StartPageRaw", 12) == 0 ||
            strncmp (vc->user_comments[i], "Start", 5) == 0 ||
            strncmp (vc->user_comments[i], "End", 3) == 0)
            {
                int j;

                free (vc->user_comments[i]);

                for (j = i; j < vc->comments - 1; j++)
                {
                    vc->user_comments[j] = vc->user_comments[j + 1];
                    vc->comment_lengths[j] = vc->comment_lengths[j + 1];
                }

                changed = 1;
                vc->comments--;
                i--;
            }

    if (changed) write_file (file, state);

    /* Cleanup */
    fclose (oggfile);
    vcedit_clear (state);
    free (state);
}

/* Add the newly calculated looping info as comments to the Vorbis File */
void add_comments (char *file, int p_pcm, int p_raw, int s, int e)
{
    vcedit_state *state;
    FILE *oggfile;
    char tmp[32];
    int size;

    /* open the tune for vcedit */
    oggfile = fopen (file, "rb");
    if (!oggfile)
    {
        printf ("\nError: Couldn't open '%s' for reading\n", file);
        exit (1);
    }

    /* get the file size before we add our comments */
    fseek (oggfile, 0, SEEK_END);
    size = ftell (oggfile);
    fseek (oggfile, 0, SEEK_SET);

    /* add stuff to the oggvorbis comment header */
    state = vcedit_new_state ();
    if (vcedit_open (state, oggfile) == -1)
    {
        printf ("\nError: %s\n", vcedit_error (state));
        exit (1);
    }

    /* add the values */
    sprintf (tmp, "%i", size);
    vorbis_comment_add_tag (state->vc, "OldHeaderSize", tmp);
    sprintf (tmp, "%i", p_pcm);
    vorbis_comment_add_tag (state->vc, "StartPagePCM", tmp);
    sprintf (tmp, "%i", p_raw);
    vorbis_comment_add_tag (state->vc, "StartPageRaw", tmp);
    sprintf (tmp, "%i", s);
    vorbis_comment_add_tag (state->vc, "Start", tmp);
    sprintf (tmp, "%i", e);
    vorbis_comment_add_tag (state->vc, "End", tmp);

    /* Save the new comments */
    write_file (file, state);

    /* Cleanup */
    fclose (oggfile);
    vcedit_clear (state);
    free (state);
}

/* Calculate everything we need to find the start and end of our loop */
void calculate_loop (int argc, char* argv[])
{
    OggVorbis_File ov;
    FILE *oggfile;
    int i, s, e, p_raw, p_pcm;
    double start = 0;
    double end = 0;

    /* open the file, our last argument */
    oggfile = fopen (argv[argc - 1], "r");

    if (!oggfile)
    {
        printf ("File \"%s\" not found.\n\n", argv[argc - 1]);
        exit (1);
    }

    if (ov_open (oggfile, &ov, NULL, -1) < 0)
    {
        printf ("Could not open input as an OggVorbis file.\n\n");
        exit (1);
    }

    /* try to get start and end time */
    for (i = 1; i < argc - 1; i++)
    {
        if (!strcmp ("-s", argv[i]) && i + 1 < argc)
            start = atof (argv[++i]);
        if (!strcmp ("-e", argv[i]) && i + 1 < argc)
            end = atof (argv[++i]);
    }

    /* print details about each logical bitstream in the input */
    if (ov_seekable (&ov))
    {
        if (!end)
            end = ov_time_total (&ov, -1);

        /* We want to loop a tune in ogg format the following way:

           start           end
           |------- + ------------ + -------|
           intro        loop        outro

           Get the starting point as pcm offset, since that makes the seeking
           most precise */

        ov_time_seek (&ov, start);
        s = ov_pcm_tell (&ov);
        printf ("\nStart (%gs):\t %i (pcm offset)\n", start, s);

        /* To allow fast seeking, we'll get the start of the page containing
           the pcm sample above, we then position the stream manually to the
           page and skip data until we've found our pcm offset. That's nearly
           as fast as ov_seek_raw () would be! */
        ov_time_seek_page (&ov, start);
        p_pcm = ov_pcm_tell (&ov);
        p_raw = ov_raw_tell (&ov) - ov.oy.returned;

        /* Get the end point as raw bytes */
        ov_time_seek (&ov, end);
        e = ov_raw_tell (&ov);
        printf ("End (%gs):\t %i (raw bytes)\n", end, e);
        fflush (stdout);

        /* now add the comments to the vorbis file */
        add_comments (argv[argc-1], p_pcm, p_raw, s, e);
    }
    else
    {
        printf ("Input was not seekable.\n");
    }

    /* close the file */
    ov_clear (&ov);
}

/* oggloop */
int main (int argc, char *argv[])
{
    if (argc <= 1 || argc > 6)
    {
        printf ("\nUsage: %s [-s <start>] [-e <end>] <OggVorbis file>\n\n", argv[0]);
        exit (1);
    } 

    /* First remove any existing Adonthell related comments */
    remove_comments (argv[argc-1]);

    /* Then calculate the values we need and add them to the OggVorbis file */
    calculate_loop (argc, argv);

    return 0;
}
