/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WIN_SELECT_H_
#define _WIN_SELECT_H_

#include "win_scrolled.h"

class win_base;
class win_theme;
class win_container;
class win_scrolled;

class win_select : public win_scrolled
{
 protected:
  //index of the current object which is selected
  list<win_base *>::iterator index_list;
  //to show the selected object if isn't visible in the select 
  void update_position();
  //mode to select
  u_int8 mode_selected_;
  
  //type of the select method
  u_int8 type_selected_;
  

  //if this select has a father
  win_select * fatherselect_;
 
  bool select_circle_;

 //current win_select activate
  static win_select * curselect_;
  void next_();
  void previous_();
 
  static bool activate_keyboard_;

  virtual void on_next();
  virtual void on_previous();
  
  //use this function to activate the object see win_base.h
  void on_activate();
  
  bool activate___();
  
  
  void set_select_object(win_base*,bool b);
  
 public:
  win_select(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth);
  ~win_select();
  //Add object
  void add(win_base *);
  void add(win_select *);
  //remove
  void remove(win_base *);
  void remove_all();
  void destroy();
  bool update();
  //get the pointer of the object which is selected
  win_base * get();
  //get the position of the object which is selected
  u_int16 get_pos();
  //set the default object
  void set_default(win_base * wb);
  void set_default(u_int16 ); //set the default object with a number, 1 is the first object 2 is the .....
  
  void set_default(); //set the first object which can be selected
  
  void set_can_be_selected_all(bool);//set to b all the element which are in the select

 
  void set_type(u_int8); //set type of the select, WIN_SELECT_TYPE_NORMAL,WIN_SELECT_TYPE_SCROLL, default is normal
  u_int8 type() {return type_selected_;}


  //set the mode of the selection WIN_SELECT_MODE_BRIGHTNESS, WIN_SELECT_MODE_BORDER, cursor not implemented
  void set_select_mode(u_int8);
 
  void set_select_circle(bool b){select_circle_=b;}
  bool is_select_circle(){return select_circle_;}

  //next object
  static void next();
  //previous object
  static void previous();
  //set the curselect
  static void set_cur_select(win_select * ws);
  //return a the father select
  static bool back();
  //activate the object
  static void activate();

  static void set_activate_keyboard(bool b){activate_keyboard_=b;}
  static bool is_activate_keyboard(){return activate_keyboard_;}
  
  //set the curselect to NULL, needed if you want clean select
  static void init();
  
  /*IMPORTANT
    you can with win_select browse a tree of selection, if you want to return at the last node call the back function, or if you want
    to go at a node you have selected just call the on_activate_function
  */  
  
  static SDLKey next_key;
  static SDLKey previous_key;
  static SDLKey activate_key;
  static SDLKey back_key;
};
#endif





