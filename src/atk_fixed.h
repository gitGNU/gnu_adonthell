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
 * @file   atk_box.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_container base class.
 * 
 * 
 */

#ifndef ATK_FIXED_H_
#define ATK_FIXED_H_

#include <vector>  
#include "atk_widget.h"
#include "atk_container.h"

/**
 * This container can contains lots of widget, you can choice widget disposition, horizontal or vertical.
 */
class atk_fixed : public atk_container
{
public :
    /** Constructor
     */
    atk_fixed (); 
    
    
    /**
     * Add a widget in the list,  at the end of the list. It's same than add_end () 
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

    
    /** It's used to build the widget. Here do nothing for now
     */
    void realize ();
    

    /**draw the widget
     */
    virtual void draw (drawing_area * da = NULL, surface * sf = NULL);
    

    
    ~atk_fixed (); 
    
protected : 
    

private :

    /* contains widget */
    std::vector <atk_widget * > v_widget_;   
};
#endif







