/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_SELECT_H_
#define WIN_SELECT_H_

#include "win_types.h"
#include <list>

class win_image;
class win_label;
class win_write;
class win_container;
class win_border;
class win_select;
class win_cursor;
class image;

class win_select
{
 private:

#ifdef _DEBUG_
  static u_int16 cpt_win_select_debug;
#endif
  
  list<win_base *> l_list; 
  list<win_base *>::iterator ite_list;
  list<win_container *> l_select;
  list<win_container *>::iterator ite_select;
  
  
  bool type_select;
  win_border * border_to_select;
  win_cursor * cursor_to_select;
  s_int16 get_y_move_to_be_visible(); 
  static win_select * cur_select;
  void adjust_visible();

 public:
  //call this constructor to create the first win_select
  win_select(win_container * tmp,bool b=WIN_SELECT_JUST_OBJECT);
  
  ~win_select();
  
  //add an object in the win_select, so this win select is like a sheet of tree
  void add(win_base * , u_int8 m);
  //remove an object in the win_select but not destroy object !!!
  void remove(win_base *);
  //remove all object
  void remove_all();
  //remove all select and destroy them
  void remove_all_select();
  //set border && cursor for the selection
  void set_border(win_border * tmp);
  void set_cursor(win_cursor * tmp);
  
  //pass to the next object
  win_base * next();
  
  //pass to the previous object
  win_base * previous();
  
  //add a selection, the twc is the container which 'll be attach to the select
  win_select *  add_select(win_container * twc,bool b=WIN_SELECT_JUST_OBJECT);
  
  //remove a selection but don't destroy
  void remove_select(win_select * tmp); 
  
 
  // call this function to change for another win_select
  void next_select();
  // call this function to return up of the cur winselect (like a tree)
  void up_select();  
  // call this function when you want activate function of the cur object
  void activate_select(); 
  //set the default object 
  void set_default_obj(void * tmp); 
  
  //get pointer of cur selection
  win_base * get(); 
  //get position of cur selection
  u_int8 get_pos(); 
 
  static SDLKey next_key;
  static SDLKey previous_key;
  static SDLKey next_select_key;
  static SDLKey up_select_key;
  static SDLKey activate_key;
  static bool activate_selection;

  //update function
  static void update(); 


  /*****************************************************************************/
  /******************NEVER USE THIS *******************************************/
 /*****************************************************************************/
  win_select(win_container * twc,win_select *tws,bool b=WIN_SELECT_JUST_OBJECT);
  win_select * father_select;
  win_container * attached_container; //container which his attached at the select
   
};



#endif







