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
 * @file   drawing_area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the drawing_area class.
 * 
 * 
 */

#ifndef DRAWING_AREA_H_
#define DRAWING_AREA_H_

#include "types.h"


/**
 * Implements "drawing zones" for drawing operations.
 * An object which is drawn into a drawing_area will only appear in
 * the rectangular zone it covers.
 * During some drawing operations, you may want to limit the blits to a limited
 * area of the %screen. For example, if you want to draw an image into a
 * window, and that image is larger than this window, you don't want the
 * entire image to appear, only the part that fits into the window. This is
 * exactly what drawing areas are for. A drawing area is a square that can be
 * any size and located anywhere on the %screen. If you assign a drawing area
 * to a drawable object (for example, an image), and then draw the image on
 * the %screen, the part of the image that doesn't fit the drawing area limits
 * isn't displayed. A drawing area can be assigned to any drawable, but also
 * to another drawing area, in which case the result of blitting operations
 * from objects that are assigned to the second drawing area will be limited
 * to the intersection of the two drawing areas. Recursively, you can use as many
 * drawing_areas as you wish for drawing operations.
 */ 
class drawing_area
{ 
public:
    /** Default constructor.
     *  The drawing_area is then located at (0, 0) and is (0, 0) sized.
     */  
    drawing_area ();

    /** Builds a drawing_area from the parameters.
     *  @param px X position.
     *  @param py Y position.
     *  @param pw Length.
     *  @param ph Height.
     */  
    drawing_area (s_int16 px, s_int16 py, u_int16 pw, u_int16 ph); 

    /** Returns the horizontal position of the drawing_area.
     *  @return horizontal position of the drawing_area.
     */  
    s_int16 x ()
    {
        return rect.x; 
    }

    /** Returns the vertical position of the drawing_area.
     *  @return vertical position of the drawing_area.
     */  
    s_int16 y () 
    {
        return rect.y; 
    }

    /** Returns the length of the drawing_area.
     *  @return length of the drawing_area.
     */  
    u_int16 length () 
    {
        return rect.w; 
    }

    /** Returns the height of the drawing_area.
     *  @return height of the drawing_area.
     */  
    u_int16 height () 
    {
        return rect.h; 
    }

    /** Move the drawing_area.
     *  @param nx new horizontal position.
     *  @param ny new vertical position.
     */  
    void move (s_int16 nx, s_int16 ny) 
    {
        rect.x = nx;
        rect.y = ny;
    }

    /** Resize the drawing_area.
     *  @param nl new length.
     *  @param nl new height.
     */ 
    void resize (u_int16 nl, u_int16 nh) 
    {
        rect.w = nl;
        rect.h = nh; 
    }

    /** Assign a drawing_area to this drawing_area.
     *  If a drawing area is assigned to another one, the zone covered
     *  by the drawing_area is the intersection of the two.
     *  @param da the drawing_area to assign.
     */  
    void assign_drawing_area (drawing_area * da) 
    {
        draw_to = da; 
    }

    /** 
     * Returns a pointer to the drawing_area attached to this one.
     * 
     * 
     * @return pointer to the attached drawing_area, NULL if none.
     */
    drawing_area * attached_drawing_area () 
    {
        return draw_to; 
    }
    
    /** Detach (if needed) the drawing_area which was attached to this
     *  one.
     */ 
    void detach_drawing_area () 
    {
        draw_to = NULL;
    }
     
    /** 
     * Convert an SDL_Rect into a drawing_area.
     * 
     * @param r SDL_rect to convert.
     * 
     * @return drawing_area which has the same dimensions and location as r.
     */
    drawing_area& drawing_area::operator = (SDL_Rect& r); 

    /** 
     * Gets the real parameters of this drawing_area.
     * 
     * 
     * @return SDL_Rect which is the intersection of this drawing area and
     * all the drawing areas assigned to it.
     */
    SDL_Rect setup_rects (); 
    
private:
    /// drawing_area location and size.
    SDL_Rect rect;

    /// Attached drawing_area.
    drawing_area *draw_to; 

}; 


#endif
