/*
   $Id$

   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file win_base.h
 * @author Vennin Joël <jol@linuxgames.com>
 * 
 * brief Declares the win_bases class. 
 *
 *
 */


#ifndef _WIN_BASE_H_
#define _WIN_BASE_H_

#include"types.h"
#include "input.h"
#include "win_event.h"
#include "win_border.h"
#include "win_background.h"
#include "win_keys.h"
#include "drawing_area.h"

class win_container;
class win_scroll;
class win_select;


/**
 * Common properties for each win_base's object
 *
 * @bug It's currently impossible to render a windowed object
 *      anywhere else than the screen. Windows should be capable
 *      of being assigned a target parameter, which is a pointer
 *      to the surface they should be drawn.
 */ 
class win_base: public win_event, public win_border, public drawing_area, public win_background
{
 public:
  
  /**
   * Default constructor:
   *    - not visible
   *    - x,y equals to 0
   *    - not focus
   *    - not activate
   *    - not brightness
   *    - not transluency
   *    - can be selected
   *    - alignement is ALIGN_NONE
   */
  win_base();
  
  /**
   * Return the relative horizontal position of the win_*.
   * @return horizontal position of the win_*.
   */
  s_int16 x() const 
    {return x_;}

  
  /**
   * Return the relative vertical position of the win_*.
   * @return vertical position of the win_*.
   */
  s_int16 y() const 
    {return y_;}
  

  /**
   * Return the pad horizontal position of the win_*.
   * @return the pad horizontal position of the win_*.
   */
  s_int16 & pad_x() 
    {return pad_x_;}
  
  
  /**
   * Return the pad vertical position of the win_*.
   * @return the pad vertical position of the win_*.
   */
  s_int16 & pad_y() 
    {return pad_y_;}

  
   /**
   * Return the horizontal position of the win_*.
   * @return the horizontal position of the win_*.
   */
  s_int16 real_x() const 
    {return drawing_area::x();}
  
  
  /**
   * Return the vertical position of the win_*.
   * @return the vertical position of the win_*.
   */
  s_int16 real_y() const 
    {return drawing_area::y();}
  

  /** Move the win_*.
   *  @param tx new horizontal position.
   *  @param ty new vertical position.
   */  
  virtual void move(s_int16 tx,s_int16 ty);
  

   /** Rezise the win_*.
    *  @param tl new horizontal position.
    *  @param th new vertical position.
    */  
  virtual void resize(u_int16 tl, u_int16 th);
  
  
  /** Test if win_* is visible
   *  @return true if visible else false 
   */
  bool is_visible() const 
    {return visible_;}
  
  
  /** Set the visible parameter
   * @param b true if the win_* should be visible, false otherwise
   */
  void set_visible(const bool b) 
    {visible_=b;}
  

  /** Test if win_* is activated
   *  @return true if activate else false 
   */
  bool is_activate() const
    {return activate_;}
  
  
  /** Set the activate parameter
   * When a win_* is setup on, the keys queue is cleared
   *
   * @param b true if the win_* should be visible, false otherwise
   */
  void set_activate(const bool b)
    {if(activate_=b) {on_activate();input::clear_keys_queue();}else on_unactivate();}
  
  
  /** Test if win_* has focus on
   *  @return true if focus on else false 
   */
  bool is_focus()const 
    {return focus_;}
  
  
  /** Set the focus parameter
   * @param b true if the win_* should be focus on, false otherwise
   */
  void set_focus(const bool b)
    {focus_=b;}
  
  
  /** Test if win_* has focus on
   *  @return true if focus on else false 
   */
  bool is_trans() const 
    {return trans_;}
  
  
  /** Set the transluency parameter
   * @param b true if the win_* should be ins transluency, false otherwise
   */
  virtual void set_trans(const bool b)
    {set_trans_border(trans_ = b);set_trans_background(b);}

  
  /** Test if win_* is in brightness
   *  @return true if in brightness else false 
   */
  bool is_brightness() const
    {return brightness_;}
  
  
  /** Set the transluency parameter
   * @param b true if the win_* should be in transluency, false otherwise
   */
  virtual void set_brightness(const bool b)
    {set_brightness_border(brightness_ = b);set_brightness_background(b);}

  
  /** Set alignement of win_*
   * @param a can be, ALIGN_NONE, ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
   */
  void set_align(const u_int8 a)
    {align_=a;update_align();}
  
  
  /** Return alignment of win_*
   * @return align_ parameter
   */
  u_int8 align() const {return align_;}
  
  
  /** Test if win_* can be selected
   * @return true if it can be selected, false otherwise
   */
  bool is_can_be_selected() const 
    {return can_be_selected_;}
  
  
  /** Set the object to be selected
   * A win_obj can be selectable or not when it is inside a win_select
   * @param b true if the object can be selected inside a win_select., false otherwise
   */
  void set_can_be_selected(const bool b) 
    {can_be_selected_ = b;}
  

