// $Id$

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
#include "image.h"
#include "animation.h"

animation_frame::animation_frame() : image()
{
  gapx=0;
  gapy=0;
}

s_int8 animation_frame::get(SDL_RWops * file)
{
  SDL_RWread(file,&gapx,sizeof(gapx),1);
  SDL_RWread(file,&gapy,sizeof(gapy),1);
  SDL_RWread(file,&delay,sizeof(delay),1);
  image::get(file);
  return(0);
}

s_int8 animation_frame::load(char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(-1);
  retvalue=get(file);
  SDL_RWclose(file);
  return(retvalue);
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

s_int8 animation::get_frame(SDL_RWops * file)
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

s_int8 animation::get(SDL_RWops * file)
{
  u_int16 i;
  SDL_RWread(file,&nbr_of_frames,sizeof(nbr_of_frames),1);
  SDL_RWread(file,&speedcounter,sizeof(speedcounter),1);
  SDL_RWread(file,&factor,sizeof(factor),1);
  for(i=0;i<nbr_of_frames;i++)
    frame[i].get(file);
  return(0);
}

s_int8 animation::load(char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(-1);
  retvalue=get(file);
  SDL_RWclose(file);
  return(retvalue);
}

void animation::set_delay(u_int16 framenbr, u_int16 delay)
{
  frame[framenbr].delay=delay;
}
