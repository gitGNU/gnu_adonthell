/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WIN_SCROLLED_H_
#define _WIN_SCROLLED_H_

class win_base;
class win_theme;
class win_container;

class win_scrolled : public win_container
{
 protected:
  bool visible_scrollbar_;
  u_int16 max_amplitude_;
  u_int16 cur_amplitude_;
  u_int16 index_pad_;
  u_int16 cursory_;
  
  void find_amplitude();
  void update_amplitude();
  void draw_scrollbar();
  virtual void on_up();
  virtual void on_down();

 public:
  //constructor x,y,length,height,and a theme
  win_scrolled(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth);
  ~win_scrolled();
  //you understand up in scrollbar
  bool up();
  
  //down too :)
  bool down();

  //return difference between the last object and the visual height, I think you don't understand, but i know you never use thisfunction just me 
  u_int16 amplitude(){return max_amplitude_;}
  virtual void add(win_base *);
  virtual void remove(win_base *);
  virtual void remove_all();
  void resize(u_int16 tl,u_int16 th);
  void destroy();
  void draw();
  void update();
  void set_space_between_border(u_int16 );
  void set_space_between_object(u_int16 );
  //if you want to show the scroll bar set to true
  
  void set_scrollbar_visible(bool b){visible_scrollbar_=b;}
  bool is_scrollbar_visible(){return visible_scrollbar_;}
};
#endif



