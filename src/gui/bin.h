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


/**
 * @file   container.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the bin base class.
 * 
 * 
 */

#ifndef BIN_H_
#define BIN_H_

#include "widget.h"
#include "container.h"

/** This class can contain only one object
 */
namespace gui
{
  
  class bin : public container
    {
      public :
	/** Constructor
	 */
	bin (); 
      
      
      /**
       * Add a widget in the list,  only one widget can be added !!!
       * on_add event is executed
       * @param a widget to add
       */
      void add (widget * w);

      
      /** Remove a widget,  don't delete the object
       * on_remove event is executed
       * @param remove the widget
       */
      void remove (widget *  w);
      
      
      
      /** clear all widget of the vector and child,  they are deleted of the memory
       * No event is called
       */
      void clear ();
      
      /**
       * update position
       */
      void update_position (); 
      
      
      /** set size of the window
       * @param length
       * @param height
       */
      void update_size ();
      
      /**
       * draw the window
       */
      bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
      
      
      /**
       * input update function
       * @return 1 if this object use the event,  else return 0
       */
      virtual int input_update (input::event *); 
      
      
      /**
       * Realize the bin
       */
      virtual void realize ();

      
      /**destructor
       * call clear () 
       */
      virtual ~bin (); 
      
      protected :
	
	widget * child; 
    }; 
};


#endif

