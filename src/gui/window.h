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

#ifndef WINDOW_H_
#define WINDOW_H_

#include <string> 

#include "widget.h"
#include "bin.h"

namespace gui
{

  /** 
      This class is a top level container
  */

  class window : public bin
    {
      public :

    
	/**
	 * Constructor
	 * set modal true
	 */ 
	window (); 
    
    
      /**
       * Set the title of the window
       * @param title of the window
       */
      void set_title (const std::string &);  

    
    
      /**
       * set type of the window
       * @param type of the window
       */
      void set_type (const u_int8); 
 

      /**
       * set modal
       * @param true if the window is a modal window
       */
      void set_modal (const bool);


      /**
       * is_modal
       * @return true if the window is a modal window
       */
      bool is_modal ()  const; 


      /**
       * set deleted with mouse
       * @param true if can be deleted
       */
      void set_deleted (const bool); 

    
      /**
       * @return if this window can be deleted with mouse
       */
      bool is_deleted () const; 
    

      /**
       * set moveable with mouse
       * @param true if can be moved with mouse
       */
      void set_moveable (const bool); 

    
      /**
       * @return if this window can be moved with mouse
       */
      bool is_moveable () const; 
    

      /**
       * set resizeable with mouse
       * @param true if can be resized
       */
      void set_resizeable (const bool); 
    
        
      /**
       * @return if this window can be resized with mouse
       */
      bool is_resizeable () const; 
    
    
      /**
       * input update function
       * @return 1 if this object use the event,  else return 0
       */
      int input_update (input::event *); 
    

      /**
       * If this function is call,  alive_ variable become false and update function return  false
       */
      void shutdown ();
    
    
      /** 
       * update the widget
       * @return alive_ variable,  if alive_ is false the window manager destroy this object
       */
      virtual bool update ();
    

      /**
       * use to build the window.
       */ 
      void realize (); 
      
    
      /* type of the window,  use for decoration */
      const static u_int8 TOPLEVEL = 0;
      const static u_int8 DIALOG = 1;
      const static u_int8 POPUP = 2;

    
      /* sig */
      /* this sig is call when you close the window */
      callback_sig on_delete; 
    
      protected : 


	private :
    
	/* type of the window*/
	u_int8 type_; 
    
      /* title of the window */
      std::string title_;

    
      /* resizeable with the mouse*/
      bool resizeable_;

    
      /* set if the widow can be deleted with the mouse */
      bool deleted_; 


      /* set if the window can be move with the mouse */
      bool moveable_; 

    
      /* modal */
      bool modal_;
    
    
      /* alive */
      bool alive_; 


      /* mouse range */
      u_int16 mouse_range_x_;
      u_int16 mouse_range_y_; 

    

      /* mouse status action */ 
      typedef enum
        {
	  ACTION_NONE, ACTION_MOVE, ACTION_RESIZE
        } mouse_action;
      mouse_action mouse_action_;
    };
};

#endif



