// $Id

/*
    Copyright (C) 2000 Alexandre Courbot.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

    This file is a part of the Adonthell project.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "animation.h"

animation_frame::animation_frame() : image()
{
  gapx=0;
  gapy=0;
}

void animation::init_frame(u_int16 nbr)
{
  frame[nbr].mask_on=false;
  frame[nbr].trans_on=false;
  frame[nbr].gapx=0;
  frame[nbr].gapy=0;
}

animation::animation()
{
  frame=NULL;
  nbr_of_frames=0;
  currentframe=0;
  speedcounter=0;
  alpha_on=false;
  mask_on=false;
  alpha=0;
  loop=false;
  reverse=false;
  factor=1;
}

animation::~animation()
{
  free(frame);
}

void animation::update()
{
  if(frame[currentframe].delay==0) return;
  if (nbr_of_frames <= 1) return;
    
  speedcounter++;
  if (speedcounter == frame[currentframe].delay)
    next_frame();
}

void animation::set_active_frame(u_int16 framenbr)
{
  if(framenbr<nbr_of_frames) currentframe=framenbr;
}

void animation::next_frame()
{
  currentframe+=factor;
  if (currentframe == nbr_of_frames)
    {
      if(loop)
	currentframe = 0;
      else if(reverse)
	{ factor=-1; currentframe-=2; }
      else currentframe-=factor;
    }
  else if((currentframe==0)&&(reverse)) factor=1;
  speedcounter = 0;
}

void animation::draw(u_int16 x, u_int16 y)
{
  if(alpha_on && mask_on)
    frame[currentframe].putbox_mask_trans(x,y,alpha);
  else if(alpha_on)
    frame[currentframe].putbox_trans(x,y,alpha);
  else if(mask_on)
    frame[currentframe].putbox_mask(x,y);
  else frame[currentframe].putbox(x,y);
}

s_int8 animation::load_frame(char * fname)
{
  frame=(animation_frame*)realloc(frame,
				  ++nbr_of_frames*sizeof(animation_frame));
  init_frame(nbr_of_frames-1);
  if(frame[nbr_of_frames-1].load(fname))
    {
      frame=(animation_frame*)realloc(frame,
				      --nbr_of_frames*sizeof(animation_frame));
      return(-1);
    }
  return(0);
}

s_int8 animation::get_frame(FILE * file)
{
  frame=(animation_frame*)realloc(frame,
				  ++nbr_of_frames*sizeof(animation_frame));
  if(frame[nbr_of_frames-1].get(file))
    {
      frame=(animation_frame*)realloc(frame,
				      --nbr_of_frames*sizeof(animation_frame));
      return(-1);
    }
  return(0);
}

void animation::set_delay(u_int16 framenbr, u_int16 delay)
{
  frame[framenbr].delay=delay;
}
