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

#ifndef WIN_CONTAINER_H_
#define WIN_CONTAINER_H_

#include "win_base.h"
#include <list>

class image;
class animation;
class win_write;
class win_label;
class win_font;
class win_border;
class win_image;
class win_anim;
class drawing_area;
class win_background;
class win_scrollbar;
class win_select;

class win_container : public win_base
{
 
 private:
  list<win_label *> l_label; //label list
  list<win_write *> l_write; //write list
  list<win_container *> l_container; //container list
  list<win_image *> l_image; //image list
  list<win_anim *> l_anim; //animation list
  
  // DATA TO SCROLLBAR
  s_int16 cur_pos_scrollbar;
  s_int16 max_y_object;
  s_int16 cur_pos_index_scrollbar;
  image * scroll_index;
  image * scroll_bottom;
  image * scroll_middle;
  image * scroll_top;
  image * scroll_bar_draw;
  win_scrollbar * sb;
  u_int16 pad_scroll;

  void init_scrollbar();
  void destroy_scrollbar();
  void draw_scrollbar();
  void update_scrollbar();
  void update_scrollbar_index();
  

  
 public:
  
  /**************************************************************************/
   /****************************YOU CAN USE THIS *****************************/
   /**************************************************************************/

  //just for the first container
  win_container(s_int16,s_int16,u_int16,u_int16);
  
  //destructor
  ~win_container();
  
  //it's to create label & write (x,y,length,height, type of font you want to use
  win_label * add_label(s_int16,s_int16,u_int16,u_int16,win_font *);
  win_write * add_write(s_int16,s_int16,u_int16,u_int16,win_font *);
  
  //next you create the first container, if you want to create another container you must use this function
  win_container * add_container(s_int16,s_int16,u_int16,u_int16);
  
  //it's to create an win_image (x,y, and pointer to image)
  win_image * add_image(s_int16,s_int16,image *);

  //it's to create an win_anim (x,y, and pointer to animation)
  win_anim * add_anim(s_int16,s_int16,animation *);
  
  //used to remove object like win_label,win_write,win_container,win_image
  void remove(win_label *);
  void remove(win_write *);
  void remove(win_container *);
  void remove(win_image *);
  void remove(win_anim *);
  
  //used to draw all object of this container
  void draw();
  
  //update all object of this container
  void update();

  //move, resize this container and all object of this container
  void move(s_int16,s_int16,bool move_by_scrollbar=false);
  void resize(u_int16,u_int16);
  
  //show all object of this container (this container too)
  void show_all();

  //show all object of this container (this container too)
  void hide_all();

  void set_scrollbar(win_scrollbar * tmpsb);//if tmpsb==NULL then no scrollbar :)
  void set_pad_scrollbar(u_int16); //change the pad to move 
  void up_scrollbar();//use it to move up scrollbar
  void down_scrollbar(); //use it to move down scrollbar
  
  // void assign_to_select();//if you use this you must set a scrollbar if not scrollbar select is the same 
  // void desasign_to_select();//detach 

 
  /**************************************************************************/
  /****************************NEVER USE THIS *****************************/
  /**************************************************************************/
  void is_object_max_y(s_int16); //if the object have the Max y 
  void find_obj_max_y(); //search the object which have the max y
  void attach_select(win_select *); //this function is called by win_select
  void dettach_select(); //this function is called by win_select
  //don't use it
  void update_da(); //this function update some position about the drawing area
  //you doesn't use this constructor !!!!!it's called by a creation of new container
  win_container(s_int16,s_int16,u_int16,u_int16,win_container *);
};

#endif
