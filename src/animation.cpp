// $Id$

/*
  Copyright (C) 1999/2000/2001   The Adonthell Project
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
u_int16 animationframe::a_d_diff=0;
#endif

#ifdef _DEBUG_
u_int16 animation::a_d_diff=0;
#endif

void animationframe::init()
{
  imagenbr=0;
  is_masked_=false;
  set_alpha(255);
  gapx=0;
  gapy=0;
  delay_=0;
  nextframe_=0;
}

void animationframe::clear()
{
  init();
}

animationframe::animationframe()
{
  init();
#ifdef _DEBUG_
  cout << "animationframe() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

animationframe::~animationframe()
{
#ifdef _DEBUG_
  cout << "~animationframe() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

s_int8 animationframe::get(gzFile file)
{
  gzread(file,&imagenbr,sizeof(imagenbr));
  gzread(file,&is_masked_,sizeof(is_masked_));
  gzread(file,&alpha_,sizeof(alpha_));
  set_alpha(alpha_);
  gzread(file,&gapx,sizeof(gapx));
  gzread(file,&gapy,sizeof(gapy));
  gzread(file,&delay_,sizeof(delay_));
  gzread(file,&nextframe_,sizeof(nextframe_));
  return(0);
}

s_int8 animationframe::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return(-1);
  retvalue=get(file);
  gzclose(file);
  return(retvalue);
}

s_int8 animation::insert_image(image * im, u_int16 pos)
{
  vector<image *>::iterator i;
  vector<animationframe>::iterator j;
  if(pos>nbr_of_images()) return -2;
  i=t_frame.begin();
  while(pos--) i++;
  t_frame.insert(i,im);
  for (j=frame.begin();j!=frame.end();j++)
    if (j->image_nbr() >= pos) j->set_image_nbr(j->image_nbr()+1);
#ifdef _DEBUG_
  cout << "Added image: " << nbr_of_images() << " total in animation.\n";
#endif
#ifdef _EDIT_
  if(in_editor)
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#endif
  return 0;
}

s_int8 animation::delete_image(u_int16 pos)
{
  vector<image *>::iterator i;
  vector<animationframe>::iterator j;

  if(pos>nbr_of_images()-1) return -2;
  i=t_frame.begin();
  while(pos--) i++;
  t_frame.erase(i);
  for(j=frame.begin();j!=frame.end();j++)
    if (j->image_nbr() >= pos) j->set_image_nbr(j->image_nbr()-1);
#ifdef _EDIT_
  if(currentimage>=nbr_of_images()) currentimage=nbr_of_images()-1;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#endif  
#ifdef _DEBUG_
  cout << "Removed image: " << nbr_of_images() << " total in animation.\n";
#endif
#ifdef _EDIT_
  if(!nbr_of_images())
    {
      currentimage=0;
      frame.clear();
      currentframe_=0;
    }
#endif
  return 0;
}

s_int8 animation::insert_frame(animationframe af, u_int16 pos)
{
  vector<animationframe>::iterator i;
  if(pos>nbr_of_frames()) return -2;
  i=frame.begin();
  while(pos--) i++;
  frame.insert(i,af);
  for (i=frame.begin(); i!=frame.end(); i++)
    if (i->nextframe() >= pos) i->set_nextframe(i->nextframe()+1);
#ifdef _DEBUG_
  cout << "Added frame: " << nbr_of_frames() << " total in animation.\n";
#endif
  return 0;
}

s_int8 animation::delete_frame(u_int16 pos)
{
  vector<animationframe>::iterator i;
  if(pos>nbr_of_frames()-1) return -2;
  for (i=frame.begin(); i!=frame.end(); i++)
    if (i->nextframe() >= pos) 
      i->set_nextframe(frame[i->nextframe()].nextframe()); 
  i=frame.begin();
  while(pos--) i++;
  frame.erase(i);
#ifdef _EDIT_
  if(currentframe_>=nbr_of_frames()) currentframe_=nbr_of_frames()-1;
  if(in_editor) 
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#endif
#ifdef _DEBUG_
  cout << "Removed frame: " << nbr_of_frames() << " total in animation.\n";
#endif
  if(!nbr_of_frames()) 
    currentframe_=0;
  return 0;
}

#ifdef _EDIT_
s_int8 animationframe::put(gzFile file)
{
  gzwrite(file,&imagenbr,sizeof(imagenbr));
  gzwrite(file,&is_masked_,sizeof(is_masked_));
  set_alpha(alpha_);
  gzwrite(file,&alpha_,sizeof(alpha_));
  set_alpha(alpha_);
  gzwrite(file,&gapx,sizeof(gapx));
  gzwrite(file,&gapy,sizeof(gapy));
  gzwrite(file,&delay_,sizeof(delay_));
  gzwrite(file,&nextframe_,sizeof(nextframe_));
  return(0);
}

s_int8 animationframe::save(const char * fname)
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
  frame.clear();
  t_frame.clear();
  currentframe_=0;
  speedcounter=0;
  play_flag=false;
  length_=height_=0;
  xoffset_=0;
  yoffset_=0;
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
  vector<image *>::iterator i;
  for(i=t_frame.begin();i!=t_frame.end();i++)
    delete (*i);
  t_frame.clear();
  frame.clear();
  init();
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
  return((!nbr_of_frames())||(!nbr_of_images()));
}

void animation::update()
{
  if((!play_flag)||(!nbr_of_frames())) return;
  if(frame[currentframe()].delay_==0) return;
  if(nbr_of_frames()<=1) return;
    
  speedcounter++;
  if (speedcounter>=frame[currentframe()].delay_)
    next_frame();
}

void animation::next_frame()
{
  currentframe_=frame[currentframe()].nextframe_;
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
  currentframe_=0;
  speedcounter=0;
}

void animation::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
#ifdef _EDIT_
  if(!nbr_of_frames()) return;
#endif

  t_frame[frame[currentframe()].image_nbr()]->set_mask(frame[currentframe()].is_masked());
  t_frame[frame[currentframe()].image_nbr()]->set_alpha(frame[currentframe()].alpha());
  
  t_frame[frame[currentframe()].image_nbr()]->draw(x+xoffset()+frame[currentframe()].gapx,
					     y+yoffset()+frame[currentframe()].gapy,
					     da_opt);
}

void animation::draw_border(u_int16 x, u_int16 y, 
			    drawing_area * da_opt=NULL)
{
  screen::drawbox(x+xoffset(),y+yoffset(),length(),1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset(),y+yoffset()+height(),length(),1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset()+length(),y+yoffset(),1,height()+1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset(),y+yoffset(),1,height()+1,0xFFFFFF,da_opt);
}

s_int8 animation::get(gzFile file)
{
  u_int16 i;
  u_int16 nbr_images;
  u_int16 nbr_frames;
  clear();
  // TODO: Remove this! (length and height are calculated later)
  gzread(file,&length_,sizeof(length_));
  gzread(file,&height_,sizeof(height_));
  // Read images
  gzread(file,&nbr_images,sizeof(nbr_images));
  for(i=0;i<nbr_images;i++)
    {
      t_frame.push_back(new image);
      t_frame.back()->get_raw(file);
    }
  // Read frames
  gzread(file,&nbr_frames,sizeof(nbr_frames));
  animationframe aftemp;
  for(i=0;i<nbr_frames;i++)
    {
      frame.push_back(aftemp);
      frame.back().get(file);
    }

  // Calculate length and height
  // TODO: move this into a separate protected function.
  for(i=0;i<nbr_of_frames();i++)
    {
      u_int16 tl,th;
      if((tl=t_frame[frame[i].imagenbr]->length()+frame[i].gapx)>length())
	length_=tl;
      
      if((th=t_frame[frame[i].imagenbr]->height()+frame[i].gapy)>height())
	height_=th;
    }
  currentframe_=0;
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

s_int8 animation::get_off(gzFile file)
{
  u_int16 t_xoffset, t_yoffset;
  gzread(file,&t_xoffset,sizeof(t_xoffset));
  gzread(file,&t_yoffset,sizeof(t_yoffset));
  get(file);
  set_offset(t_xoffset,t_yoffset);
  return 0;
}

s_int8 animation::load_off(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return -1;
  retvalue=get_off(file);
  gzclose(file);
  return retvalue;
}

void animation::zoom(u_int16 sx, u_int16 sy, animation * src)
{
  static u_int16 i;
  clear();
  for(i=0;i<src->nbr_of_images();i++)
    {
      image * im=new image;
      im->resize((src->t_frame[i]->length()*sx)/src->length(),
			(src->t_frame[i]->height()*sy)/src->height());
      im->zoom(src->t_frame[i]);
      t_frame.push_back(im);
    }
  
  for(i=0;i<src->nbr_of_frames();i++)
    {
      frame.push_back(src->frame[i]);
      frame.back().gapx=(src->frame[i].gapx*sx)/src->length();
      frame.back().gapy=(src->frame[i].gapy*sy)/src->height();
    }
}

#ifdef _EDIT_
s_int8 animation::put(gzFile file)
{
  u_int16 i;
  u_int16 nbr_images=(u_int16)nbr_of_images();
  u_int16 nbr_frames=(u_int16)nbr_of_frames();
  gzwrite(file,&length_,sizeof(length_));
  gzwrite(file,&height_,sizeof(height_));
  gzwrite(file,&nbr_images,sizeof(nbr_images));
  for(i=0;i<nbr_images;i++)
    t_frame[i]->put_raw(file);
  gzwrite(file,&nbr_frames,sizeof(nbr_frames));
  for(i=0;i<nbr_frames;i++)
      frame[i].put(file);
  set_currentframe(0);
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

s_int8 animation::put_off(gzFile file)
{
  gzwrite(file,&xoffset_,sizeof(xoffset_));
  gzwrite(file,&yoffset_,sizeof(yoffset_));
  put(file);
  return 0;
}

s_int8 animation::save_off(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return -1;
  retvalue=put_off(file);
  gzclose(file);
  return retvalue;
}

void animation::quick_save()
{
  char s[500];
  if(!strcmp(file_name,""))
    {
      sprintf(s,"You should save (F5) before calling this...");
    }
  else if(save(file_name))
    {
      sprintf(s,"Error saving %s!",file_name);
    }
  else
    {
      sprintf(s,"%s saved successfully!",file_name);
    }
  set_info_win(s);      
}

void animation::quick_load()
{
  char s[500];
  if(!strcmp(file_name,""))
    {
      sprintf(s,"You should load (F6) before calling this...");
    }
  else if(load(file_name))
    {
      sprintf(s,"Error saving %s!",file_name);
    }
  else
    {
      sprintf(s,"%s loaded successfully!",file_name);
    }
  set_info_win(s);      
}

void animation::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save animation as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s) return;
  char st[500];
  if(save(s))
    {
      sprintf(st,"Error saving %s!",s);
    }
  else sprintf(st,"%s saved successfully!",s);
  strcpy(file_name,s);
  set_info_win(st);
  delete qw;
}

void animation::load()
{
  animation * t=new animation;
  win_file_select * wf=new win_file_select(60,20,200,200,th,font,".anim","./");
  char * s=wf->wait_for_select(makeFunctor(*this,&animation::update_editor),
			       makeFunctor(*this,&animation::draw_editor));
  if(!s)
    {
      delete t;
      delete wf;
      return;
    }
  char st[500];
  if(t->load(s)) 
    {
      sprintf(st,"Error loading %s!",s);
    }
  else 
    {
      sprintf(st,"%s loaded successfully!",s);
      set_currentframe(0);
      currentimage=currentframe();
      *(animation*)this=*t;
    }
  strcpy(file_name,s);
  set_info_win(st);
  delete t;
  delete wf;
  must_upt_label_mode=true;
  must_upt_label_frame_nbr=true;
  must_upt_label_frame_info=true;
  must_upt_label_status=true;
}


#endif

animation & animation::operator =(animation &a)
{
  u_int16 i;
  for(i=0;i<nbr_of_images();i++)
    delete t_frame[i];
  t_frame.clear();
  length_=a.length_;
  height_=a.height_;
  currentframe_=a.currentframe_;
  speedcounter=a.speedcounter;
  play_flag=a.play_flag;
  for(i=0;i<a.nbr_of_images();i++)
    {
      t_frame.push_back(new image);
      *(t_frame[i])=*(a.t_frame[i]);
    }
  frame.clear();
  for(i=0;i<a.nbr_of_frames();i++)
    frame.push_back(a.frame[i]);
  set_offset(a.xoffset(),a.yoffset());
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
  set_currentframe(nbr);
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

void animation::ed_add_image()
{
  image * im=new image;
  u_int16 p;
  win_file_select * wf=new win_file_select(60,20,200,200,th,font,".pnm","./");
  char * s=wf->wait_for_select(makeFunctor(*this,&animation::update_editor),
			       makeFunctor(*this,&animation::draw_editor));
  if(!s) { delete wf; return; }
  char st[500];
  if(!im->load_pnm(s)) 
    {
      do
	{
	  char tmp[255];
	  char * s2;
	  sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_images(),
		  nbr_of_images());
	  win_query * qw2=new win_query(70,40,th,font,tmp);
	  s2=qw2->wait_for_text(makeFunctor(*this,
					    &animation::update_editor),
				makeFunctor(*this,
					    &animation::draw_editor));
	  if(!s2)
	    { 
	      delete qw2;
	      delete wf;
	      return;
	    }
	  if(!s2[0]) p=nbr_of_images();
	  else p=atoi(s2);
	  delete qw2;
	}
      while(p>nbr_of_images());
      insert_image(im,p);
      sprintf(st,"%s loaded successfuly!",s);
    }
  else
    {
      delete im;
      sprintf(st,"Error loading %s!",s);
    }
  set_info_win(st);
  delete wf;
}

void animation::ed_add_raw_image()
{
  image * im=new image;
  u_int16 p;
  win_file_select * wf=new win_file_select(60,20,200,200,th,font,".img","./");
  char * s=wf->wait_for_select(makeFunctor(*this,&animation::update_editor),
			       makeFunctor(*this,&animation::draw_editor));
  if(!s) { delete wf; return; }
  char st[500];
  if(!im->load(s)) 
    {
      do
	{
	  char tmp[255];
	  char * s2;
	  sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_images(),
		  nbr_of_images());
	  win_query * qw2=new win_query(70,40,th,font,tmp);
	  s2=qw2->wait_for_text(makeFunctor(*this,
					    &animation::update_editor),
				makeFunctor(*this,
					    &animation::draw_editor));
	  if(!s2)
	    { 
	      delete qw2;
	      delete wf;
	      return;
	    }
	  if(!s2[0]) p=nbr_of_images();
	  else p=atoi(s2);
	  delete qw2;
	}
      while(p>nbr_of_images());
      insert_image(im,p);
      sprintf(st,"%s loaded successfuly!",s);
    }
  else
    {
      delete im;
      sprintf(st,"Error loading %s!",s);
    }
  set_info_win(st);
  delete wf;
}

void animation::save_image()
{
  if(!nbr_of_images())
    {
      set_info_win("There are no image to save!");
      return;
    }
  win_query * qw=new win_query(70,40,th,font,"Save image as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s) return;
  char st[500];
  if(t_frame[currentimage]->save_pnm(s))
    {
      sprintf(st,"Error saving %s!",s);
    }
  else sprintf(st,"%s saved successfully!",s);
  strcpy(file_name,s);
  set_info_win(st);
  delete qw;
}

void animation::save_image_raw()
{
  if(!nbr_of_images())
    {
      set_info_win("There are no image to save!");
      return;
    }
  win_query * qw=new win_query(70,40,th,font,"Save raw image as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &animation::update_editor),
			     makeFunctor(*this,
					 &animation::draw_editor));
  if(!s) return;
  char st[500];
  if(t_frame[currentimage]->save(s))
    {
      sprintf(st,"Error saving %s!",s);
    }
  else sprintf(st,"%s saved successfully!",s);
  strcpy(file_name,s);
  set_info_win(st);
  delete qw;
}

void animation::ed_add_frame()
{
  animationframe af;
  if(!nbr_of_images())
    {
      win_info * wi=new win_info(70,40,th,font,"You must have at least one image in your animation before thinking about inserting a frame!");
      wi->wait_for_keypress(makeFunctor(*this,&animation::update_editor),
			    makeFunctor(*this,&animation::draw_editor));
      delete wi;
    }
  else insert_frame(af,nbr_of_frames());
#ifdef _EDIT_
  if(in_editor)
    {
      must_upt_label_frame_nbr=true;
      must_upt_label_frame_info=true;
    }
#endif
}

u_int16 animation::increase_frame(u_int16 c)
{
  c++;
  if(c==nbr_of_frames()) c=0;
  return c;
}

u_int16 animation::decrease_frame(u_int16 c)
{
  if(c==0) c=nbr_of_frames()-1;
  else c--;
  return c;
}

u_int16 animation::increase_image(u_int16 c)
{
  c++;
  if(c==nbr_of_images()) c=0;
  return c;
}

u_int16 animation::decrease_image(u_int16 c)
{
  if(c==0) c=nbr_of_images()-1;
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
      if(nbr_of_frames()>0)
	sprintf(frame_txt,"Frame %d/%d",currentframe(),
		nbr_of_frames()-1);
      else sprintf(frame_txt,"No frame");
    }
  else
    {
      if(nbr_of_images()>0)
	sprintf(frame_txt,"Image %d/%d",currentimage,
		nbr_of_images()-1);
      else sprintf(frame_txt,"No image");
    }
  label_frame_nbr->set_text(frame_txt);
}

void animation::update_label_frame_info()
{
  if(mode==FRAME)
    {
      if(nbr_of_frames()>0)
	{
	  char s_delay[6];
	  sprintf(s_delay,"%d",frame[currentframe()].delay());
	  sprintf(frame_txt,"Frame info :\nImage number: %d\nLength: %d\n"
		  "Height: %d\nDelay: %s\nX Offset:%d\nY Offset:%d\n"
		  "Next Frame: %d\nMasked: %s\nAlpha: %d\n"
		  "\nAnimation info:\nLength: %d\nHeight:%d\n",
		  frame[currentframe()].imagenbr,
		  t_frame[frame[currentframe()].image_nbr()]->length(),
		  t_frame[frame[currentframe()].image_nbr()]->height(),
		  frame[currentframe()].delay()==0?"Infinite":s_delay,
		  frame[currentframe()].gapx,frame[currentframe()].gapy,
		  frame[currentframe()].nextframe(),
		  frame[currentframe()].is_masked()?"Yes":"No",
		  frame[currentframe()].alpha(),
		  length(),height());
	  label_frame_info->set_text(frame_txt);
	}
      else
	{
	  sprintf(frame_txt,"No frame, press \"F1\" to add one.");
	  label_frame_info->set_text(frame_txt);
	}
    }
  else
    {
      if(nbr_of_images()>0)
	{
	  sprintf(frame_txt,"Image info:\nLength: %d\nHeight: %d\n\nAnimation info:\nLength:%d\nHeight:%d",
		  t_frame[currentimage]->length(),t_frame[currentimage]->height(),
		  length(),height());
	  label_frame_info->set_text(frame_txt);
	}
      else
	{
	  sprintf(frame_txt,"No image, press \"F1\" to add one.");
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
  // Image mode
  if(input::has_been_pushed(SDLK_g)) set_mode(IMAGE);
  // Frame mode
  if(input::has_been_pushed(SDLK_f)) set_mode(FRAME);

  // Save
  if(input::has_been_pushed(SDLK_F5))
    { 
      if(SDL_GetModState()&KMOD_LSHIFT)
	quick_save();
      else save(); 
    }

  // Load
  if(input::has_been_pushed(SDLK_F6))
    { 
      if(SDL_GetModState()&KMOD_LSHIFT)
	quick_load();
      else load(); 
    }

  // Navigate through images and frames
  // + Shift: set the X offset in frame mode
  if(testkey(SDLK_RIGHT)) 
    {
      if(mode==IMAGE) select_image(increase_image(currentimage));
      if((mode==FRAME)&&(!play_flag))
	if(!(SDL_GetModState()&KMOD_SHIFT))
	  {select_frame(increase_frame(currentframe()));}
	else set_frame_gapx(currentframe(),frame[currentframe()].gapx+1);	    
    }
  
  // Navigate through images and frames
  // + Shift: set the X offset in frame mode
  if(testkey(SDLK_LEFT))
    {
      if(mode==IMAGE) select_image(decrease_image(currentimage));
      if((mode==FRAME)&&(!play_flag))
	if(!(SDL_GetModState()&KMOD_SHIFT))
	  {select_frame(decrease_frame(currentframe()));}
     	else set_frame_gapx(currentframe(),frame[currentframe()].gapx-1);
    }

  // Increase the frame delay in frame mode
  // + Shift: set the Y offset in frame mode
  if(testkey(SDLK_UP)&&mode==FRAME&&!play_flag) 
    if(!(SDL_GetModState()&KMOD_SHIFT))
      {set_frame_delay(currentframe(),frame[currentframe()].delay()+1);}
    else set_frame_gapy(currentframe(),frame[currentframe()].gapy-1);

  // Decrease the frame delay in frame mode
  // + Shift: set the Y offset in frame mode
  if(testkey(SDLK_DOWN)&&mode==FRAME&&!play_flag) 
    if(!(SDL_GetModState()&KMOD_SHIFT))
      {set_frame_delay(currentframe(),frame[currentframe()].delay()-1);}
    else set_frame_gapy(currentframe(),frame[currentframe()].gapy+1);

  // Start playing
  if(input::has_been_pushed(SDLK_p)) play();
  // Stop playing
  if(input::has_been_pushed(SDLK_o)) stop();
  // Rewind
  if(input::has_been_pushed(SDLK_i)) rewind();

  // Reverse the current image horizontally in image mode
  if(input::has_been_pushed(SDLK_r))
    {
      if(mode==IMAGE)
	{
	  image im;
	  im=*(t_frame[currentimage]);
	  t_frame[currentimage]->reverse_lr(&im);
	}
    }

  // Reverse the current image vertically in image mode
  if(input::has_been_pushed(SDLK_t))
    {
      if(mode==IMAGE)
	{
	  image im;
	  im=*(t_frame[currentimage]);
	  t_frame[currentimage]->reverse_ud(&im);
	}
    }

  // Loads a PNM image in image mode
  // Adds a frame in frame mode
  if(input::has_been_pushed(SDLK_F1))
    {
      if(mode==IMAGE) ed_add_image();
      if((mode==FRAME)&&(!play_flag)) ed_add_frame();
    }

  // Delete an image in image mode
  // Delete a frame in frame mode
  if(input::has_been_pushed(SDLK_F2))
    {
      if(mode==IMAGE) delete_image(currentimage);
      if((mode==FRAME)&&(!play_flag)) delete_frame(currentframe());
    }

  // Loads an image in game format in image mode
  if(input::has_been_pushed(SDLK_F3))
    {
      if(mode==IMAGE) ed_add_raw_image();
      if((mode==FRAME)&&(!play_flag)) ed_add_frame();
    }

  // Save the current image in PNM format
  if(input::has_been_pushed(SDLK_F9))
    {
      if(mode==IMAGE) save_image();
    }

  // Save the current image in game format
  if(input::has_been_pushed(SDLK_F10))
    {
      if(mode==IMAGE) save_image_raw();
    }
  
  // + Ctrl Copy the current image/frame to the clipboard
  if(input::has_been_pushed(SDLK_c))
    {
      if(SDL_GetModState()&&KMOD_CTRL)
	{
	  if(mode==IMAGE)
	    *clipboard=*(t_frame[currentimage]);
	  if(mode==FRAME)
	    f_clipboard=frame[currentframe()];
	}
    }

  // + Ctrl Paste the clipboard image/frame
  if(input::has_been_pushed(SDLK_v))
    {
      if(SDL_GetModState()&&KMOD_CTRL)
	{
	  if(mode==IMAGE)
	    *(t_frame[currentimage])=*clipboard;
	  if(mode==FRAME)
	    {
	      frame[currentframe()].is_masked_=f_clipboard.is_masked();
	      frame[currentframe()].alpha_=f_clipboard.alpha();
	    }
	}
    }

  // Inserts the clipboard image/frame at the end
  if(input::has_been_pushed(SDLK_b))
    {
      if(SDL_GetModState()&&KMOD_CTRL)
	{
	  if(mode==IMAGE)
	    {
	      image * im=new image;
	      *im=*clipboard;
	      insert_image(im,nbr_of_images());
	    }
	  if(mode==FRAME)
	    {
	      animationframe af;
	      af.is_masked_=f_clipboard.is_masked();
	      af.alpha_=f_clipboard.alpha();
	      insert_frame(af,nbr_of_frames());
	    }
	}
    }

  // Sets next image as frame's image in frame mode
  if(testkey(SDLK_KP_PLUS)&&mode==FRAME&&!play_flag)
    set_frame_imagenbr(currentframe(),
		       increase_image(frame[currentframe()].image_nbr()));
    // Sets previous image as frame's image in frame mode
  if(testkey(SDLK_KP_MINUS)&&mode==FRAME&&!play_flag)
    set_frame_imagenbr(currentframe(),
		       decrease_image(frame[currentframe()].image_nbr()));

  // Increase alpha in frame mode
  if(testkey(SDLK_PAGEUP)&&mode==FRAME&&!play_flag)
    set_frame_alpha(currentframe(),frame[currentframe()].alpha()+1);
  // Decrease alpha in frame mode
  if(testkey(SDLK_PAGEDOWN)&&mode==FRAME&&!play_flag) 
    set_frame_alpha(currentframe(),frame[currentframe()].alpha()-1);

  // Increase next frame in frame mode
  if(testkey(SDLK_HOME)&&mode==FRAME&&!play_flag)
    set_frame_nextframe(currentframe(),
			increase_frame(frame[currentframe()].nextframe()));
  // Decrease next frame in frame mode
  if(testkey(SDLK_END)&&mode==FRAME&&!play_flag) 
    set_frame_nextframe(currentframe(),
			decrease_frame(frame[currentframe()].nextframe()));
  
  // Switch mask on/off in frame mode
  if(testkey(SDLK_INSERT)&&mode==FRAME&&!play_flag)
    set_frame_mask(currentframe(),frame[currentframe()].is_masked()==true?
		   false:true);
}

void animation::update_editor()
{  
  update();
  u_int16 i;
  length_=0;
  height_=0;
  for(i=0;i<nbr_of_frames();i++)
    {
      u_int16 tl,th;
      if((tl=t_frame[frame[i].imagenbr]->length()+frame[i].gapx)>length())
	length_=tl;

      if((th=t_frame[frame[i].imagenbr]->height()+frame[i].gapy)>height())
	height_=th;
    }
  if(info_win_count)
    {
      if(info_win_count==1)
	{
	  info_win->set_border_visible(true);
	  info_win->set_background_visible(true);
	  info_win->set_visible_all(true);
	}
      info_win->update();
      info_win_count++;
      if(info_win_count==200) 
	{
	  info_win->set_border_visible(false);
	  info_win->set_background_visible(false);
	  info_win->set_visible_all(false);
	  info_win_count=0;
	}
    }

  // Update status
  if(must_upt_label_mode) {update_label_mode(); must_upt_label_mode=false;}
  if(must_upt_label_frame_nbr) {update_label_frame_nbr(); 
  must_upt_label_frame_nbr=false;}
  if(must_upt_label_frame_info) {update_label_frame_info(); 
  must_upt_label_frame_info=false;}
  if(must_upt_label_status) {update_label_status(); 
  must_upt_label_status=false;}
}

void animation::draw_editor()
{
  // Redraw a blank screen
  screen::clear();

  // Draw the image to draw
  if(mode==FRAME) draw(0,0);
  else if(nbr_of_images()>0) t_frame[currentimage]->putbox(0,0);

  // Draw the status window
  container->draw();

  // Draw info window
  info_win->draw();
}

void animation::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do();i++) update_editor();
  draw_editor();
}

void animation::set_info_win(char * text)
{
  info_win_label->set_text(text);
  info_win_label->set_auto_size(true);
  info_win->set_align_all(WIN_ALIGN_CENTER);
  info_win_count=1;
}

void animation::editor()
{
  u_int16 i;

  clipboard=new image;

  // Creating the status window
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

  // Creating the info windo
  info_win=new win_container(20,10,280,20,th);
  info_win_label=new win_label(0,5,0,0,th,font);
  info_win->add(info_win_label);
  info_win_count=0;
  info_win->set_border_size(WIN_SIZE_MINI);
  info_win->set_border_visible(false);
  info_win->set_background_visible(false);
  info_win->set_visible_all(false);

  // Defaults settings
  //Mode label (image/frame)
  must_upt_label_mode=false;
  // Image/Frame number
  must_upt_label_frame_nbr=false;
  // Informations (size, etc...)
  must_upt_label_frame_info=false;
  // Status (playing, ...)
  must_upt_label_status=false;  
  set_mode(IMAGE);
  stop();
  rewind();
  in_editor=true;

  // Main loop
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      input::update();
      for(i=0;i<screen::frames_to_do();i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }

  // Clean things up
  input::clear_keys_queue();
  in_editor=false;
  delete container;
  delete info_win;
  delete th;
  delete font;
  delete clipboard;
}

#endif
