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

#ifdef _DEBUG_
u_int16 animation::a_d_diff=0;
#endif

void animation_frame::init()
{
  imagenbr=0;
  is_masked=false;
  alpha=0;
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

void animation_frame::set_nextframe(u_int16 nf)
{
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

#ifdef _EDIT_
s_int8 animation_frame::put(SDL_RWops * file)
{
  SDL_RWwrite(file,&imagenbr,sizeof(imagenbr),1);
  SDL_RWwrite(file,&is_masked,sizeof(is_masked),1);
  SDL_RWwrite(file,&alpha,sizeof(alpha),1);
  SDL_RWwrite(file,&gapx,sizeof(gapx),1);
  SDL_RWwrite(file,&gapy,sizeof(gapy),1);
  SDL_RWwrite(file,&delay,sizeof(delay),1);
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
  t_frame=NULL;
  frame=NULL;
  nbr_of_images=0;
  nbr_of_frames=0;
  currentframe=0;
  speedcounter=0;
  play_flag=false;

#ifdef _EDIT_
  image temp;
  string t;
  mode=IMAGE;
  currentimage=0;
  t=WIN_DIRECTORY;
  t+=WIN_BACKGROUND_DIRECTORY;
  t+=WIN_THEME_ORIGINAL;
  t+=WIN_BACKGROUND_FILE;
  temp.load_raw(t.data());
  font=new win_font(WIN_THEME_ORIGINAL);
  border=new win_border(WIN_THEME_ORIGINAL);
  container=new win_container(200,12,110,216);
  label_mode=container->add_label(5,5,100,30,font);
  label_frame_nbr=container->add_label(5,35,100,30,font);
  label_anim_info=container->add_label(5,65,100,65,font);
  label_frame_info=container->add_label(5,100,100,65,font);
  bg=new image(320,240);
  bg->putbox_tile_img(&temp);
  container->set_border(border);
  container->show_all();
#endif
#ifdef _DEBUG_
  cout << "animation() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

animation::~animation()
{

#ifdef _EDIT_
  delete bg;
  delete label_mode;
  delete label_anim_info;
  delete label_frame_info;
  delete label_frame_nbr;
  delete border;
  delete font;
  // FIXME: Bug in the container destructor!
  //  delete container;
#endif

  delete[] t_frame;
  delete[] frame;
#ifdef _DEBUG_
  cout << "~animation() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
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

#ifdef _EDIT_
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

void animation::save()
{
  char * s=query_window("Save as:");
  if(!s) return;
  if(save(s)) info_window("Error saving!");
  delete s;
}

void animation::info_window(char * t_label)
{
  win_container * querycont;
  win_label * querylabel;
  win_label * querylabel2;
  win_border * queryborder;
  win_font * queryfont;
  win_background * queryback;
  u_int8 previous_mode=input::get_keyboard_mode();

  queryback=new win_background(WIN_THEME_ORIGINAL);
  queryfont=new win_font(WIN_THEME_ORIGINAL);
  queryborder=new win_border(WIN_THEME_ORIGINAL);
  querycont=new win_container(70,40,200,120);
  querylabel=querycont->add_label(5,5,190,130,queryfont);
  querylabel->set_text(t_label);
  querylabel2=querycont->add_label(60,90,80,15,queryfont);
  querylabel2->set_text("Press any key...");
  querycont->set_border(queryborder);
  querycont->set_background(queryback);
  querycont->show_all();
  input::set_keyboard_mode(MODE_PUSHED);
  input::clear_keys_queue();
  do
    {
      input::update();
      update_and_draw();
      querycont->update();
      querycont->draw();
      screen::show();
    } 
  while(input::get_next_key()<0);
  input::set_keyboard_mode(previous_mode);
  delete querycont;
  delete queryfont;
  delete queryback;
  input::clear_keys_queue();
}

char * animation::query_window(char * t_label)
{
  u_int8 previous_mode=input::get_keyboard_mode();
  char * s;
  win_container * querycont;
  win_label * querylabel;
  win_write * querywrite;
  win_border * queryborder;
  win_font * queryfont;
  win_background * queryback;

  queryback=new win_background(WIN_THEME_ORIGINAL);
  queryfont=new win_font(WIN_THEME_ORIGINAL);
  queryborder=new win_border(WIN_THEME_ORIGINAL);
  querycont=new win_container(70,40,200,120);
  querylabel=querycont->add_label(5,5,100,15,queryfont);
  querylabel->set_text(t_label);
  querywrite=querycont->add_write(5,20,100,30,queryfont);
  querycont->set_border(queryborder);
  querycont->set_background(queryback);
  querycont->show_all();  
  input::set_keyboard_mode(MODE_CHAR);
  while(!querywrite->is_text())
    {
      input::update();
      update_and_draw();
      querycont->update();
      querycont->draw();
      screen::show();
      if(input::is_pushed(SDLK_ESCAPE))
	{
	  input::set_keyboard_mode(previous_mode);
	  delete querycont;
	  delete queryfont;
	  delete queryback;
	  s=NULL;
	  return(s);
	}
    }
  input::set_keyboard_mode(previous_mode);
  s=strdup(querywrite->get_text());
  delete querycont;
  delete queryfont;
  delete queryback;
  return(s);
}

void animation::load()
{
  animation * t=new animation;
  char * s;
  s=query_window("Load animation:");
  if(!s)
    {
      delete t;
      return;
    }
  if(t->load(s)) info_window("Error loading!");
  else 
    {
      currentimage=0;
      *(animation*)this=*t;
    }
  delete t;
  delete s;
}


#endif

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

#ifdef _EDIT_
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
#ifdef _DEBUG_
  cout << "Added frame: " << nbr_of_frames << " total in animation.\n";
#endif
  return 0;
}

void animation::add_image()
{
  image im;
  u_int16 p;
  char * s=query_window("File to load:");
  if(!s) return;
  if(!im.load_raw(s)) 
    {
      do
	{
	  char tmp[255];
	  char * s2;
	  sprintf(tmp,"Insert at pos(0-%d):",nbr_of_images);
	  s2=query_window(tmp);
	  if(!s2)
	    {
	      delete s;
	      return;
	    }
	  p=atoi(s2);
	  delete s2;
	}
      while(p>nbr_of_images);
      insert_image(im,p);
    }
  else info_window("Error loading!");
  delete s;
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
    if(frame[i].imagenbr>pos) frame[i].imagenbr--;
  delete[] oldt_frame;
  if(currentimage>=nbr_of_images) currentimage=nbr_of_images-1;
#ifdef _DEBUG_
  cout << "Removed image: " << nbr_of_images << " total in animation.\n";
#endif
  if(!nbr_of_images) 
    {
      t_frame=NULL;
      currentimage=0;
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
    if(frame[i].nextframe>=pos) frame[i].nextframe++; 
  delete[] oldframe;
  if(currentframe>=nbr_of_frames) currentframe=nbr_of_frames-1;
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
  if(!nbr_of_images) info_window("You must add at least one image before adding a frame!");
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
  return ((input::is_pushed(k)));//&&(SDL_GetModState()&&KMOD_LCTRL)));
}

void animation::update_editor()
{  
  update();
  container->update();
}

void animation::update_editor_keys()
{
  if((SDL_GetModState()&&KMOD_LCTRL)) input::set_keyboard_mode(MODE_STATE);
  else input::set_keyboard_mode(MODE_PUSHED);

  // Mode switching
  if(input::is_pushed(SDLK_F1)) mode=IMAGE;
  if(input::is_pushed(SDLK_F2)) mode=FRAME;

  // General functions
  if(input::is_pushed(SDLK_F5)) save();
  if(input::is_pushed(SDLK_F6)) load();

  // Image mode functions
  if(mode==IMAGE)
    {
      if(testkey(SDLK_RIGHT)) 
	currentimage=increase_image(currentimage);
      if(testkey(SDLK_LEFT)) 
	currentimage=decrease_image(currentimage);

      if(input::is_pushed(SDLK_a)) add_image();
      if(input::is_pushed(SDLK_d)) delete_image(currentimage);
    }

  // Frame mode functions
  else
    {
      if(input::is_pushed(SDLK_p)) play();
      if(input::is_pushed(SDLK_o)) stop();
      if(input::is_pushed(SDLK_i)) rewind();

      if(!play_flag)
	{
	  if(input::is_pushed(SDLK_a)) add_frame();
	  if(input::is_pushed(SDLK_d)) delete_frame(currentframe);
	  if(testkey(SDLK_KP_PLUS))
	    frame[currentframe].imagenbr=
	      increase_image(frame[currentframe].imagenbr);
	  if(testkey(SDLK_KP_MINUS)) 
	    frame[currentframe].imagenbr=
	      decrease_image(frame[currentframe].imagenbr);
	  
	  if(testkey(SDLK_RIGHT)) 
	    currentframe=increase_frame(currentframe);
	  if(testkey(SDLK_LEFT)) 
	    currentframe=decrease_frame(currentframe);
	  
	  if(testkey(SDLK_UP))
	    frame[currentframe].delay++;
	  if(testkey(SDLK_DOWN)) 
	    frame[currentframe].delay--;
	  
	  if(testkey(SDLK_PAGEUP)) frame[currentframe].alpha++;
	  if(testkey(SDLK_PAGEDOWN)) frame[currentframe].alpha--;
	  
	  if(testkey(SDLK_HOME))
	    frame[currentframe].nextframe=
	      increase_frame(frame[currentframe].nextframe);
	  if(testkey(SDLK_END)) 
	    frame[currentframe].nextframe=
	      decrease_frame(frame[currentframe].nextframe);
	  
	  if(testkey(SDLK_INSERT)) 
	    frame[currentframe].is_masked=frame[currentframe].is_masked==true?
	      false:true;
	}
    }
}

void animation::draw_editor()
{
  static char frame_txt[500];
  bg->draw(0,0);
  sprintf(frame_txt,"%s mode",mode==IMAGE?"Image":"Frame");
  label_mode->set_text(frame_txt);
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

  if(mode==FRAME)
    {
      if(nbr_of_frames>0)
	{
	  char s_delay[6];
	  sprintf(s_delay,"%d",frame[currentframe].delay);
	  draw(0,0);
	  sprintf(frame_txt,"Frame info :\nImage number: %d\nLength: %d\n"
		  "Height: %d\nDelay: %s\n"
		  "Next Frame: %d\n\nMasked: %s\nAlpha: %d",
		  frame[currentframe].imagenbr,
		  t_frame[frame[currentframe].imagenbr].length,
		  t_frame[frame[currentframe].imagenbr].height,
		  frame[currentframe].delay==0?"Infinite":s_delay,
		  frame[currentframe].nextframe,
		  frame[currentframe].get_mask()?"Yes":"No",
		  frame[currentframe].get_alpha());
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
	  t_frame[currentimage].putbox(0,0);
	  sprintf(frame_txt,"Image info:\nLength: %d\nHeight:%d",
		  t_frame[currentimage].length,t_frame[currentimage].height);
	  label_frame_info->set_text(frame_txt);
	}
      else
	{
	  sprintf(frame_txt,"No image, press \"a\" to add one.");
	  label_frame_info->set_text(frame_txt);
	}
    }

  if(mode==FRAME)
    {
      sprintf(frame_txt,"%s",play_flag?"Playing":"Stopped");
      label_anim_info->set_text(frame_txt);
    }
  else label_anim_info->set_text("");
  container->draw();
}

void animation::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do;i++) update_editor();
  draw_editor();
}

void animation::editor()
{
  u_int8 previous_mode=input::get_keyboard_mode();
  while(!input::is_pushed(SDLK_ESCAPE))
    {
      input::update();
      update_editor_keys();
      update_and_draw();
      screen::show();
    }
  input::set_keyboard_mode(previous_mode);
}

#endif
