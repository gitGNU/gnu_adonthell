/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file drawable.h
 * Declares the drawable class.
 */ 

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include "drawing_area.h"

class image; 

/**
 * Template class for drawable objects manipulation.
 *
 * This class is designed to allow flexibility in
 * drawable objects manipulation. It also serves as
 * a template when creating your own classes.
 *
 * It defines the basic virtual methods every drawable
 * object is required to have. When you design such drawable
 * object, make it inherit from this class and overload the virtual
 * functions you wish to use.
 *
 * The main advantage of this class is that it allows you to manipulate
 * any drawable object (image, animation, mapview...) without caring
 * about it's type, at the little cost of having to
 *
 * Anyway, there are a few methods that are required to be overloaded
 * in your class. The draw method is a must-have. Your object must also
 * take care to sets the size of the drawable correctly (the best thing
 * being that it should use the drawable's size as it's own and don't
 * overload the length () and height () methods).
 * 
 */
class drawable
{
public:
    
    /** 
     * Default constructor.
     * 
     */
    drawable (); 
    /**
     * Destructor.
     */ 
    virtual ~drawable ();

    /** 
     * Returns the length of the drawable.
     * 
     * 
     * @return length of the drawable.
     */
    u_int16 length () 
    {
        return length_; 
    }

    /** 
     * Returns the height of the drawable.
     * 
     * 
     * @return height of the drawable.
     */
    u_int16 height () 
    {
        return height_; 
    }

    /** 
     * Virtual update function, provided for objects which
     * don't need one.
     * 
     */
    virtual void update ();

    /** 
     * Draw the object on the %screen.
     * 
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param da_opt Optional drawing_area to temporarly assign during the
     * drawing operation.
     */
    virtual void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL) = 0;

    /**
     * Draws the current graphical representation of the drawable.
     * @param r the image where to draw the drawable.
     */ 
    void snapshot (image& r);
    
protected:
    
    /** 
     * Sets the length of the drawable.
     * 
     * @param l new length.
     */
    void set_length (u_int16 l)
    {
        length_ = l; 
    }

    /**
     * Sets the height of the drawable.
     *
     * @param h new height.
     */ 
    void set_height (u_int16 h)
    {
        height_ = h; 
    }
    
private:
    u_int16 length_; 
    u_int16 height_; 
};

#endif

