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
class win_cursor;

class win_base
{

 private:
  void init_base(s_int16 tx,s_int16 ty, u_int16 tl,u_int16 th,win_container *,drawing_area *);
  
  s_int16 save_x;
  s_int16 save_y;
#ifdef _DEBUG_
  static u_int16 cpt_win_obj_debug; 
#endif
  
  
  image * corner_t_l;
  image * corner_t_r;
  image * corner_b_l;
  image * corner_b_r;
  image * h_border;
  image * v_border;
  
  image * select_corner_t_l;
  image * select_corner_t_r;
  image * select_corner_b_l;
  image * select_corner_b_r;
  image * select_h_border;
  image * select_v_border;
 
  
  image * background;
 
  //contain image to build border
  win_border * wborder;
  
  win_border * wborder_select;

  //contain image to build background
  win_background * wback;
  
  //transparency of background
  u_int8 level_trans_back;
  
  //function to activate by selection
  void (*act_function)(void*,void*);
  void * param_func1;
  void * param_func2;
  void execute_activate_function();
  
  void destroy_border();
  void destroy_border_select();
  
  

 protected: 
  //pointer of win_select, needed because each object depend of a win_container
  win_container * wc;
  
  //drawing area where the object can be visible
  drawing_area * da;
  
  //pointer to a win_select, if this object is in a win_select wselect!=NULL
  win_select * wselect;
  
  bool selected;
  bool visible;
  
  void draw_background();
  void draw_border();
  void resize_border();
  void resize_background();
  
  u_int8 select_mode;

  u_int16 x_pad_l; //x start for the text
  image * cursor;

 public:
   
  
   /**************************************************************************/
   /****************************YOU CAN USE THIS *****************************/
   /**************************************************************************/
 
   win_base(s_int16,s_int16,u_int16,u_int16,win_container * twc=NULL ,drawing_area * tda=NULL);
   virtual ~win_base();

   //visible
   void show();
   void hide();
  
   //attach a function which is executing when the object is activate
   //this function must have 2 parameter void
   void attach_activate_function(void (*f)(void*,void*),void*,void*);
   
   //detach a function
   void detach_activate_function();

   //position && size
   s_int16 get_x();
   s_int16 get_y();
   u_int16 get_length();
   u_int16 get_height();
   void resize(u_int16,u_int16);
   virtual void move(s_int16,s_int16,bool move_by_scrollbar=false);
   
   //select mode
   void set_select_mode(u_int8);
   u_int8 get_select_mode();
   
   //return pointer drawing_area
   drawing_area * get_drawing_area();
   
   //return norder of this object
   win_border * get_border(); 
   
   //set border of this object, if paramater is NULL, no border is drawing
   void set_border(win_border * );
   void set_border_select(win_border * twb);
     
   //set cursor WORK ONLY WITH LABEL AND WRITE
   void set_cursor (win_cursor * tmp);
   
   //set background of this object, if paramater is NULL, no background is drawing
   void set_background(win_background * );


   
   /**************************************************************************/
   /****************************NEVER USE THIS *******************************/
   /**************************************************************************/
   
   
   //postion of x,y in the screen, not the position of the object in a container
   s_int16 real_x; //position on the screen
   s_int16 real_y; //position on the screen
   
   //x,y position in the container
   s_int16 x; //position on a container
   s_int16 y; //position on a container
   u_int16 length; //size visible on the screen
   u_int16 height; //size visible on the screen
   
   //select && visible
   void select();
   void unselect();
   
   //update postion and etc.... 
   virtual void update_da();
   virtual void draw()=0;
   virtual void update()=0;

   //activate the object
   void activate();

   void save_position();
   void reload_position();

   void attach_select(win_select *,u_int8 tmode); //this function is called by win_select
   void dettach_select();

};
#endif




















