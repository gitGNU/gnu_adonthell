/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "mouse_event.h"

#include "atk_border_ui.h"
#include "atk_window.h"


atk_window::atk_window () :  alive_ (true) 
{
    // modal by default
    set_modal (true);

    //sensible by default
    set_sensible (true);

    set_resizeable (true);

    set_moveable (true);

    set_deleted (true); 

    /* mouse action none at the beginning */
    mouse_action_ = ACTION_NONE; 
}


void atk_window::set_title (const std::string & t)
{
    title_ = t; 
}


void atk_window::set_type (const u_int8 t)
{
    type_ = t; 
}


bool atk_window::is_modal () const
{
    return modal_; 
}


void atk_window::set_modal (const bool m)
{
    modal_ = m; 
}



int atk_window::input_update (input_event * ev)
{
  if (!is_sensible () ) return 0; 
  
    if (atk_bin::input_update (ev) == 1) return 1;

    mouse_event * me = (mouse_event *) ev; 
    
    /* we check if there is already a mouse actio in progress */
    if (mouse_action_ == ACTION_MOVE)
      {
	if (me->type () == mouse_event::MOUSE_MOTION)
	  {
	    set_position (me->x ()  - mouse_range_x_, me->y ()  - mouse_range_y_);
	    return 1; 
	  }
	else if (me->type () == mouse_event::BUTTON_RELEASED)
	  {
	    mouse_action_ = ACTION_NONE; 
	    return 1; 
	  } 
      }
    
    
    if (mouse_action_ == ACTION_RESIZE)
      {
        if (me->type () == mouse_event::MOUSE_MOTION)
	  {
	    set_size (me->x ()  - get_x_real ()  , me->y ()  - get_y_real () );
	    return 1; 
	  }
        else if (me->type () == mouse_event::BUTTON_RELEASED)
	  {
	    mouse_action_ = ACTION_NONE; 
            return 1; 
	  } 
      }
    
    
    if (point_belong (me->x (), me->y ())) return 0; 
    else
      {
        /* there are no action in progress we check if the cursor is on special border */
        if (deleted_)
	  { 
	    if (mouse_action_ == ACTION_NONE && me->type () == mouse_event::BUTTON_PUSHED 
		&& object_ui_ && ((atk_border_ui*)object_ui_)->is_in_ctr (me->x(), me->y()))
	      {
		on_delete (); 
		return 1; 
	      } 
	  }
        
        if (moveable_)
	  {
	    if (mouse_action_ == ACTION_NONE && me->type () == mouse_event::BUTTON_PUSHED &&
	        object_ui_ && ((atk_border_ui*)object_ui_)->is_in_bt (me->x(), me->y()))
	      {
		mouse_action_ = ACTION_MOVE;
                mouse_range_x_ = me->x () - get_x_real ();
                mouse_range_y_ = me->y () - get_y_real (); 
                return 1; 
	      } 
	  }
	
        if (resizeable_)
	  {
            if (mouse_action_ == ACTION_NONE && me->type () == mouse_event::BUTTON_PUSHED &&
                object_ui_ && ((atk_border_ui*)object_ui_)->is_in_cbr (me->x(), me->y ())) 
	      {
		mouse_action_ = ACTION_RESIZE;
                return 1; 
	      }  
	  }
        
      }  
    return 0; 
}


bool atk_window::update ()
{
    return alive_; 
}


void atk_window::shutdown ()
{
    alive_ = false; 
}


void atk_window::set_resizeable (const bool b)
{
    resizeable_ = b; 
}

bool atk_window::is_resizeable () const
{
    return resizeable_; 
}


void atk_window::set_deleted (const bool b)
{
    deleted_ = b; 
}

bool atk_window::is_deleted () const
{
    return deleted_; 
}


void atk_window::set_moveable (const bool b)
{
    moveable_ = b; 
}

bool atk_window::is_moveable () const
{
  return moveable_; 
}







void atk_window::realize ()
{
  atk_bin::realize();
  if (!child) return;
  
  if (child->get_length ()  != get_length () || child->get_height ()  != get_height ())
    {
      // WARNING maybe change and put this system in atk_bin ? ? 
      if (resizeable_)
        {
	  set_size (child->get_length () + (border_width_ << 1) , child->get_height () + (border_width_ << 1)); 
        }
      else
        {
	  child->set_size (get_length () -(border_width_ << 1), get_height () - (border_width_ << 1));
	  child->realize (); 
        }
    } 
}


