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

#include "gfx/screen.h"

#include "container.h"
#include "widget.h"

using namespace gui;

widget::widget () : parent_ (NULL) 
{    
}

 
void widget::set_parent (container * parent)
{
  parent_ = parent;  
}


void widget::set_position (s_int16 x, s_int16 y)
{
  x_ = x; 
  y_ = y;

  update_position (); 
}

void widget::update_position ()
{
  /* if there is a parent, calcul obsolute position with relative position */
  if (parent_) drawing_area::move (parent_->get_x_real () + x_ + parent_->get_border_width (), parent_->get_y_real () + y_ +parent_->get_border_width () );
  else drawing_area::move (x_, y_); 
}

s_int16 widget::get_x () const
{
  return x_; 
}


s_int16 widget::get_y () const
{
  return y_; 
}


s_int16 widget::get_x_real () const
{
  return drawing_area::x (); 
}


s_int16 widget::get_y_real () const
{
  return drawing_area::y (); 
}


void widget::set_size (s_int32 length, s_int32 height)
{
  if (get_length () == length && get_height () == height) return; 
  
  if ( length >= 0 && height >=0) drawing_area::resize (length, height); 
  else if (length >= 0 ) drawing_area::resize (length, 0);
  else if (height >= 0 ) drawing_area::resize (0, height);
  else drawing_area::resize (0, 0);

  update_size ();
}


void widget::update_size ()
{
}


u_int16 widget::get_length () const
{
  return drawing_area::length (); 
}


u_int16 widget::get_height () const
{
  return drawing_area::height (); 
}


void widget::set_visible (const bool b)
{
  if (visible_ = b) on_show ();
  else on_hide (); 
}


bool widget::is_visible () const
{
  return visible_; 
}


void widget::set_sensible (const bool b)
{
  sensible_ = b; 
}


bool widget::is_sensible () const
{
  return sensible_; 
}


void widget::set_can_focus (const bool b)
{
  can_focus_ = b; 
}


bool widget::is_can_focus () const
{
  return can_focus_; 
}


void widget::set_has_focus (const bool b)
{
  if (has_focus_ = b) on_focus_on ();
  else on_focus_off (); 
}


bool widget::is_has_focus () const
{
  return has_focus_; 
}

 
void widget::set_can_default (const bool b)
{
  can_default_ = b; 
}


bool widget::is_can_default () const
{
  return can_default_; 
}


void widget::set_has_default (const bool b)
{
  has_default_ = b; 
}


bool widget::is_has_default () const
{
  return has_default_; 
}


widget::~widget ()
{
  on_destroy (); 
}



bool widget::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  return visible_;
}


int widget::input_update (input::event * ev)
{
  return 0;
}


bool widget::update ()
{
  return true; 
}
