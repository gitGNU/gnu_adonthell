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
  delay=0;
  nextframe=0;
}

animation_frame::animation_frame()
{
  init();
}

animation_frame::~animation_frame()
{
}

u_int8 animation_frame::get_alpha()
{
  return alpha;
}

void animation_frame::set_alpha(u_int8 a)
{
  alpha=a;
}

bool animation_frame::get_mask()
{
  return is_masked;
}

void animation_frame::set_mask(bool mask)
{
  is_masked=mask;
}

u_int16 animation_frame::get_image()
{
  return imagenbr;
}

void animation_frame::set_image(u_int16 imnbr)
{
  imagenbr=imnbr;
}

u_int16 animation_frame::get_delay()
{
  return delay;
}

void animation_frame::set_delay(u_int16 d)
{
  delay=d;
}

u_int16 animation_frame::get_nextframe()
{
  return nextframe;
}

void animation_frame::set_nextframe(u_int16 nf, bool last_frame=false)
{
  lastframe=last_frame;
  nextframe=nf;
}

s_int8 animation_frame::get(SDL_RWops * file)
{
  SDL_RWread(file,&imagenbr,sizeof(imagenbr),1);
  SDL_RWread(file,&is_masked,sizeof(is_masked),1);
  SDL_RWread(file,&alpha,sizeof(alpha),1);
  SDL_RWread(file,&gapx,sizeof(gapx),1);
  SDL_RWread(file,&gapy,sizeof(gapy),1);
  SDL_RWread(file,&delay,sizeof(delay),1);
  SDL_RWread(file,&lastframe,sizeof(lastframe),1);
  SDL_RWread(file,&nextframe,sizeof(nextframe),1);
  return(0);
}

s_int8 animation_frame::load(const char * fname)
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
  SDL_RWwrite(file,&imagenbr,sizeof(imagenbr),1);
  SDL_RWwrite(file,&is_masked,sizeof(is_masked),1);
  SDL_RWwrite(file,&alpha,sizeof(alpha),1);
  SDL_RWwrite(file,&gapx,sizeof(gapx),1);
  SDL_RWwrite(file,&gapy,sizeof(gapy),1);
  SDL_RWwrite(file,&delay,sizeof(delay),1);
  SDL_RWwrite(file,&lastframe,sizeof(lastframe),1);
  SDL_RWwrite(file,&nextframe,sizeof(nextframe),1);
  return(0);
}

s_int8 animation_frame::save(const char * fname)
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
  t_frame=NULL;
  frame=NULL;
  nbr_of_images=0;
  nbr_of_frames=0;
  currentframe=0;
  speedcounter=0;
  //  loop=false;
  //  reverse=false;
  //  factor=1;
  play_flag=false;
}

animation::~animation()
{
  delete[] t_frame;
  delete[] frame;
#ifdef DEBUG
  cout << "~animation() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

void animation::update()
{
  if(!play_flag) return;
  if(frame[currentframe].delay==0) return;
  if(nbr_of_frames<=1) return;
    
  speedcounter++;
  if (speedcounter>=frame[currentframe].delay)
    next_frame();
}

void animation::set_active_frame(u_int16 framenbr)
{
  if(framenbr<nbr_of_frames) currentframe=framenbr;
}

void animation::next_frame()
{
  if(frame[currentframe].lastframe) return;
  currentframe=frame[currentframe].nextframe;
  speedcounter=0;
}

void animation::play()
{
  play_flag=true;
}

void animation::stop()
{
  play_flag=false;
}

void animation::rewind()
{
  currentframe=0;
  speedcounter=0;
}

void animation::draw(u_int16 x, u_int16 y)
{
  t_frame[frame[currentframe].imagenbr].set_mask(frame[currentframe].is_masked);
  t_frame[frame[currentframe].imagenbr].set_alpha(frame[currentframe].alpha);
  
  t_frame[frame[currentframe].imagenbr].draw(x,y);
}

/*
s_int8 animation::load_frame(const char * fname, u_int16 pos)
{
  animation_frame * oldframe=frame;
  u_int16 i;
  if(pos>nbr_of_frames) return -2;
  frame=new animation_frame[++nbr_of_frames];
  for (i=0;i<pos;i++)
    {
      frame[i]=oldframe[i];
    }

  if(frame[pos].image::load_raw(fname))
    {
      --nbr_of_frames;
      delete[] frame;
      frame=oldframe;
      return(-1);
    }
  for (i=pos+1;i<nbr_of_frames;i++)
    {
      frame[i]=oldframe[i-1];
      if(frame[i].next_nostd) frame[i].nextframe++;
    }

  delete[] oldframe;
  return 0;
}

s_int8 animation::get_frame(SDL_RWops * file, u_int16 pos)
{
  animation_frame * oldframe=frame;
  u_int16 i;
  frame=new animation_frame[++nbr_of_frames];
  for (i=0;i<nbr_of_frames-1;i++)
    {
      frame[i]=oldframe[i];
    }

  if(frame[nbr_of_frames-1].image::get_raw(file))
    {
      --nbr_of_frames;
      delete[] frame;
      frame=oldframe;
      return(-1);
    }
  delete[] oldframe;
  return 0;
}

s_int8 animation::delete_frame(u_int16 pos)
{
  animation_frame * oldframe=frame;
  u_int16 i;
  if(pos>nbr_of_frames-1) return -2;
  frame=new animation_frame[--nbr_of_frames];
  for(i=0;i<pos;i++)
    frame[i]=oldframe[i];
  for(i=pos;i<nbr_of_frames;i++)
    frame[i]=oldframe[i+1];
  if(currentframe>=nbr_of_frames) currentframe=nbr_of_frames-1;
  delete[] oldframe;
  return 0;
}
*/

s_int8 animation::get(SDL_RWops * file)
{
  u_int16 i;
  SDL_RWread(file,&nbr_of_images,sizeof(nbr_of_images),1);
  t_frame=new image[nbr_of_images];
  for(i=0;i<nbr_of_images;i++)
    t_frame[i].get_raw(file);
  SDL_RWread(file,&nbr_of_frames,sizeof(nbr_of_frames),1);
  frame=new animation_frame[nbr_of_frames];
  for(i=0;i<nbr_of_frames;i++)
      frame[i].get(file);
  currentframe=0;
  return(0);
}

s_int8 animation::load(const char * fname)
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
  SDL_RWwrite(file,&nbr_of_images,sizeof(nbr_of_images),1);
  for(i=0;i<nbr_of_images;i++)
    t_frame[i].put_raw(file);
  SDL_RWwrite(file,&nbr_of_frames,sizeof(nbr_of_frames),1);
  for(i=0;i<nbr_of_frames;i++)
      frame[i].put(file);
  currentframe=0;
  return(0);
}

s_int8 animation::save(const char * fname)
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
/*
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
*/

animation & animation::operator =(animation &a)
{
  u_int16 i;
  nbr_of_images=a.nbr_of_images;
  nbr_of_frames=a.nbr_of_frames;
  currentframe=a.currentframe;
  speedcounter=a.speedcounter;
  play_flag=a.play_flag;
  delete[] t_frame;
  t_frame=new image[nbr_of_images];
  for(i=0;i<nbr_of_images;i++)
    t_frame[i]=a.t_frame[i];
  delete[] frame;
  frame=new animation_frame[nbr_of_frames];
  for(i=0;i<nbr_of_frames;i++)
    frame[i]=a.frame[i];
  return *this;
}
