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
 * @file   atk_manager.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_manager base class.
 * 
 * 
 */

#ifndef ATK_MANAGER_H_
#define ATK_MANAGER_H_

#include <deque> 

#include "atk_widget.h"
#include "atk_window.h"
#include "input_manager.h"

/** This class is the window manager. it catch all event, keyboard, mouse ... It display all atk_window.
 */
class atk_manager
{
public :

    atk_manager ();
    

    /**
     * init method
     */
    void init (); 
         
    
    /** Add at the end of list a atk_window
     * @param widget to add
     */
     void add (atk_window *w); 
    
    
    /** Remove a atk_window
     * @param widget to remove
     */
     void remove (atk_window * w);
    
    
    /** Clear the atk_manager and remove atk_window of the memory 
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

    int input_update (input_event *ev); 
    

    /**
     * If this function is call,  alive_ variable become false and update function return  false
     */
    void shutdown (); 
    

    /**
     * set the focus windows,  this function put the window on the foreground and cur_window_ at the param, in this contexte it is the first window updated
     * by input event.
     * @param a window 
     */
    void set_focus (atk_window *); 

 
    /**
     * Destructor
     * Delelte from memory all component
     */
    ~atk_manager (); 
    
private : 

    /* input listener */ 
    input_listener il_; 
    

    /**current selected window*/
    atk_window * cur_window_; 
    
    
    /* list of all window that contain this class */
    std::deque<atk_window*> v_window_;

    
    /* alive,  return by update */
    bool alive_; 


    /* mouse position
       WARNING replace this with a cursor*/
    u_int16 mouse_x_;
    u_int16 mouse_y_; 
    
};

#endif


