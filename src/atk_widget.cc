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

#include "atk_container.h"
#include "atk_widget.h"


atk_widget::atk_widget () : parent_ (NULL) 
{ 
}

 
void atk_widget::set_parent (atk_container & parent)
{
    parent_ = &parent;  
}


void atk_widget::set_position (s_int32 x, s_int32 y)
{
    if (parent_) drawing_area::move (parent_->get_x_real () + (x_ = x), parent_->get_y_real () + (y_ = y));
    else drawing_area::move (x_ = x, y_ = y); 
}


s_int32 atk_widget::get_x () const
{
    return x_; 
}


s_int32 atk_widget::get_y () const
{
    return y_; 
}


s_int32 atk_widget::get_x_real () const
{
    return drawing_area::x (); 
}


s_int32 atk_widget::get_y_real () const
{
    return drawing_area::y (); 
}


void atk_widget::set_size (u_int32 length, u_int32 height)
{
    resize (length, height); 
}


u_int32 atk_widget::get_length () const
{
    return drawing_area::length (); 
}


u_int32 atk_widget::get_height () const
{
    return drawing_area::height (); 
}


void atk_widget::set_visible (const bool b)
{
    if (visible_ = b) on_show ();
    else on_hide (); 
}


bool atk_widget::is_visible () const
{
    return visible_; 
}


void atk_widget::set_sensible (const bool b)
{
    sensible_ = b; 
}


bool atk_widget::is_sensible () const
{
    return sensible_; 
}


void atk_widget::set_can_focus (const bool b)
{
    can_focus_ = b; 
}


bool atk_widget::is_can_focus () const
{
    return can_focus_; 
}


void atk_widget::set_has_focus (const bool b)
{
    if (has_focus_ = b) on_focus_on ();
    else on_focus_off (); 
}


bool atk_widget::is_has_focus () const
{
    return has_focus_; 
}

 
void atk_widget::set_can_default (const bool b)
{
    can_default_ = b; 
}


bool atk_widget::is_can_default () const
{
    return can_default_; 
}


void atk_widget::set_has_default (const bool b)
{
    has_default_ = b; 
}


bool atk_widget::is_has_default () const
{
    return has_default_; 
}


atk_widget::~atk_widget ()
{
    on_destroy (); 
}


