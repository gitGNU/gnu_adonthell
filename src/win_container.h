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
class win_write;
class win_label;
class win_font;
class win_border;
class win_image;
class drawing_area;
class win_background;

class win_container : public win_base
{
 
 private:
  list<win_label *> l_label; //label list
  list<win_write *> l_write; //write list
  list<win_container *> l_container; //container list
  list<win_image *> l_image; //image list
  
 public:
  //just for the first container
  win_container(u_int16,u_int16,u_int16,u_int16);
  
  //you doesn't use this constructor !!!!!
  win_container(u_int16,u_int16,u_int16,u_int16,win_container *);
  //destructor
  ~win_container();
  
  //it's to create label & write (x,y,length,height, type of font you want to use
  win_label * add_label(u_int16,u_int16,u_int16,u_int16,win_font *);
  win_write * add_write(u_int16,u_int16,u_int16,u_int16,win_font *);
  
  //next you create the first container, if you want to create another container you must use this function
  win_container * add_container(u_int16,u_int16,u_int16,u_int16);
  
  //it's to create an win_image (x,y, and pointer to image)
  win_image * add_image(u_int16,u_int16,image *);

  //used to remove object like win_label,win_write,win_container,win_image
  void remove(win_label *);
  void remove(win_write *);
  void remove(win_container *);
  void remove(win_image *);
  
  //used to draw all object of this container
  void draw();
  
  //update all object of this container
  void update();

  //don't use it
  void update_da();
  
  //move, resize this container and all object of this container
  void move(u_int16,u_int16);
  void resize(u_int16,u_int16);
  
  //show all object of this container (this container too)
  void show_all();

  //show all object of this container (this container too)
  void hide_all();
};

#endif










