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

#ifndef WIN_BASE_H_
#define WIN_BASE_H_

class win_container;
class drawing_area;
class image;
class win_border;
class win_background;
class win_select;

class win_base
{
 private:
  void init_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container *,drawing_area *);
  

 
  image * corner;
  image * h_border;
  image * v_border;
  image * background;
  
  //contain image to build border
  win_border * wborder;
  
  //contain image to build background
  win_background * wback;
  
  //transparency of background
  u_int8 level_trans_back;
  

  void (*act_function)(void*,void*);
  void * param_func1;
  void * param_func2;
  void execute_activate_function();
  
 protected: 

  //pointer of win_select, needed because each object depend of a win_container
   win_container * wc;

   //drawing area where the object can be visible
   drawing_area * da;
   
   //position and size of this object
   u_int16 x;
   u_int16 y;
   u_int16 length;
   u_int16 height;
   

   bool selected;
   bool visible;
  

   void draw_background();
   
   void draw_border();
   //resize border: Don't use it
   void resize_border();
   //draw border: Don't use it
   void resize_background();

   

 
 public:
    win_base(u_int16,u_int16,u_int16,u_int16,win_container * twc=NULL ,drawing_area * tda=NULL);
   ~win_base();
   
   //pointer to a win_select
   win_select * wselect;

   //mode of the selection
   u_int8 select_mode;

   //postion of x,y in the screen, not the position of the object in a container
   u_int16 real_x;
   u_int16 real_y;
     
   
   //select && visible
   void select();
   void unselect();
   void show();
   void hide();

   //update postion and etc.... : don't use it
   void update_da();
 

   //activate the object:: don't use it
   void activate();
 
   //attach a function which is executing when the object is activate
   //this function must have 2 parameter void
   void attach_activate_function(void (*f)(void*,void*),void*,void*);
   
   //detach a function
   void detach_activate_function();

   //position && size
   u_int16 get_x();
   u_int16 get_y();
   u_int16 get_length();
   u_int16 get_height();
   void resize(u_int16,u_int16);
   void move(u_int16,u_int16);
   
   //return pointer drawing_area
   drawing_area * get_drawing_area();
   
  //return norder of this object
   win_border * get_border(); 
   
   //set border of this object, if paramater is NULL, no border is drawing
   void set_border(win_border *);
  
    //set background of this object, if paramater is NULL, no background is drawing
   void set_background(win_background * );
  
 
};
#endif




















