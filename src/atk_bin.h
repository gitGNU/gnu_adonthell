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
 * @brief  Declares the atk_bin base class.
 * 
 * 
 */

#ifndef ATK_BIN_H_
#define ATK_BIN_H_

#include "atk_widget.h"
#include "atk_container.h"

/** This class can contain only one object
 */
class atk_bin : public atk_container
{
public :
    /** Constructor
     */
    atk_bin (); 

    
    /**
     * Add a widget in the list,  only one widget can be added !!!
     * on_add event is executed
     * @param a widget to add
     */
    void add (atk_widget * w);

    
    /** Remove a widget,  don't delete the object
     * on_remove event is executed
     * @param remove the widget
     */
    void remove (atk_widget *  w);
    

    
    /** clear all widget of the vector and child,  they are deleted of the memory
     * No event is called
     */
    void clear ();

    
    /** set position of this widget (but the layout can change this value) ,  position in his parent
     * @param x
     * @param y
     */
    void set_position (s_int32 x, s_int32 y);
    

    /**destructor
     * call clear () 
     */
    ~atk_bin (); 
protected :
     
    atk_widget * child; 
}; 



#endif

