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
#include "image.h"
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

s_int8 mappattern::get (SDL_RWops * file)
{
  u_int16 i;
  s_int8 res;
  SDL_RWread (file,&nbr_of_frames, sizeof (nbr_of_frames), 1);
  SDL_RWread (file,&counterlimit, sizeof (counterlimit), 1);
  framecounter = 0;
  currentframe = 0;
  frame = new image[nbr_of_frames];
  
  for (i = 0; i < nbr_of_frames; i++)
    {
      if ((res=frame[i].get_pnm(file))) return (res);
      frame[i].set_mask(true);
      frame[i].set_alpha(255);
    }
  return (0);
}

s_int8 mappattern::load (char name[])
{
    u_int16 retvalue;
    SDL_RWops * file;

    file = SDL_RWFromFile (name, "r");
    if (!file)
        return (1);
    else
    {
        retvalue = get(file);
        SDL_RWclose (file);
        return (retvalue);
    }
}

/*s_int8 mappattern::put (SDL_RWops * file)
{
    u_int16 i;
    SDL_RWwrite (file,&nbr_of_frames, sizeof (nbr_of_frames), 1);
    SDL_RWwrite (file,&counterlimit, sizeof (counterlimit), 1);
    for (i = 0; i < nbr_of_frames; i++)
        if ((frame[i].simpleput(file)))
	  return (-1);
    return (0);
}*/

void mappattern::putbox (u_int16 x, u_int16 y, drawing_area * da_opt)
{
  frame[currentframe].putbox(x,y,da_opt);
}

void mappattern::putbox_mask (u_int16 x, u_int16 y, drawing_area * da_opt)
{
  frame[currentframe].putbox_mask(x,y,da_opt);
}
/*
void mappattern::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
		  u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_part(x,y,bw,bh,xo,yo);
}
*/
void mappattern::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha, 
			       drawing_area * da_opt)
{
  frame[currentframe].putbox_trans(x,y,alpha,da_opt);
}
/*
void mappattern::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, 
				   u_int16 bh,u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_mask_part(x,y,bw,bh,xo,yo);
}
*/
void mappattern::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha, 
				    drawing_area * da_opt)
{
  frame[currentframe].putbox_mask_trans(x,y,alpha,da_opt);
}
/*
void mappattern::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
			u_int8 alpha)
{
  frame[currentframe].putbox_part_trans(x,y,bw,bh,xo,yo,alpha);
}
*/
/*
void mappattern::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
					 u_int16 bh,u_int16 xo, u_int16 yo, 
					 u_int8 alpha)
{
  frame[currentframe].putbox_mask_part_trans(x,y,bw,bh,xo,yo,alpha);
}
*/
