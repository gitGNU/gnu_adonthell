/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _WIN_SELECT_H_
#define _WIN_SELECT_H_

#include "win_scroll.h"

class win_select : public win_scroll
{
 public:
  
  win_select();

  bool input_update();
  
  void add(win_base * w);
  
  void remove(win_base * w);

  void remove_all();
  
  void set_mode(const u_int8 mode){mode_ = mode;}
  
  void set_border_select(win_border * border){border_select_ = border;}
  
  void set_circle(const bool  b) {circle_ = b;}
  bool is_circle(){return circle_;}

  void set_default();

  void set_default_object(const win_base * wb);

  void set_default_position(const u_int16 pos);

  void set_pos( const u_int8 pos) {win_scroll::set_pos(pos);finish_scroll_ = true;}

  bool update();

  win_base * get_selected_object();
  
  u_int16 get_selected_position();

  static const u_int8 MODE_BORDER = 0;
  
  static const u_int8 MODE_BRIGHTNESS = 1;




  
 protected:
  
  void rules(const bool b, win_base * wb);

  void update_cur_select_position();

  void next();
  
  void previous();

  void activate();

  u_int8 mode_;
  
  bool circle_;

  bool finish_scroll_;

  win_border * border_select_;

  lwb::iterator cur_select_;
};



/*
class win_base;
class win_theme;
class win_container;
class win_scrolled;

class win_select : public win_scrolled
{
 protected:
#ifndef SWIG
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
#endif

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
  
  IMPORTANT
    you can with win_select browse a tree of selection, if you want to return at the last node call the back function, or if you want
    to go at a node you have selected just call the on_activate_function
   
  static s_int32 next_key;
  static s_int32 previous_key;
  static s_int32 activate_key;
  static s_int32 back_key;
};

*/
#endif





