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
 * @file   atk_container.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_container base class.
 * 
 * 
 */

#ifndef ATK_CONTAINER_H_
#define ATK_CONTAINER_H_


#include <vector>  

#include "callback_sig.h"
#include "atk_widget.h"

class atk_border; 

/** It's an abstract class, it's can contains lots of widget,  there are some method,  add,  remove a container...
 *
 */

class atk_container : public atk_widget
{
public :


    /**Initialize some variable,  child to NULL and border_width (6); */
    atk_container (); 

    
    /**
     * Add a widget in the list.
     * on_add event is executed
     * @param a widget to add
     */
    virtual void add (atk_widget * w) = 0; 


    /** Remove a widget,  don't delete the object
     * on_remove event is executed
     * @param remove the widget
     */
    virtual void remove (atk_widget *  w) = 0; 


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


    virtual ~atk_container (); 
    
    
    
    /* call back */
    callback_sig on_add;
    callback_sig on_remove; 
    
    
protected :
    
    /* the space between container and widgets childs */
    u_int16 border_width_; 


    /* the border used by this container */
    atk_border * border_; 
    
private : 

    
    
};


#endif

