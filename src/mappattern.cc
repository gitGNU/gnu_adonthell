/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "mappattern.h"

mappattern::~mappattern()
{
  u_int16 i;
  for (i = 0; i < nbr_of_frames; i++)
    frame[i].~image();
  free(frame);
  frame=NULL;
  nbr_of_frames=0;
}

void mappattern::update ()
{
    if (nbr_of_frames > 1)
    {
        framecounter++;
        if (framecounter == counterlimit)
        {
            currentframe++;
            if (currentframe == nbr_of_frames)
                currentframe = 0;
            framecounter = 0;
        }
    }
}

s_int8 mappattern::get (FILE * file)
{
    u_int16 i;
    fread (&nbr_of_frames, sizeof (nbr_of_frames), 1, file);
    fread (&counterlimit, sizeof (counterlimit), 1, file);
    framecounter = 0;
    currentframe = 0;
    frame = new image[nbr_of_frames];
    
    for (i = 0; i < nbr_of_frames; i++)
        if ((frame[i].get(file)))
	  return (-1);
    return (0);
}

s_int8 mappattern::load (char name[])
{
    u_int16 retvalue;
    FILE *file;

    file = fopen (name, "r");
    if (!file)
        return (1);
    else
    {
        retvalue = get(file);
        fclose (file);
        return (retvalue);
    }
}

void mappattern::putbox (u_int16 x, u_int16 y)
{
  frame[currentframe].putbox(x,y);
}

void mappattern::putbox_mask (u_int16 x, u_int16 y)
{
  frame[currentframe].putbox_mask(x,y);
}

void mappattern::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
		  u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_part(x,y,bw,bh,xo,yo);
}

void mappattern::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  frame[currentframe].putbox_trans(x,y,alpha);
}

void mappattern::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, 
				   u_int16 bh,u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_mask_part(x,y,bw,bh,xo,yo);
}

void mappattern::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  frame[currentframe].putbox_mask_trans(x,y,alpha);
}

void mappattern::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
			u_int8 alpha)
{
  frame[currentframe].putbox_part_trans(x,y,bw,bh,xo,yo,alpha);
}

void mappattern::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
					 u_int16 bh,u_int16 xo, u_int16 yo, 
					 u_int8 alpha)
{
  frame[currentframe].putbox_mask_part_trans(x,y,bw,bh,xo,yo,alpha);
}
