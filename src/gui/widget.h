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
 * @file   widget.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the widget base class.
 * 
 * 
 */
#ifndef WIDGET_H_
#define WIDGET_H_

#include "input/event.h"

#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "../callback_sig.h"


namespace gui
{
  
  class container; 
  
  class window;
  
  /**
   * It's the common class for all object in the Adonthell Tool Kit.
   */
  class widget : protected gfx::drawing_area
    {
      public : 
	/**
	 * Constructor,  initialise some variable. 
	 */
	widget (); 
      
      /** 
	  This method is abstract, it's used to build the widget.
      */
      virtual void realize () //  = 0; 
	{}
      
      /** Set the parent of this widget. A widget can have only one parent
       *  @param parent his new parent
       */ 
      void set_parent (container * parent);  
      
      /**
       * draw the widget
       */
      virtual bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
      
      
      /** set position of this widget (but the layout can change this value) ,  position in his parent
       * @param x
       * @param y
       */
      void set_position (s_int16 x, s_int16 y); 
      
      
      /** update position
       */
      virtual void update_position (); 
      
      
      /** get x position
       * @return x position in his parent
       */
      s_int16 get_x () const; 
      
      
      
      /** get y position
       * @return y position in his parent
       */
      s_int16 get_y () const; 
      
      
      
      /** get x real position
       * @return real x position in the screen 
       */
      s_int16 get_x_real () const; 
      
      
      /** get y real position
       * @return real y position in the screen 
       */
      s_int16 get_y_real () const; 
      
      
      /** set the minimum size of a widget
       * @param length
       * @param height
       */
      void set_size (s_int32 length, s_int32 height); 
      
      
      /**
       * update size
       */
      virtual void update_size (); 


      /** get length
       * @return length 
       */
      u_int16 get_length () const; 
      
      
      /** get height
       * @return height 
       */
      u_int16 get_height () const; 
      
      
      /** set is this object is visible.
       * An event is executed on_show or on_hide depend on bool value.
       * @param b true visible, false hide.
       */
      void set_visible (const bool b);
      
      
      /**
       *@return true is this object is visible
       */
      bool is_visible () const; 
      
      
      /** set is this object is sensible, if sensible this object can receive input event.
       * @param b true sensible, false unsensible.
       */
      void set_sensible (const bool b);
      
      
      /**
       *@return true is this object is sensible.
       */
      bool is_sensible () const; 
      
      
      /** set if this object can have focus.
       * @param b true is this object can have focus .
       */
      void set_can_focus (const bool b);
      
      
      /**
       *@return true is this object can have focus.
       */
      bool is_can_focus () const; 
      
      
      /** set if this object has focus.
       * An event is executed on_focus_on or on_focus_off depend on bool value.
       * @param b true is this object has focus .
       */
      void set_has_focus (const bool b);
      
      
      /**
       *@return true is this object has focus.
       */
      bool is_has_focus () const;
      
      
      /** set if this object can have default.
       * @param b true is this object can have default.
       */
      void set_can_default (const bool b);
      
      
      /**
       *@return true is this object can have default.
       */
      bool is_can_default () const; 
      
      
      /** set if this object has default.
       * @param b true is this object has default.
       */
      void set_has_default (const bool b);
      
      
      /**
       * @return true is this object has default.
       */
      bool is_has_default () const;

      
      /**
       * input update function
       * @return 1 if this object use the event,  else return 0
       */
      virtual int input_update (input::event *); 

      
      /** 
       * update the widget
       * @return true if no problem,  it is very usefull in window.
       */
      virtual bool update (); 
      
      
      int get_id () const;

    
      /** Execute on_destroy callback 
       */
      virtual ~widget (); 
      
      
      /*define event */
      callback_sig on_show;
      
      callback_sig on_hide;
      
      callback_sig on_focus_on;
      
      callback_sig on_focus_off; 
      
      callback_sig on_destroy; 
      
      protected : 
	
    
	
	/* get event from this windows */
	// window * parent_window_; 
	
	
	/* parent of this widget,  */
	container * parent_; 
      
      
      /* position of the widget, position in the parent */
      s_int16 x_;
      s_int16 y_;
      
      
      /* boolean if this object is visible*/
      bool visible_; 
      
      
      /* if th object is sensible (input : keyboard, mouse...) */
      bool sensible_;
      
      
      /* true if the object can has focus */
      bool can_focus_;

      
      /* true if the object has focus */
      bool has_focus_;
      
      
      /* true if the object can be default */
      bool can_default_;
      
      
      /* true if the object has default */
      bool has_default_;
    

      static int ID_;

      int my_ID;

    };  
  
};
#endif



