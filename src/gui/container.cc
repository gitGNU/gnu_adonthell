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

container::container () : my_border_width (5), my_object_ui (NULL) 
{  
}


void container::set_border_width (const u_int16 b)
{
  my_border_width = b; 
}


u_int16 container::get_border_width () const
{
  return my_border_width; 
}



container::~container ()
{
  /* destroy the decoration */
  if (my_object_ui) delete my_object_ui; 
}


void container::set_border_ui (border_template * bd_tmp)
{
  if ( my_object_ui)  delete my_object_ui;
  my_object_ui = new border_ui (this);
  ((border_ui*)my_object_ui)->set_border (bd_tmp); 
}

bool container::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (widget::draw (da, sf) )
    {
      assign_drawing_area (da);
      if (my_object_ui) my_object_ui->draw(da, sf);
      detach_drawing_area ();
      return true;
    }
  return false;
}

void container::update_size ()
{
  widget::update_size ();
  
  if (my_object_ui) my_object_ui->resize();
}

void container::update_position ()
{
  widget::update_position();
  /* we move the decoration associated at this container*/
  if (my_object_ui) my_object_ui->move();
}


void container::realize()
{
  widget::realize();
  /* we call resize for objet_ui */
  if (my_object_ui) my_object_ui->resize ();
}