  /** Update process
   *  @return true if update is successful, false otherwise 
   */
  virtual bool update();
  
  
  /** Draw process
   * @return true if draw is successful, false otherwise
   */
  virtual bool draw();
  
  
  /** Input Update process 
   * @ 
   */
  virtual bool input_update();

  virtual ~win_base();
  
  static const u_int8 ALIGN_NONE = 0;
  static const u_int8 ALIGN_LEFT = 1;
  static const u_int8 ALIGN_CENTER = 2;
  static const u_int8 ALIGN_RIGHT = 3;
  
 protected:

  friend class win_container;

  friend class win_scroll;
  
  friend class win_select;

  virtual void update_position();
  
  void update_align();

  void set_container(win_container * wc);
  
  

  s_int16 x_;

  s_int16 y_;
  
  s_int16 pad_x_;
  
  s_int16 pad_y_;

  u_int8 align_;

  bool visible_;
  
  bool focus_;

  bool activate_;
  
  bool brightness_;

  bool trans_;

  bool can_be_selected_;

  win_container * wb_father_;
  
};






/* 
class win_theme;
class win_draw;
class win_select;

#if defined (USE_PYTHON)
class py_callback;

#include "Python.h"
#endif

#include <vector>
#include "callback.h"
#include "drawing_area.h"

class win_base
{
 public:
 */  /*******************************************************************/
  //CONSTRUCTOR
  /*******************************************************************/
  //parameters x,y,length,height, and a win_theme
 /*  
win_base(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth);
 */
  


  /*******************************************************************/
  //POSITION AND SIZE
  /*******************************************************************/
 /*  
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

  //two choice: WIN_SIZE_MINI , WIN_SIZE_NORMAL
  void set_border_size(u_int8 siz) {border_size_=siz;}
 
  //justify object : WIN_ALIGN_LEFT, WIN_ALIGN_RIGHT, WIN_ALIGN_CENTER
  void set_align(u_int8 a){align_=a;update_align();}
  u_int8 align(){return align_;}
  

 */
  /*******************************************************************/
  //VISIBLE
  /*******************************************************************/
 /* 
 void set_visible(bool b){visible_=b;}
  bool is_visible(){return visible_;}

  void set_background_visible(bool b){visible_background_=b;}
  bool is_background_visible(){return visible_background_;}

  void set_border_visible(bool b){visible_border_=b;}
  bool is_border_visible(){return visible_border_;}


 */

  /*******************************************************************/
  //CALLBACK AND EVENT
  /*******************************************************************/
  //Use this function to set a callback function
 
 /* void set_return_code (int rc) { return_code_ = rc; }
#ifndef SWIG
  void set_signal_connect (const Functor0 &func, u_int8 signal);
  void set_callback_destroy (const Functor0wRet<bool> &func)
  {
    callback_destroy_ = func; 
  }
  void set_callback_quit (const Functor1<int> &func)
  {
    callback_quit_=func;
  }
#endif
#if defined (USE_PYTHON)
  void py_signal_connect (PyObject *pyfunc, int signal, PyObject *args = NULL); 
#endif

 */

  /*******************************************************************/
  //SELECT OBJECT
  /*******************************************************************/
 /* 
 bool is_can_be_selected(){return can_be_selected_;}
  void set_can_be_selected(bool b){can_be_selected_=b;}
  
  //return true if this object is in win_select;
  bool is_in_select(){return in_select_;}
  
  //return if selected
  bool is_select(){return selected_;}

 */

  /*******************************************************************/
  //ACTIVATED AND FOCUS
  /*******************************************************************/
 /*  
virtual void set_activated(bool b) {(activated_=b)?on_activate():on_unactivate();}
  bool is_activated(){return activated_;}
  
  bool is_focus(){return focus_;}
  virtual void set_focus(bool b);
 */


  /*******************************************************************/
  //THEME & BACKGROUND & BORDER CONFIG
  /*******************************************************************/
  //if true the object draw with brightness 
 /* 
 virtual void set_draw_brightness(bool b)
    {draw_brightness_=b;}

  //level of transparency
  void set_level_brightness(u_int16 t) {level_brightness_=t;}
  void set_level_trans_background(u_int16 t) {level_trans_back_=t;}
  
  //put the name of the theme
  void set_theme(win_theme * th);
  

 */
  /*******************************************************************/
  //OTHER 
  /*******************************************************************/
  //return the drawing area of this object
 /*
 drawing_area * get_drawing_area(){return da_;}
  
  virtual bool update();
  virtual void update_real_position(); //don't use it
  
  virtual bool draw();
  
  //destructor
  virtual ~win_base();

#ifndef SWIG
  friend class win_container;
  friend class win_select;
#endif // SWIG


 protected:

#if defined (USE_PYTHON)
  // the python callbacks connected to the window
  vector<py_callback *> py_callbacks;   
#endif // USE_PYTHON

  int level_trans_back_; 
  int return_code_;
  
  s_int16 x_,realx_;
  s_int16 y_,realy_;
  u_int16 length_;
  u_int16 height_;
  s_int16 padx_;
  s_int16 pady_;
  

  Functor0 callback_[20];
  Functor0wRet<bool> callback_destroy_;
  Functor1<int> callback_quit_;

  drawing_area * da_;
  drawing_area * pda_;

  
  bool visible_; //if object is visible
  bool in_select_;// if object is in select
  bool selected_; // if object is selected
  bool activated_;// if object is activated
  bool visible_background_;//if background is visible
  bool visible_border_;// if border is visible
  bool draw_brightness_;//if draw object in brightness mode
  bool can_be_selected_;//if object can be selected
  bool focus_; //if object has focus on


  win_theme * theme_; //cur theme of this object
  win_base * wb_father_; //up windows


  u_int8 mode_select_;//mode used for selection
  u_int8 border_size_;//size of border
  u_int8 level_brightness_;//level of brightness
  u_int8 align_;


  
  void update_drawing_area_position();
  void draw_border();
  void draw_background();
  void assign_drawing_area(){if(wb_father_) da_->assign_drawing_area(wb_father_->get_drawing_area());}
  void detach_drawing_area(){da_->detach_drawing_area();}
  
  void set_in_select(bool b){in_select_=b;}
  void set_select(bool b){(selected_=b)?on_select():on_unselect();}
  void set_select_mode_(u_int8 mode); 

  void update_align();
   
  
  //execute the callback function
  virtual void on_activate();
  virtual void on_unactivate();
  virtual void on_update();
  virtual void on_draw_visible();
  virtual void on_draw();
  virtual void on_activate_key();
  virtual void on_select();
  virtual void on_unselect();


 private:
#ifdef _DEBUG_WIN
  static long cptw;
#endif
};

 */

#endif















