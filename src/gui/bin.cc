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

#include <iostream>
#include "bin.h"

using namespace gui;

bin::bin () : child (NULL)
{ 
}



void bin::add (widget * w)
{
  clear();
  
  //this container can contain just one widget
  child = w;

  /* we set this at the parent for w objet */
  w->set_parent (this);

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
  
  /* if there is a child update_position of the child */
  if (child) child->update_position (); 
}


void bin::update_size ()
{
  /* update container size */
  container::update_size ();
  
  /* if there is a child, we resize it and rebuild it */
  if (child)
    {
      /* we center the alone child */
      child->set_size (get_length () - ( get_border_width () << 1 ), get_height () - ( get_border_width() << 1));
      child->realize ();
    }
}



bool bin::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  /* draw the container */
  if (container::draw (da, sf))
    {
      /* attach the child at the drawing area */
      assign_drawing_area(da);
      /* draw the contain */ 
      if (child) child->draw (this, sf);
      /* detach the drawing area */
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
  /* if this objet is unsensible we return 0 */
  if ( !is_sensible () || child == NULL) return 0;
  
  /* if there is a child we send the event at the child */
  return child->input_update (ev);
}



void bin::realize ()
{
  container::realize ();
  //check size and location of children
  set_position ( get_x(), get_y());
  set_size (get_length(), get_height());
  //WARNING : execute child->realize () ??
}


bin::~bin ()
{
  clear (); 
}
