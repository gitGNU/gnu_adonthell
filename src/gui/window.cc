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

#include "input/mouse_event.h"

#include "border_ui.h"
#include "window.h"

using namespace gui;

window::window () :  alive_ (true) 
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


void window::set_title (const std::string & t)
{
  title_ = t; 
}


void window::set_type (const u_int8 t)
{
  type_ = t; 
}


bool window::is_modal () const
{
  return modal_; 
}


void window::set_modal (const bool m)
{
  modal_ = m; 
}



int window::input_update (input::event * ev)
{
  if (!is_sensible () ) return 0; 
  /* if the object inside the window keep the event,we return 1 */
  if (bin::input_update (ev) == 1) return 1;
  
  /* translate event into mouse_event */
  input::mouse_event * me = (input::mouse_event *) ev; 
  
  /* we check if there is already a mouse action in progress */
  if (mouse_action_ == ACTION_MOVE)
    {
      /* if mouse action is move and event is mouse_motion */
      if (me->type () == input::mouse_event::MOUSE_MOTION)
	{
	  set_position (me->x ()  - mouse_range_x_, me->y ()  - mouse_range_y_);
	  return 1; 
	}
      
      /* if mouse event is button_RELEASED so the action become none */
      else if (me->type () == input::mouse_event::BUTTON_RELEASED)
	{
	  mouse_action_ = ACTION_NONE; 
	  return 1; 
	} 
    }
  /* if mouse action is resize */
  else if (mouse_action_ == ACTION_RESIZE)
    {
      /* if mouse event is mouse motion, we resize the objet */
      if (me->type () == input::mouse_event::MOUSE_MOTION)
	{
	  set_size ( me->x ()  - get_x_real (), me->y ()  - get_y_real ());
	  return 1; 
	}
      /* if mouse event is mouse released, we set action to none */
      else if (me->type () == input::mouse_event::BUTTON_RELEASED)
	{
	  mouse_action_ = ACTION_NONE; 
	  return 1; 
	} 
    }
  
  /* if the action is inside the window we return null*/
  if (point_belong (me->x (), me->y ())) return 0; 
  else
    {
      /* there are no action in progress we check if the cursor is on special border */
      if (deleted_)
	{ 
	  if (mouse_action_ == ACTION_NONE && me->type () == input::mouse_event::BUTTON_PUSHED 
	      && object_ui_ && ((border_ui*)object_ui_)->is_in_ctr (me->x(), me->y()))
	    {
	      on_delete (); 
	      return 1; 
	    } 
	}
      
      if (moveable_)
	{
	  if (mouse_action_ == ACTION_NONE && me->type () == input::mouse_event::BUTTON_PUSHED &&
	      object_ui_ && ((border_ui*)object_ui_)->is_in_bt (me->x(), me->y()))
	    {
	      mouse_action_ = ACTION_MOVE;
	      mouse_range_x_ = me->x () - get_x_real ();
	      mouse_range_y_ = me->y () - get_y_real (); 
	      return 1; 
	    } 
	}
      
      if (resizeable_)
	{
	  if (mouse_action_ == ACTION_NONE && me->type () == input::mouse_event::BUTTON_PUSHED &&
	      object_ui_ && ((border_ui*)object_ui_)->is_in_cbr (me->x(), me->y ())) 
	    {
	      mouse_action_ = ACTION_RESIZE;
	      return 1; 
	    }  
	}
      
    }  
  return 0; 
}


bool window::update ()
{
    return alive_; 
}


void window::shutdown ()
{
    alive_ = false; 
}


void window::set_resizeable (const bool b)
{
    resizeable_ = b; 
}

bool window::is_resizeable () const
{
    return resizeable_; 
}


void window::set_deleted (const bool b)
{
    deleted_ = b; 
}

bool window::is_deleted () const
{
    return deleted_; 
}


void window::set_moveable (const bool b)
{
    moveable_ = b; 
}

bool window::is_moveable () const
{
  return moveable_; 
}


void window::realize ()
{
  bin::realize();
  if (!child) return;
  
/*  if (child->get_length ()  != get_length () - get_border_width() || child->get_height ()  != get_height () - get_border_width ())
    {
      // WARNING maybe change and put this system in bin ? ? 
      // if the window is resizable, the window is adapted to the child
      if (resizeable_)
        {
	  set_size (child->get_length () + (border_width_ << 1) , child->get_height () + (border_width_ << 1)); 
	  child->set_position ( get_border_width(), get_border_width());
	}
      else
        {
	  //if the window is not resizable, we resize the child 
	  child->set_size (get_length () - (border_width_ << 1),get_height () - (border_width_ << 1));
	  child->realize (); 
        }
    } */
}


