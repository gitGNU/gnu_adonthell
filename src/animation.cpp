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
#include "animation.h"

#ifdef DEBUG
u_int16 animation::a_d_diff=0;
#endif

void animation_frame::init()
{
  gapx=0;
  gapy=0;
  next_nostd=false;
  nextframe=0;
  image::init();
}

animation_frame::animation_frame()
{
  init();
}

animation_frame::~animation_frame()
{
}

s_int8 animation_frame::get(SDL_RWops * file)
{
  SDL_RWread(file,&gapx,sizeof(gapx),1);
  SDL_RWread(file,&gapy,sizeof(gapy),1);
  SDL_RWread(file,&delay,sizeof(delay),1);
  SDL_RWread(file,&next_nostd,sizeof(next_nostd),1);
  SDL_RWread(file,&nextframe,sizeof(nextframe),1);
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

#ifdef _EDIT
s_int8 animation_frame::put(SDL_RWops * file)
{
  SDL_RWwrite(file,&gapx,sizeof(gapx),1);
  SDL_RWwrite(file,&gapy,sizeof(gapy),1);
  SDL_RWwrite(file,&delay,sizeof(delay),1);
  SDL_RWwrite(file,&next_nostd,sizeof(next_nostd),1);
  SDL_RWwrite(file,&nextframe,sizeof(nextframe),1);
  image::put(file);
  return(0);
}

s_int8 animation_frame::save(char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(-1);
  retvalue=put(file);
  SDL_RWclose(file);
  return(retvalue);
}
#endif

animation::animation()
{
#ifdef DEBUG
  cout << "animation() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
  frame=NULL;
  nbr_of_frames=0;
  currentframe=0;
  speedcounter=0;
  loop=false;
  reverse=false;
  factor=1;
}

animation::~animation()
{
  delete[] frame;
#ifdef DEBUG
  cout << "~animation() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
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
  if(frame[currentframe].next_nostd)
    currentframe=frame[currentframe].nextframe;
  else
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
    }
  speedcounter = 0;
}

void animation::draw(u_int16 x, u_int16 y)
{
  frame[currentframe].draw(x,y);
}

s_int8 animation::load_frame(char * fname)
{
  animation_frame * oldframe=frame;
  int i;
  frame=new animation_frame[++nbr_of_frames];
  for (i=0;i<nbr_of_frames-1;i++)
    {
      frame[i]=oldframe[i];
    }

  if(frame[nbr_of_frames-1].image::load_raw(fname))
    {
      --nbr_of_frames;
      delete[] frame;
      frame=oldframe;
      return(-1);
    }
  delete[] oldframe;
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
  SDL_RWread(file,&loop,sizeof(loop),1);
  SDL_RWread(file,&reverse,sizeof(reverse),1);
  frame=new animation_frame[nbr_of_frames];
  for(i=0;i<nbr_of_frames;i++)
      frame[i].get(file);
  currentframe=0;
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

#ifdef _EDIT
s_int8 animation::put(SDL_RWops * file)
{
  u_int16 i;
  SDL_RWwrite(file,&nbr_of_frames,sizeof(nbr_of_frames),1);
  SDL_RWwrite(file,&speedcounter,sizeof(speedcounter),1);
  SDL_RWwrite(file,&factor,sizeof(factor),1);
  SDL_RWwrite(file,&loop,sizeof(loop),1);
  SDL_RWwrite(file,&reverse,sizeof(reverse),1);
  for(i=0;i<nbr_of_frames;i++)
    frame[i].put(file);
  return(0);
}

s_int8 animation::save(char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(-1);
  retvalue=put(file);
  SDL_RWclose(file);
  return(retvalue);
}
#endif

void animation::set_delay(u_int16 framenbr, u_int16 delay)
{
  frame[framenbr].delay=delay;
}

void animation::set_next_frame(u_int16 framenbr, u_int16 next_frame)
{
  frame[framenbr].next_nostd=true;
  frame[framenbr].nextframe=next_frame;
}

void animation::set_mask(u_int16 framenbr, bool m)
{
  frame[framenbr].set_mask(true);
}
