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

#include "atk_bin.h"

atk_bin::atk_bin () : child (NULL)
{ 
}



void atk_bin::add (atk_widget * w)
{
  //if (child) remove (child); 
  clear();

    child = w;
    w->set_parent (this);
    
    w->set_position (0, 0);
        
    on_add (); 
}



void atk_bin::remove (atk_widget *w)
{
    w->set_parent (NULL);

    child = NULL;

    on_remove (); 
}


void atk_bin::clear ()
{
    if (child) 
    {
        delete child; 
        child = NULL;
    }
}




void atk_bin::update_position()
{
  atk_container::update_position();
  
  if (child) child->update_position (); 
}

void atk_bin::set_size (u_int32 length, u_int32 height)
{
    atk_container::set_size (length, height);
    
    if (child)
    {
        child->set_size (get_length () -(border_width_ << 1), get_height () - (border_width_ << 1));
        child->realize ();
    }
}


bool atk_bin::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (atk_container::draw (da, sf))
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
int atk_bin::input_update (input::event * ev)
{
  if (child) return child->input_update (ev);
  return 0;
}


atk_bin::~atk_bin ()
{
    clear (); 
}



