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
 * @brief  Declares the container base class.
 * 
 * 
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_


#include <vector>  

#include "../callback_sig.h"
#include "widget.h"
#include "object_ui.h"

namespace gui
{

  class border_template;

  /** It's an abstract class, it's can contains lots of widget,  there are some method,  add,  remove a container...
   *
   */

  class container : public widget
    {
      public :
	
	
	/**Initialize some variable,  child to NULL and border_width (5); */
	container (); 
      
      
      /**
       * Add a widget in the list.
       * on_add event is executed
       * @param a widget to add
       */
      virtual void add (widget * w) = 0; 
      
      
      /** Remove a widget,  don't delete the object
       * on_remove event is executed
       * @param remove the widget
       */
      virtual void remove (widget *  w) = 0; 


      /** Set space between widget and container
       * @param border width
       */
      void set_border_width (const u_int16);
      
      
      /** get border width
       * @return the border width
       */
      u_int16 get_border_width () const;
      
      
      /** clear all widget of the vector and child,  they are deleted
       * No event is called
       */
      virtual void clear () = 0; 
      
      
      /**
       * draw the container
       */
      virtual bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
      
      
      /**
       * Destructor
       */
      virtual ~container (); 
      
      
      /**
       * Set the border for this container
       * @param bd_tmp : the border template use to display border, this objet alloc a border but not the template
       * futur change : change set_border in set_ui : ?? maybe 
       */
      void set_border_ui (border_template * bd_tmp); 
      
      
      /** set the minimum size of a widget
       * @param length
       * @param height
       */
      virtual void set_size (s_int32 length, s_int32 height); 
      
      
      /**
       * update position
       */
      virtual void update_position();
      
      /** 
       * it's used to build the widget.
       */
      virtual void realize ();
    
      
      /* call back */
      callback_sig on_add; //call when an objet is added
      callback_sig on_remove; //call when an objet is removed
    
      
      protected :
	
	/* the space between container and widgets childs */
	u_int16 border_width_; 
      
      
      /* the border used by this container */
      //border_ui * border_; 
      object_ui * object_ui_; 

      private : 

    };
};

#endif


