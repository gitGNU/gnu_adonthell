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
 * @file   box.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the container base class.
 * 
 * 
 */

#ifndef FIXED_H_
#define FIXED_H_

#include <vector>  
#include "widget.h"
#include "container.h"

namespace gui
{

  /**
   * This container can contains lots of widget, you can choice widget disposition, horizontal or vertical.
   */
  class fixed : public container
    {
      public :
	/** Constructor
	 */
	fixed (); 
    
    
      /**
       * Add a widget in the list,  at the end of the list. It's same than add_end () 
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

    
      /** It's used to build the widget. Here do nothing for now
       */
      void realize ();
    

      /**draw the widget
       */
      virtual bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL);
    
      /**
       * destructor
       */
      ~fixed (); 
    
      protected : 
    

	private :

	/* contains widget */
	std::vector <widget * > v_widget_;   
    };
};
#endif







