/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WIN_BASE_H_
#define _WIN_BASE_H_

class win_theme;
class win_draw;
class win_select;

#include "callback.h"
#include "drawing_area.h"

class win_base
{
 private:
  int level_trans_back_;

 protected:
  s_int16 x_,realx_;
  s_int16 y_,realy_;
  u_int16 length_;
  u_int16 height_;
  s_int16 padx_;
  s_int16 pady_;

  Functor0 callback_[20];

  drawing_area * da_;
  drawing_area * pda_;
  
  bool visible_;
  bool in_select_;
  bool selected_;
  bool activated_;
  bool visible_background_;
  bool visible_border_;
  bool draw_brightness_;

  win_theme * theme_;
  win_base * wb_father_; 


  u_int8 mode_select_;
  u_int8 border_size_;
  u_int8 level_brightness_;
  u_int8 type_obj_;
  void update_drawing_area_position();
  void draw_border();
  void draw_background();
  void assign_drawing_area(){if(wb_father_) da_->assign_drawing_area(wb_father_->get_drawing_area());}
  void detach_drawing_area(){da_->detach_drawing_area();}
  
  void set_in_select(bool b){in_select_=b;}
  void set_select(bool b){if(b) on_select(); else on_unselect();}
  void set_select_mode_(u_int8 mode) {mode_select_=mode;set_draw_brightness(mode==WIN_SELECT_MODE_BRIGHTNESS);}
  win_base(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth);
  

  //execute the callback function
  virtual void on_activate();
  virtual void on_unactivate();
  virtual void on_update();
  virtual void on_draw();
  virtual void on_activate_key();
  virtual void on_select();
  virtual void on_unselect();

 public:
  //constructor, parameter x,y,length,height, and a win_theme
  
  virtual ~win_base();
  
  //POSITION AND SIZE
  s_int16 x(){return(x_);}
  s_int16 y(){return(y_);}
  u_int16 length(){return(length_);}
  u_int16 height(){return(height_);}
  s_int16 & padx(){return(padx_);}
  s_int16 & pady(){return(pady_);}
  s_int16 realx(){return(realx_);}
  s_int16 realy(){return(realy_);}
  virtual void move(s_int16 tx,s_int16 ty);
  virtual void resize(u_int16 tl,u_int16 th);
  void set_border_size(u_int8 siz){border_size_=siz;}//two choice: WIN_SIZE_MINI , WIN_SIZE_NORMAL
  
  //VISIBLE
  void set_visible(bool b){visible_=b;}
  bool is_visible(){return visible_;}
  void set_background_visible(bool b){visible_background_=b;}
  bool is_background_visible(){return visible_background_;}
  void set_border_visible(bool b){visible_border_=b;}
  bool is_border_visible(){return visible_border_;}
  
  //Use this function to set a callback function
  void set_signal_connect(const Functor0 &func,u_int8 signal);
  
  //if true you can use function of this object like up and down to win_scrolled or write to win_write ....
  virtual void set_activated(bool b){if(b) on_activate();else on_unactivate();}
  
  //return true if activated
  bool is_activated(){return activated_;}
  
  //return true if this object is in win_select;
  bool is_in_select(){return in_select_;}
  
  //return if selected
  bool is_select(){return selected_;}
  
  //return type of object 
  u_int8 type_obj(){return type_obj_;}
  
  //if true the object draw with brightness 
  virtual void set_draw_brightness(bool b){draw_brightness_=b;}

  //level of transparency
  void set_level_brightness(u_int16 t){level_brightness_=t;}
  void set_level_trans_background(u_int16 t){level_trans_back_=t;}
  
  //put the name of the theme
  void set_theme(win_theme * th);
  
  //return the drawing area of this object
  drawing_area * get_drawing_area(){return da_;}
  
  //update the object, now not optimized i must change some thing
  virtual void update();
  virtual void update_real_position();
  
  //draw the object
  virtual void draw();
  
  friend class win_container;
  friend class win_select;
};
#endif






