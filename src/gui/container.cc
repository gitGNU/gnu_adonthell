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

#include "border_ui.h"
#include "container.h"

using namespace gui;

container::container () : border_width_ (5), object_ui_ (NULL) 
{  
}


void container::set_border_width (const u_int16 b)
{
  border_width_ = b; 
}


u_int16 container::get_border_width () const
{
  return border_width_; 
}



container::~container ()
{
  /* destroy the decoration */
  if (object_ui_) delete object_ui_; 
}


void container::set_border_ui (border_template * bd_tmp)
{
  if ( object_ui_)  delete object_ui_;
  object_ui_ = new border_ui (this);
  ((border_ui*)object_ui_)->set_border (bd_tmp); 
}

bool container::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (widget::draw (da, sf) )
    {
      assign_drawing_area (da);
      if (object_ui_) object_ui_->draw(da, sf);
      detach_drawing_area ();
      return true;
    }
  return false;
}


void container::set_size (s_int32 length, s_int32 height)
{
  widget::set_size (length, height);
  if (object_ui_) object_ui_->resize();
}

void container::update_position ()
{
  widget::update_position();
  /* we move the decoration associated at this container*/
  if (object_ui_) object_ui_->move();
}


void container::realize()
{
  widget::realize();
  /* we call resize for objet_ui */
  if (object_ui_) object_ui_->resize ();
}











