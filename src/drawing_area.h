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


/** @file drawing_area.h
 *  @brief Declares the rect and drawing_area classes.
 */ 



#ifndef _drawing_area_h
#define _drawing_area_h

#include "types.h"




/** Simple rectangle class
 */ 
class rect
{
public:
    /** Returns the length of the rect.
     *  @return length of the rect.
     */ 
    s_int16 length ()
    {
        return length_;
    }
    
    /** Returns the height of the rect.
     *  @return height of the rect.
     */ 
    s_int16 height ()
    {
        return height_;
    }

    /** Returns the horizontal position of the rect.
     *  @return horizontal position of the rect.
     */ 
    s_int16 x ()
    {
        return x_;
    }
    
    /** Returns the vertical position of the rect.
     *  @return vertical position of the rect.
     */ 
    s_int16 y ()
    {
        return y_;
    }

    /** Move the rect.
     *  @param nx new horizontal position.
     *  @param ny new vertical position.
     */ 
    void move (s_int16 nx, s_int16 ny)
    {
        x_ = nx;
        y_ = ny;
    }

    /** Resize the rect.
     *  @param nl new length.
     *  @param nl new height.
     */ 
    void resize (s_int16 nl, s_int16 nh)
    {
        length_ = nl;
        height_ = nh;
    }
    
private:
    s_int16 x_, y_, length_, height_;
};




/** Implements "drawing zones" for drawing operations.
 *  An object which is drawn into a drawing_area will only appear in
 *  the rectangular zone it covers.
 *  During some drawing operations, you may want to limit the blits to a limited
 *  area of the screen. For example, if you want to draw an image into a
 *  window, and that image is larger than this window, you don't want the
 *  entire image to appear, only the part that fits the window. This is exactly
 *  what drawing areas are for. A drawing area is a square that can be any size
 *  and located anywhere on the screen. If you assign a drawing area to a
 *  drawable object (for example, an image), and then draw the image on the
 *  screen, the part of the image that doesn't fit the drawing area limits isn't
 *  displayed. A drawing area can be assigned to any drawable, but also to
 *  another drawing area, in which case the result of blitting operations from
 *  objects that are assigned to the second drawing area will be limited to the
 *  intersection of the two drawing areas.
 */ 
class drawing_area:public rect
{
public:
    /** Default constructor.
     *  The drawing_area is then located at (0, 0) and is (0, 0) sized.
     */ 
    drawing_area ();
    
#ifndef SWIG
    /** Builds a drawing_area from the parameters.
     *  @param px X position.
     *  @param py Y position.
     *  @param pw Length.
     *  @param ph Height.
     */ 
    drawing_area (s_int16 px, s_int16 py, s_int16 pw, s_int16 ph);
#endif

    /** Assign a drawing_area to this drawing_area.
     *  If a drawing area is assigned to another one, the zone covered
     *  by the drawing_area is the intersection of the two.
     *  @param da the drawing_area to assign.
     */ 
    void assign_drawing_area (drawing_area * da)
    {
        draw_to = da;
    }

    /** Detach (if needed) the drawing_area which was attached to this
     *  one.
     */ 
    void detach_drawing_area ()
    {
        draw_to = NULL;
    }
    
#ifndef SWIG
    /** Copy drawing areas.
     */ 
    drawing_area & operator = (const rect & r);
    
    /** Returns the intersection of two drawing_areas.
     */ 
    drawing_area operator + (drawing_area & da);
#endif
    
private:
    /** The drawing_area that is attached to this one.
     */ 
    drawing_area * draw_to;

    /** Returns a rect that is the intersection of
     *  this drawing area and the ones that are recursively
     *  attached to it */ 
    rect get_rects ();

    /** Returns whether the X position px is covered by the drawing_area.
     */ 
    bool is_x_in (s_int16 px);

    /** Returns whether the Y position py is covered by the drawing_area.
     */ 
    bool is_y_in (s_int16 py);

    /** Returns whether the dot (px, py) is covered by the drawing_area.
     */ 
    bool is_point_in (s_int16 px, s_int16 py);

#ifndef SWIG
    friend class image;
    friend class screen;
#endif
};

#endif
