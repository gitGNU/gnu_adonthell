// $Id$

/*
  Copyright (C) 1999-2000   The Adonthell Project
  Part of the Adonthell Project http://adonthell.linuxgames.com
  
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

#ifdef _DEBUG_
u_int16 animation_frame::a_d_diff=0;
#endif

#ifdef _DEBUG_
u_int16 animation::a_d_diff=0;
#endif

void animation_frame::init()
{
  imagenbr=0;
  is_masked=false;
  set_alpha(255);
  gapx=0;
  gapy=0;
  delay=0;
  nextframe=0;
}

animation_frame::animation_frame()
{
  init();
#ifdef _DEBUG_
  cout << "animation_frame() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

animation_frame::~animation_frame()
{
#ifdef _DEBUG_
  cout << "~animation_frame() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

u_int8 animation_frame::get_alpha()
{
#ifdef REVERSE_ALPHA
  return alpha;
#else
  return 255-alpha;
#endif
}

void animation_frame::set_alpha(u_int8 a)
{
#ifdef REVERSE_ALPHA
  alpha=a;
#else
  alpha=255-a;
#endif
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

void animation_frame::set_nextframe(u_int16 nf)
{
  nextframe=nf;
}

s_int8 animation_frame::get(gzFile file)
{
  gzread(file,&imagenbr,sizeof(imagenbr));
  gzread(file,&is_masked,sizeof(is_masked));
  gzread(file,&alpha,sizeof(alpha));
  set_alpha(alpha);
  gzread(file,&gapx,sizeof(gapx));
  gzread(file,&gapy,sizeof(gapy));
  gzread(file,&delay,sizeof(delay));
  gzread(file,&nextframe,sizeof(nextframe));
  return(0);
}

s_int8 animation_frame::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return(-1);
  retvalue=get(file);
  gzclose(file);
  return(retvalue);
}

#ifdef _EDIT_
s_int8 animation_frame::put(gzFile file)
{
  gzwrite(file,&imagenbr,sizeof(imagenbr));
  gzwrite(file,&is_masked,sizeof(is_masked));
  set_alpha(alpha);
  gzwrite(file,&alpha,sizeof(alpha));
  set_alpha(alpha);
  gzwrite(file,&gapx,sizeof(gapx));
  gzwrite(file,&gapy,sizeof(gapy));
  gzwrite(file,&delay,sizeof(delay));
  gzwrite(file,&nextframe,sizeof(nextframe));
  return(0);
}

s_int8 animation_frame::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return(-1);
  retvalue=put(file);
  gzclose(file);
  return(retvalue);
}
#endif

void animation::init()
{
  t_frame=NULL;
  frame=NULL;
  nbr_of_images=0;
  nbr_of_frames=0;
  currentframe=0;
  speedcounter=0;
  play_flag=false;
  length=height=0;
#ifdef _EDIT_
  in_editor=false;
  currentimage=0;
#endif
}

animation::animation()
{
  init();
#ifdef _DEBUG_
  cout << "animation() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

void animation::clear()
{
  delete[] t_frame;
  delete[] frame;
}

animation::~animation()
{
  clear();
#ifdef _DEBUG_
  cout << "~animation() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

bool animation::is_empty()
{
  return((!nbr_of_frames)||(!nbr_of_images));
}

u_int16 animation::get_length()
{
  return length;
}

u_int16 animation::get_height()
{
  return height;
}

void animation::update()
{
  if((!play_flag)||(!nbr_of_frames)) return;
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
  currentframe=frame[currentframe].nextframe;
  speedcounter=0;
#ifdef _EDIT_
  if(in_editor)
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#endif
}

void animation::play()
{
  play_flag=true;
#ifdef _EDIT_
  if(in_editor) must_upt_label_status=true;
#endif
}

void animation::stop()
{
  play_flag=false;
#ifdef _EDIT_
  if(in_editor) must_upt_label_status=true;
#endif
}

void animation::rewind()
{
  currentframe=0;
  speedcounter=0;
}

void animation::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
#ifdef _EDIT_
  if(!nbr_of_frames) return;
#endif

  t_frame[frame[currentframe].imagenbr].set_mask(frame[currentframe].is_masked);
  t_frame[frame[currentframe].imagenbr].set_alpha(frame[currentframe].get_alpha());
  
  t_frame[frame[currentframe].imagenbr].draw(x+frame[currentframe].gapx,
					     y+frame[currentframe].gapy,
					     da_opt);
}

s_int8 animation::get(gzFile file)
{
  u_int16 i;
  gzread(file,&length,sizeof(length));
  gzread(file,&height,sizeof(height));
  gzread(file,&nbr_of_images,sizeof(nbr_of_images));
  delete[] t_frame;
  t_frame=new image[nbr_of_images];
  for(i=0;i<nbr_of_images;i++)
    t_frame[i].get_raw(file);
  gzread(file,&nbr_of_frames,sizeof(nbr_of_frames));
  delete[] frame;
  frame=new animation_frame[nbr_of_frames];
  for(i=0;i<nbr_of_frames;i++)
      frame[i].get(file);

  for(currentframe=0;currentframe<nbr_of_frames;currentframe++)
    {
      u_int16 tl,th;
      if((tl=t_frame[frame[currentframe].imagenbr].
	  get_length()+frame[currentframe].gapx)>length)
	length=tl;
      
      if((th=t_frame[frame[currentframe].imagenbr].
	  get_height()+frame[currentframe].gapy)>height)
	height=th;
    }
  currentframe=0;
  return(0);
}

s_int8 animation::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return(-1);
  retvalue=get(file);
  gzclose(file);
  return(retvalue);
}

void animation::zoom(u_int16 sx, u_int16 sy, animation * src)
{
  static u_int16 i;
  clear();
  init();
  nbr_of_images=src->nbr_of_images;
  t_frame=new image[nbr_of_images];
  for(i=0;i<nbr_of_images;i++)
    {
      t_frame[i].resize((src->t_frame[i].length*sx)/src->length,
			(src->t_frame[i].height*sy)/src->height);
      t_frame[i].zoom(&src->t_frame[i]);
    }
  
  nbr_of_frames=src->nbr_of_frames;
  frame=new animation_frame[nbr_of_frames];
  for(i=0;i<nbr_of_frames;i++)
    {
      frame[i]=src->frame[i];
      frame[i].gapx=(src->frame[i].gapx*sx)/src->length;
      frame[i].gapy=(src->frame[i].gapy*sy)/src->height;
    }
}

void animation::get_zoom_scale(u_int16 &max_x, u_int16 &max_y)
{
  u_int16 i;
  for(i=0;i<nbr_of_images;i++)
    {
      
    }
}

#ifdef _EDIT_
s_int8 animation::put(gzFile file)
{
  u_int16 i;
  gzwrite(file,&length,sizeof(length));
  gzwrite(file,&height,sizeof(height));
  gzwrite(file,&nbr_of_images,sizeof(nbr_of_images));
  for(i=0;i<nbr_of_images;i++)
    t_frame[i].put_raw(file);
  gzwrite(file,&nbr_of_frames,sizeof(nbr_of_frames));
  for(i=0;i<nbr_of_frames;i++)
      frame[i].put(file);
  currentframe=0;
  return(0);
}

s_int8 animation::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return(-1);
  retvalue=put(file);
  gzclose(file);
  return(retvalue);
}

void animation::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s) { delete qw; return; }
  if(save(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error saving!");
      wi->wait_for_keypress(makeFunctor(*this,&animation::update_editor),
			    makeFunctor(*this,&animation::draw_editor));
      delete wi;
    }
  delete qw;
}

void animation::load()
{
  animation * t=new animation;
  win_query * qw=new win_query(70,40,th,font,"Load animation:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s)
    {
      delete t;
      delete qw;
      return;
    }
  if(t->load(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&animation::update_editor),
			    makeFunctor(*this,&animation::draw_editor));
      delete wi;
    }

  else 
    {
      currentimage=0;
      *(animation*)this=*t;
    }
  delete t;
  delete qw;
  if(in_editor) 
    {
      must_upt_label_mode=true;
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
      must_upt_label_status=true;
    }
}


#endif

animation & animation::operator =(animation &a)
{
  u_int16 i;
  length=a.length;
  height=a.height;
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

#ifdef _EDIT_

void animation::select_image(u_int16 nbr)
{
  currentimage=nbr;
  must_upt_label_frame_nbr=true;
  must_upt_label_frame_info=true;
}


void animation::select_frame(u_int16 nbr)
{
  currentframe=nbr;
  must_upt_label_frame_nbr=true;
  must_upt_label_frame_info=true;
}

void animation::set_frame_gapx(u_int16 nbr, s_int16 gx)
{
  if(gx<0) return;
  frame[nbr].gapx=gx;
  must_upt_label_frame_info=true;
}

void animation::set_frame_gapy(u_int16 nbr, s_int16 gy)
{
  if(gy<0) return;
  frame[nbr].gapy=gy;
  must_upt_label_frame_info=true;
}

void animation::set_frame_alpha(u_int16 nbr, u_int8 a)
{
  frame[nbr].set_alpha(a);
  must_upt_label_frame_info=true;
}

void animation::set_frame_delay(u_int16 nbr, u_int16 d)
{
  frame[nbr].set_delay(d);
  must_upt_label_frame_info=true;
}

void animation::set_frame_nextframe(u_int16 nbr, u_int16 nf)
{
  frame[nbr].set_nextframe(nf);
  must_upt_label_frame_info=true;
}

void animation::set_frame_mask(u_int16 nbr, bool m)
{
  frame[nbr].set_mask(m);
  must_upt_label_frame_info=true;
}

void animation::set_frame_imagenbr(u_int16 nbr, u_int16 imnbr)
{
  frame[nbr].imagenbr=imnbr;
  must_upt_label_frame_info=true;
}

s_int8 animation::insert_image(image &im, u_int16 pos)
{
  image * oldt_frame=t_frame;
  u_int16 i;
  if(pos>nbr_of_images) return -2;
  t_frame=new image[++nbr_of_images];
  for(i=0;i<pos;i++)
    t_frame[i]=oldt_frame[i];
  t_frame[pos]=im;
  for(i=pos+1;i<nbr_of_images;i++)
    t_frame[i]=oldt_frame[i-1];
  for(i=0;i<nbr_of_frames;i++)
    if(frame[i].imagenbr>=pos) frame[i].imagenbr++;
  delete[] oldt_frame;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#ifdef _DEBUG_
  cout << "Added image: " << nbr_of_images << " total in animation.\n";
#endif
  return 0;
}

s_int8 animation::insert_frame(animation_frame &af, u_int16 pos)
{
  animation_frame * oldframe=frame;
  u_int16 i;
  if(pos>nbr_of_frames) return -2;
  frame=new animation_frame[++nbr_of_frames];
  for(i=0;i<pos;i++)
    frame[i]=oldframe[i];
  frame[pos]=af;
  for(i=pos+1;i<nbr_of_frames;i++)
    frame[i]=oldframe[i-1];
  for(i=0;i<nbr_of_frames;i++)
    if(frame[i].nextframe>=pos) frame[i].nextframe++; 
  delete[] oldframe;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#ifdef _DEBUG_
  cout << "Added frame: " << nbr_of_frames << " total in animation.\n";
#endif
  return 0;
}

void animation::add_image()
{
  image im;
  u_int16 p;
  win_query * qw=new win_query(70,40,th,font,"Load PNM file:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s) { delete qw; return; }
  if(!im.load_pnm(s)) 
    {
      do
	{
	  char tmp[255];
	  char * s2;
	  sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_images,
		  nbr_of_images);
	  win_query * qw2=new win_query(70,40,th,font,tmp);
	  s2=qw2->wait_for_text(makeFunctor(*this,
					    &animation::update_editor),
				makeFunctor(*this,
					    &animation::draw_editor));
	  if(!s2)
	    { 
	      delete qw2;
	      delete qw;
	      return;
	    }
	  if(!s2[0]) p=nbr_of_images;
	  else p=atoi(s2);
	  delete qw2;
	}
      while(p>nbr_of_images);
      insert_image(im,p);
    }
  else
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading PNM file!");
      wi->wait_for_keypress(makeFunctor(*this,&animation::update_editor),
			    makeFunctor(*this,&animation::draw_editor));
      delete wi;
    }
  delete qw;
}

s_int8 animation::delete_image(u_int16 pos)
{
  image * oldt_frame=t_frame;
  u_int16 i;
  if(pos>nbr_of_images-1) return -2;
  t_frame=new image[--nbr_of_images];
  for(i=0;i<pos;i++)
    t_frame[i]=oldt_frame[i];
  for(i=pos;i<nbr_of_images;i++)
    t_frame[i]=oldt_frame[i+1];
  for(i=0;i<nbr_of_frames;i++)
    {
      if((frame[i].imagenbr>=pos)&&(frame[i].imagenbr>0)) frame[i].imagenbr--;
      if((frame[i].nextframe>=pos)&&(frame[i].nextframe>0)) 
	frame[i].nextframe--;
    }
  delete[] oldt_frame;
  if(currentimage>=nbr_of_images) currentimage=nbr_of_images-1;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#ifdef _DEBUG_
  cout << "Removed image: " << nbr_of_images << " total in animation.\n";
#endif
  if(!nbr_of_images) 
    {
      t_frame=NULL;
      currentimage=0;
      delete[] frame;
      frame=NULL;
      nbr_of_frames=0;
      currentframe=0;
    }
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
  for(i=0;i<nbr_of_frames;i++)
    if(frame[i].nextframe>=pos && frame[i].nextframe) frame[i].nextframe--; 
  delete[] oldframe;
  if(currentframe>=nbr_of_frames) currentframe=nbr_of_frames-1;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#ifdef _DEBUG_
  cout << "Removed frame: " << nbr_of_frames << " total in animation.\n";
#endif
  if(!nbr_of_frames) 
    {
      frame=NULL;
      currentframe=0;
    }
  return 0;
}

void animation::add_frame()
{
  animation_frame af;
  if(!nbr_of_images)
    {
      win_info * wi=new win_info(70,40,th,font,"You must have at least one image in your animation before thinking about inserting a frame!");
      wi->wait_for_keypress(makeFunctor(*this,&animation::update_editor),
			    makeFunctor(*this,&animation::draw_editor));
      delete wi;
    }

  else insert_frame(af,nbr_of_frames);
}

u_int16 animation::increase_frame(u_int16 c)
{
  c++;
  if(c==nbr_of_frames) c=0;
  return c;
}

u_int16 animation::decrease_frame(u_int16 c)
{
  if(c==0) c=nbr_of_frames-1;
  else c--;
  return c;
}

u_int16 animation::increase_image(u_int16 c)
{
  c++;
  if(c==nbr_of_images) c=0;
  return c;
}

u_int16 animation::decrease_image(u_int16 c)
{
  if(c==0) c=nbr_of_images-1;
  else c--;
  return c;
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

void animation::set_mode(anim_editor_mode m)
{
  mode=m;
  must_upt_label_mode=true;
  must_upt_label_frame_nbr=true;
  must_upt_label_frame_info=true;
  must_upt_label_status=true;
}

void animation::update_label_mode()
{
  sprintf(frame_txt,"%s mode",mode==IMAGE?"Image":"Frame");
  label_mode->set_text(frame_txt);
}

void animation::update_label_frame_nbr()
{
  if(mode==FRAME)
    {
      if(nbr_of_frames>0)
	sprintf(frame_txt,"Frame %d/%d",currentframe,
		nbr_of_frames-1);
      else sprintf(frame_txt,"No frame");
    }
  else
    {
      if(nbr_of_images>0)
	sprintf(frame_txt,"Image %d/%d",currentimage,
		nbr_of_images-1);
      else sprintf(frame_txt,"No image");
    }
  label_frame_nbr->set_text(frame_txt);
}

void animation::update_label_frame_info()
{
  if(mode==FRAME)
    {
      if(nbr_of_frames>0)
	{
	  char s_delay[6];
	  sprintf(s_delay,"%d",frame[currentframe].delay);
	  sprintf(frame_txt,"Frame info :\nImage number: %d\nLength: %d\n"
		  "Height: %d\nDelay: %s\nX Offset:%d\nY Offset:%d\n"
		  "Next Frame: %d\nMasked: %s\nAlpha: %d\n"
		  "\nAnimation info:\nLength: %d\nHeight:%d\n",
		  frame[currentframe].imagenbr,
		  t_frame[frame[currentframe].imagenbr].length,
		  t_frame[frame[currentframe].imagenbr].height,
		  frame[currentframe].delay==0?"Infinite":s_delay,
		  frame[currentframe].gapx,frame[currentframe].gapy,
		  frame[currentframe].nextframe,
		  frame[currentframe].get_mask()?"Yes":"No",
		  frame[currentframe].get_alpha(),
		  length,height);
	  label_frame_info->set_text(frame_txt);
	}
      else
	{
	  sprintf(frame_txt,"No frame, press \"a\" to add one.");
	  label_frame_info->set_text(frame_txt);
	}
    }
  else
    {
      if(nbr_of_images>0)
	{
	  sprintf(frame_txt,"Image info:\nLength: %d\nHeight:%d\n\nAnimation info:\nLength:%d\nHeight:%d",
		  t_frame[currentimage].length,t_frame[currentimage].height,
		  length,height);
	  label_frame_info->set_text(frame_txt);
	}
      else
	{
	  sprintf(frame_txt,"No image, press \"a\" to add one.");
	  label_frame_info->set_text(frame_txt);
	}
    }
}

void animation::update_label_status()
{
  if(mode==FRAME)
    {
      sprintf(frame_txt,"%s",play_flag?"Playing":"Stopped");
      label_anim_info->set_text(frame_txt);
    }
  else label_anim_info->set_text("");
}

void animation::update_editor_keys()
{
  // Mode switching
  if(input::has_been_pushed(SDLK_F1)) set_mode(IMAGE);
  if(input::has_been_pushed(SDLK_F2)) set_mode(FRAME);

  // General functions
  if(input::has_been_pushed(SDLK_F5)) save();
  if(input::has_been_pushed(SDLK_F6)) load();

  // Editor keys

  if(testkey(SDLK_RIGHT)) 
    {
      if(mode==IMAGE) select_image(increase_image(currentimage));
      if((mode==FRAME)&&(!play_flag))
	if(!(SDL_GetModState()&KMOD_LSHIFT))
	  {select_frame(increase_frame(currentframe));}
	else set_frame_gapx(currentframe,frame[currentframe].gapx+1);	    
    }
  
  if(testkey(SDLK_LEFT))
    {
      if(mode==IMAGE) select_image(decrease_image(currentimage));
      if((mode==FRAME)&&(!play_flag))
	if(!(SDL_GetModState()&KMOD_LSHIFT))
	  {select_frame(decrease_frame(currentframe));}
     	else set_frame_gapx(currentframe,frame[currentframe].gapx-1);
    }

  if(input::has_been_pushed(SDLK_a))
    {
      if(mode==IMAGE) add_image();
      if((mode==FRAME)&&(!play_flag)) add_frame();
    }
  
  if(input::has_been_pushed(SDLK_r))
    {
      if(mode==IMAGE)
	{
	  image im;
	  im=t_frame[currentimage];
	  t_frame[currentimage].reverse_lr(&im);
	}
    }

  if(input::has_been_pushed(SDLK_t))
    {
      if(mode==IMAGE)
	{
	  image im;
	  im=t_frame[currentimage];
	  t_frame[currentimage].reverse_ud(&im);
	}
    }

  if(input::has_been_pushed(SDLK_c))
    {
      if(SDL_GetModState()&&KMOD_LCTRL)
	{
	  if(mode==IMAGE)
	    *clipboard=t_frame[currentimage];
	  if(mode==FRAME)
	    f_clipboard=frame[currentframe];
	}
    }

  if(input::has_been_pushed(SDLK_v))
    {
      if(SDL_GetModState()&&KMOD_LCTRL)
	{
	  if(mode==IMAGE)
	    t_frame[currentimage]=*clipboard;
	  if(mode==FRAME)
	    {
	      frame[currentframe].is_masked=f_clipboard.is_masked;
	      frame[currentframe].alpha=f_clipboard.alpha;
	    }
	}
    }

  if(input::has_been_pushed(SDLK_b))
    {
      if(SDL_GetModState()&&KMOD_LCTRL)
	{
	  if(mode==IMAGE)
	    insert_image(*clipboard,nbr_of_images);
	  if(mode==FRAME)
	    {
	      animation_frame af;
	      af.is_masked=f_clipboard.is_masked;
	      af.alpha=f_clipboard.alpha;
	      insert_frame(af,nbr_of_frames);
	    }
	}
    }

  if(input::has_been_pushed(SDLK_d))
    {
      if(mode==IMAGE) delete_image(currentimage);
      if((mode==FRAME)&&(!play_flag)) delete_frame(currentframe);
    }

  if(input::has_been_pushed(SDLK_p)) play();
  if(input::has_been_pushed(SDLK_o)) stop();
  if(input::has_been_pushed(SDLK_i)) rewind();

  if(testkey(SDLK_KP_PLUS)&&mode==FRAME&&!play_flag)
    set_frame_imagenbr(currentframe,
		       increase_image(frame[currentframe].imagenbr));
  
  if(testkey(SDLK_KP_MINUS)&&mode==FRAME&&!play_flag)
    set_frame_imagenbr(currentframe,
		       decrease_image(frame[currentframe].imagenbr));

  if(testkey(SDLK_UP)&&mode==FRAME&&!play_flag) 
    if(!(SDL_GetModState()&KMOD_LSHIFT))
      {set_frame_delay(currentframe,frame[currentframe].delay+1);}
    else set_frame_gapy(currentframe,frame[currentframe].gapy-1);

  if(testkey(SDLK_DOWN)&&mode==FRAME&&!play_flag) 
    if(!(SDL_GetModState()&KMOD_LSHIFT))
      {set_frame_delay(currentframe,frame[currentframe].delay-1);}
    else set_frame_gapy(currentframe,frame[currentframe].gapy+1);

  if(testkey(SDLK_PAGEUP)&&mode==FRAME&&!play_flag)
    set_frame_alpha(currentframe,frame[currentframe].get_alpha()+1);
  if(testkey(SDLK_PAGEDOWN)&&mode==FRAME&&!play_flag) 
    set_frame_alpha(currentframe,frame[currentframe].get_alpha()-1);

  if(testkey(SDLK_HOME)&&mode==FRAME&&!play_flag)
    set_frame_nextframe(currentframe,
			increase_frame(frame[currentframe].nextframe));
  if(testkey(SDLK_END)&&mode==FRAME&&!play_flag) 
    set_frame_nextframe(currentframe,
			decrease_frame(frame[currentframe].nextframe));
  
  if(testkey(SDLK_INSERT)&&mode==FRAME&&!play_flag)
    set_frame_mask(currentframe,frame[currentframe].is_masked==true?
		   false:true);
}

void animation::update_editor()
{  
  update();
  u_int16 i;
  length=0;
  height=0;
  for(i=0;i<nbr_of_frames;i++)
    {
      u_int16 tl,th;
      if((tl=t_frame[frame[i].imagenbr].
	get_length()+frame[i].gapx)>length)
	length=tl;

      if((th=t_frame[frame[i].imagenbr].
	get_height()+frame[i].gapy)>height)
	  height=th;
    }
}

void animation::draw_editor()
{
  bg->draw(0,0);
  if(mode==FRAME) draw(0,0);
  else if(nbr_of_images>0) t_frame[currentimage].putbox(0,0);
  container->draw();
  if(must_upt_label_mode) {update_label_mode(); must_upt_label_mode=false;}
  if(must_upt_label_frame_nbr) {update_label_frame_nbr(); 
  must_upt_label_frame_nbr=false;}
  if(must_upt_label_frame_info) {update_label_frame_info(); 
  must_upt_label_frame_info=false;}
  if(must_upt_label_status) {update_label_status(); 
  must_upt_label_status=false;}
}

void animation::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do();i++) update_editor();
  draw_editor();
}

void animation::editor()
{
  u_int16 i;
  image temp;
  string t;
  t=WIN_DIRECTORY;
  t+=WIN_BACKGROUND_DIRECTORY;
  t+=WIN_THEME_ORIGINAL;
  t+=WIN_BACKGROUND_FILE;
  clipboard=new image;
  temp.load_pnm(t.data());
  bg=new image(320,240);
  bg->putbox_tile_img(&temp);
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);
  container=new win_container(200,12,110,216,th);
  label_mode=new win_label(5,5,100,30,th,font);
  label_frame_nbr=new win_label(5,35,100,30,th,font);
  label_anim_info=new win_label(5,65,100,65,th,font);
  label_frame_info=new win_label(5,80,100,120,th,font);
  container->add(label_mode);
  container->add(label_frame_nbr);
  container->add(label_anim_info);
  container->add(label_frame_info);
  container->set_visible_all(true);
  container->set_border_visible(true);
  must_upt_label_mode=false;
  must_upt_label_frame_nbr=false;
  must_upt_label_frame_info=false;
  must_upt_label_status=false;  
  set_mode(IMAGE);
  stop();
  rewind();
  in_editor=true;
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      input::update();
      for(i=0;i<screen::frames_to_do();i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }
  input::clear_keys_queue();
  in_editor=false;
  delete container;
  delete bg;
  delete th;
  delete font;
  delete clipboard;
}

#endif
