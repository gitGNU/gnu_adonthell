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

#include "win_event.h"
#include "win_border.h"
#include "win_background.h"
#include "win_keys.h"

class win_container;
class win_scroll;
class win_select;
class win_manager;

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

  void set_manager (win_manager*);
    
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
  
  win_manager * manager_;
};



#endif















