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
 * @file   manager.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the manager base class.
 * 
 * 
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <deque> 

#include "widget.h"
#include "window.h"
#include "input/manager.h"

namespace gui
{

  /** This class is the window manager. it catch all event, keyboard, mouse ... It display all window.
   */
  class manager
    {
      public :

	manager ();
    

      /**
       * init method
       */
      void init (); 
         
    
      /** Add at the end of list a window
       * @param widget to add
       */
      void add (window *w); 
    
    
      /** Remove a window
       * @param widget to remove
       */
      void remove (window * w);
    
    
      /** Clear the manager and remove window of the memory 
       */
      void clear (); 
    
    
      /**
       * draw all contains.
       */
      void draw (); 


      /**
       * update all the window system
       * @return false if the system is shutdown 
       */
      bool update (); 
    
    
      // protected : 

      /**
       * Executed when an keyboard event is processed
       */
      //     int keyboard_event (input_event * ev);
    
    
      /**
       * Executed when an keyboard event is processed
       */
      //     int mouse_event (input_event * ev);

      int input_update (input::event *ev); 
    

      /**
       * If this function is call,  alive_ variable become false and update function return  false
       */
      void shutdown (); 
    

      /**
       * set the focus windows,  this function put the window on the foreground and cur_window_ at the param, in this contexte it is the first window updated
       * by input event.
       * @param a window 
       */
      void set_focus (window *); 

 
      /**
       * Destructor
       * Delelte from memory all component
       */
      ~manager (); 
    
      private : 

	/* input listener */ 
	input::listener il_; 
    

      /**current selected window*/
      window * cur_window_; 
    
    
      /* list of all window that contain this class */
      std::deque<window*> v_window_;

    
      /* alive,  return by update */
      bool alive_; 


      /* mouse position
	 WARNING replace this with a cursor*/
      u_int16 mouse_x_;
      u_int16 mouse_y_; 
    
    };
};
#endif


