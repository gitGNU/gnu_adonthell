/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WIN_CONTAINER_H_
#define _WIN_CONTAINER_H_

class win_base;
class win_theme;

class win_container : public win_base
{
 protected:
  list<win_base *> list_obj;
  u_int16 space_between_border_;
  u_int16 space_between_object_;
  u_int8 justify_;
  u_int8 layout_;
  void update_layout();

 public:
  win_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth);
  ~win_container();

  //add an object
  virtual void add(win_base *);

  //remove an object
  virtual void remove(win_base *);

  //remove all, but not in memory
  virtual void remove_all();

  //destroy all object of the list and in memory
  virtual void destroy();

  //update function(not optimized actually)
  virtual void update();

  //draw on the screen
  virtual void draw();
  
  void move(s_int16 tx,s_int16 ty);

  //set the space between object and the border, work if you use layout or justify
  virtual void set_space_between_border(u_int16);
  
  //set the space between object and the border, work if you use layout
  virtual void set_space_between_object(u_int16);

  u_int16 get_space_between_border(){return space_between_border_;}
  u_int16 get_space_between_object(){return space_between_object_;}

  //if true all of this object is in brightness mode
  void set_draw_brightness(bool b);

  void set_visible_all(bool);
  
  //justify an win_base object in this object
  void set_justify(win_base * wb,u_int8);
  
  //justify all object : WIN_JUSTIFY_LEFT, WIN_JUSTIFY_RIGHT, WIN_JUSTIFY_CENTER
  void set_justify(u_int8);

  //set the layout (like in java i think) Now 2 sort of layout but i can add several if you suggest me. WIN_LAYOUT_NO (no layout: you put your object where you want)
  //and WIN_LAYOUT_LIST( all object show like a listbox)
  void set_layout(u_int8 lay);

  //IMPORTANT: You can use set_justify and layout to do good window

  void update_real_position();
};
#endif








