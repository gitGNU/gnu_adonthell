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

#include "bin.h"

using namespace gui;

bin::bin () : child (NULL)
{ 
}



void bin::add (widget * w)
{
  //if (child) remove (child); 
  clear();

    child = w;
    w->set_parent (this);
    
    w->set_position (0, 0);
        
    on_add (); 
}



void bin::remove (widget *w)
{
    w->set_parent (NULL);

    child = NULL;

    on_remove (); 
}


void bin::clear ()
{
    if (child) 
    {
        delete child; 
        child = NULL;
    }
}




void bin::update_position()
{
  container::update_position();
  
  if (child) child->update_position (); 
}

void bin::set_size (u_int32 length, u_int32 height)
{
    container::set_size (length, height);
    
    if (child)
    {
        child->set_size (get_length () -(border_width_ << 1), get_height () - (border_width_ << 1));
        child->realize ();
    }
}


bool bin::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (container::draw (da, sf))
    {
      assign_drawing_area(da);
      /* draw the contain */ 
      if (child) child->draw (this, sf);
      detach_drawing_area();
      return true;
    }
  return false;
}


/**
 * input update function
 * @return 1 if this object use the event,  else return 0
     */
int bin::input_update (input::event * ev)
{
  if (child) return child->input_update (ev);
  return 0;
}


bin::~bin ()
{
    clear (); 
}



