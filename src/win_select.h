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
class win_anim;
class win_label;
class win_write;
class win_container;
class win_border;
class win_select;
class win_cursor;
class image;

class Type_win_select{


 public:
  win_image * img;
  win_anim * anm;
  win_label * lab;
  win_write * wri;
  win_container * con;


  //pointer of his mother win_select
  win_select * wselect; 

  // real border of the object
  // needed if the object have a border before the selection
  win_border * n_border; 
  
  ///needed to identify type of object
  u_int8 id; // 0:lab, 1:wri, 2:img, 3: con
  
  //constructor for each object
  Type_win_select(win_image * p,win_select *,u_int8 m);
  Type_win_select(win_anim * p,win_select *,u_int8 m);
  Type_win_select(win_label * p,win_select *,u_int8 m);
  Type_win_select(win_write * p,win_select *,u_int8 m);
  Type_win_select(win_container * p,win_select *,u_int8 m);
  
  ~Type_win_select();

  //activate the object
  void activate();
  
  //return a pointer of this object
  void * get();

  //give the selected mode of this object
  void select();

  //give the unselected mode of this object
  void unselect();
  
  s_int16 get_y_move(win_container *);

  void y_move(s_int16);
};


class win_select
{

 public:
  //****************************************
  //**************** PUBLIC  ***************
  //****************************************
  win_container * wc;
  //CONSTRUCTOR
  win_select(win_container *);

   //DESTRUCTOR
  ~win_select();

  //list of all object in this selection
  list<Type_win_select> l_list;

  //pointer on the l_list
  list<Type_win_select>::iterator ite_list;
  
  
  //border used to select an object
  win_border * cur_border; 
  //cursor used to select an object
  win_cursor * cursor;
  
  //Method used to add object
  void add(win_label *,u_int8 m=WIN_SELECT_MODE_BRIGHTNESS);
  void add(win_write *,u_int8 m=WIN_SELECT_MODE_BRIGHTNESS);
  void add(win_image *,u_int8 m=WIN_SELECT_MODE_BRIGHTNESS);
  void add(win_anim *,u_int8 m=WIN_SELECT_MODE_BRIGHTNESS);
  void add(win_container *,u_int8 m=WIN_SELECT_MODE_BORDER);
  
  //Method used to remove object
  void remove(win_label *);
  void remove(win_write *);
  void remove(win_image *);
  void remove(win_anim *);
  void remove(win_container*);

  //change position to next object and return this pointer 
  void * next();
  
  //change position to previous object and return this pointer 
  void * previous();
  
  //get pointer of the cur object 
  void * get();

  //get position of the cur object start at 1 return zero if no objet
  u_int16 get_pos();
  

  //set the default object
  void set_default_obj(void * tmp);

  //set border to the selection, You need to set a border if you have insert object with WIN_SELECT_MODE_BORDER
  void set_border(win_border *);
  
  void set_cursor(win_cursor *);

  //activate the cur object
  void activate_select();
  
  void adjust_visible();

};
#endif







