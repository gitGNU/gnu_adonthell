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

#include "atk_border_ui.h"
#include "atk_container.h"


atk_container::atk_container () : border_width_ (5), object_ui_ (NULL) 
{  
}


void atk_container::set_border_width (const u_int16 b)
{
    border_width_ = b; 
}


u_int16 atk_container::get_border_width () const
{
  return border_width_; 
}



atk_container::~atk_container ()
{
  if (object_ui_) delete object_ui_; 
}


void atk_container::set_border_ui (atk_border_template * bd_tmp)
{
  if ( object_ui_)  delete object_ui_;
  object_ui_ = new atk_border_ui (this);
  ((atk_border_ui*)object_ui_)->set_border (bd_tmp); 
}

bool atk_container::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (atk_widget::draw (da, sf) )
    {
      assign_drawing_area (da);
      if (object_ui_) object_ui_->draw(da, sf);
      detach_drawing_area ();
      return true;
    }
  return false;
}


void atk_container::set_size (s_int32 length, s_int32 height)
{
  atk_widget::set_size (length, height);
  if (object_ui_) object_ui_->resize();
}

void atk_container::update_position ()
{
  atk_widget::update_position();
  if (object_ui_) object_ui_->move();
}


void atk_container::realize()
{
  atk_widget::realize();
  if (object_ui_) object_ui_->resize ();
}











