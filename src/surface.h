/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   surface.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the surface class.
 * 
 * 
 */


#ifndef SURFACE_H_
#define SURFACE_H_


#include "drawable.h"



/**
 * Class where drawables can actually be drawn to.
 * Another name for a surface could eventually be "pixmap". A surface
 * is nothing more than an array of pixels where drawables can put their
 * %image to. This class has only two purposes: to group methods shared by
 * image and screen, and to ensure that every drawing operation can be performed
 * on the screen or on an image.
 *
 * Every surface has two special parameters:
 *  @li a mask parameter, indicating whether the transparent color (which hex
 *      triplet is 0xFF00FF, and you can get with screen::trans_col ()) should
 *      be drawn or not.
 *  @li an alpha parameter, indicating the level of translucency of the surface.
 *      it's range go from 0 to 255, 0 being totally invisible and 255 totally
 *      opaque.
 * 
 */
class surface : public drawable
{
public:

    /**
     * Default constructor.
     * The surface will be totally empty, that is
     * (0, 0) sized, no mask, alpha value of 255 (opaque).
     * 
     */ 
    surface ();

    /**
     * Destructor.
     * 
     */
    virtual ~surface (); 
    

    /**
     * @name Settings.
     * These methods sets the parameters of the surface.
     * 
     */ 
    //@{
        

    /** 
     * 
     * Returns whether a surface is masked or not.
     * 
     * @return true if the surface is masked, false if it isn't.
     */
    bool is_masked () const
    {
        return mask_on; 
    }

    /** 
     * Sets the mask parameter of the surface.
     * 
     * @param m true if the surface should be masked, false otherwise.
     */
    void set_mask (bool m); 
    
    /** 
     * Returns the alpha value of the surface.
     * 
     * 
     * @return the alpha value of the surface.
     */
    u_int8 alpha () const
    {
        return alpha_; 
    }

    /** 
     * Sets the alpha value of the surface.
     * 
     * @param a The new alpha value for this surface.
     */
    void set_alpha (u_int8 a); 
    
    //@}


    /**
     * @name Drawing Methods.
     * 
     */

    //@{

    
    /** 
     * Draw the surface.
     * 
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param da_opt optional drawing_area to use during the drawing operation.
     * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
     */ 
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        draw (x, y, 0, 0, length (), height (), da_opt, target); 
    }

#ifndef SWIG
    /** 
     * Draw a part of the surface.
     * 
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param sx X position where to start drawing from this image.
     * @param sy Y position where to start drawing from this image.
     * @param sl length of the part of this image to draw.
     * @param sh height of the part of this image to draw.
     * @param da_opt optional drawing_area to use during the drawing operation.
     * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
     *
     * @attention Not accessible from Python. Use draw_part from Python instead.
     * @sa draw_part () 
     *
     */
    void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
               u_int16 sh, const drawing_area * da_opt = NULL,
               surface * target = NULL) const;
#endif

    /**
     * Synonym of draw () to guarantee its access from Python.
     *
     * @sa draw () 
     * 
     */ 
    void draw_part (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                    u_int16 sh, const drawing_area * da_opt = NULL,
                    surface * target = NULL) const
    {
        draw (x, y, sx, sy, sl, sh, da_opt, target); 
    }
    
    /** 
     * Fills an area of the surface with a given color.
     *
     * The color you pass to this function MUST come from a game's function
     * (like surface::get_pix () or screen::trans_col ()), because of the
     * screen depth dependant value of the col argument.
     * 
     * @param x X position where to fill.
     * @param y Y position where to fill.
     * @param l length of the area to fill.
     * @param h height of the area to fill.
     * @param col color to fill the surface with.
     * @param da_opt optionnal drawing_area to use during the fill operation.
     *
     */
    void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                   u_int32 col, drawing_area * da_opt = NULL);
    
#ifndef SWIG
    /** 
     * Fills an area of the surface with a given color.
     *
     * This function is independant of the screen depth. You just give
     * the red, green and blue triplets of the color you want to fill with.
     * 
     * @param x X position where to fill.
     * @param y Y position where to fill.
     * @param l length of the area to fill.
     * @param h height of the area to fill.
     * @param r red value of the color to fill with.
     * @param g green value of the color to fill with.
     * @param b blue value of the color to fill with.
     * @param da_opt optionnal drawing_area to use during the fill operation.
     *
     * @attention Not accessible from Python. Use fillrect_rgb from Python instead.
     * @sa fillrect_rgb ()  
     */
    void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int8 r,
                   u_int8 g, u_int8 b, drawing_area * da_opt = NULL)
    {
        fillrect (x, y, l, h, SDL_MapRGB (vis->format, r, g, b), da_opt);
    }
#endif

    /**
     * Synonym of fillrect () to guarantee its access from Python.
     * 
     * @sa fillrect () 
     */ 
    void fillrect_rgb (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int8 r,
                       u_int8 g, u_int8 b, drawing_area * da_opt = NULL)
    {
        fillrect (x, y, l, h, r, g, b, da_opt); 
    }


    //@}

    /**
     * @name Pixel manipulation Methods.
     * Use these methods to directly and quickly manipulate
     * pixels from a surface.
     * 
     */ 
    //@{
         
    
    /** 
     * Locks the surface.
     * Sometimes you may want to access directly the pixels of a surface. This
     * can be done with the get_pix ()  and put_pix () methods, thus you must
     * ABSOLUTELY lock the surface before doing so. This function is made for
     * that. Note that using get_pix () or put_pix () without locking the surface
     * may result in unpredictable behavior, crashes included.
     * 
     */
    void lock () const; 
    
    /** 
     * Unlock the surface after you've worked on it's pixels with the
     * get_pix () and put_pix () methods.
     * 
     */
    void unlock () const;
    
    /** 
     * Puts a pixel of a given color.
     *
     * The col parameter is specific to the current screen depth,
     * and must come from a game's function like get_pix or
     * screen::trans_col ().
     * 
     * @param x X position of the pixel to change.
     * @param y Y position of the pixel to change.
     * @param col color to put.
     */
    void put_pix (u_int16 x, u_int16 y, u_int32 col); 

#ifndef SWIG
    /** 
     * Puts a pixel of a given color.
     *
     * The r, g and b parameters are the hex triplets of the color
     * to put.
     * 
     * @param x X position of the pixel to change.
     * @param y Y position of the pixel to change.
     * @param r red value of the color to put.
     * @param g green value of the color to put.
     * @param b blue value of the color to put.
     *
     * @attention Not accessible from Python. Use put_pix_rgb from Python instead.
     * @sa put_pix_rgb ()  
     *
     */ 
    void put_pix (u_int16 x, u_int16 y, u_int8 r, u_int8 g, u_int8 b) 
    {
        put_pix (x, y, SDL_MapRGB (vis->format, r, g, b)); 
    }
#endif
    
    /**
     * Synonym of put_pix () to guarantee its access from Python.
     *
     * @sa put_pix () 
     */ 
    void put_pix_rgb (u_int16 x, u_int16 y, u_int8 r, u_int8 g, u_int8 b) 
    {
        put_pix (x, y, r, g, b); 
    }
    
    /** 
     * Gets a pixel from the surface.
     *
     * The col parameter is specific to the current screen depth,
     * and can be used with functions like put_pix ().
     * 
     * @param x X position of the pixel to change.
     * @param y Y position of the pixel to change.
     * @param col returned color.
     */
    void get_pix (u_int16 x, u_int16 y, u_int32& col) const; 

#ifndef SWIG
    /** 
     * Gets a pixel from a surface.
     *
     * The returned r, g and b values are the hex triplets of the color.
     * 
     * @param x X position of the pixel to change.
     * @param y Y position of the pixel to change.
     * @param r red value of the color.
     * @param g green value of the color.
     * @param b blue value of the color.
     *
     * @attention Not accessible from Python. Use get_pix_rgb from Python instead.
     * @sa get_pix_rgb ()   
     */
    void get_pix (u_int16 x, u_int16 y, u_int8& r, u_int8& g, u_int8& b) const
    {
        u_int32 col;
        get_pix (x, y, col); 
        SDL_GetRGB (col, vis->format, &r, &g, &b); 
    }
#endif
    
    /**
     * Synonym of get_pix () to guarantee its access from Python.
     *
     * @sa get_pix () 
     */ 
    void get_pix_rgb (u_int16 x, u_int16 y, u_int8 r, u_int8 g, u_int8 b) const
    {
        get_pix (x, y, r, g, b); 
    }

    //@}
    
    
#ifndef SWIG
    /**
     * Surface copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */
    surface& operator = (surface& src); 
#endif

    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (surface& src) 
    {
        *this = src; 
    }
         
    SDL_Surface *vis;
protected: 
    /**
     * The actual surface.
     * 
     */ 
 
    /** 
     * Resize this surface. All the content will be lost.
     * 
     * @param l new length.
     * @param h new height.
     */
    void resize (u_int16 l, u_int16 h);

    /** 
     * Resets the surface to it's initial state, that is totally
     * empty.
     * 
     */
    void clear (); 

    /**
     * Must be set to true when you change the surface
     * by something else than class surface operations.
     * 
     */ 
    mutable bool changed; 
     
private:

    /**
     * Forbid value passing.
     * 
     */ 
    surface (surface & src); 
    
    /// SDL_Rects used in every blitting function.
    static SDL_Rect srcrect, dstrect; 

    /// Mask
    bool mask_on; 

    /// Alpha value
    u_int8 alpha_;

    /// Set at true by screen's contructor to prevent screen surface from
    /// being deleted two times.
    bool not_screen; 

    /// Used internally for blitting operations with drawing_areas.
    void setup_rects (u_int16 x, u_int16 y, const drawing_area * draw_to) const
    {
        setup_rects (x, y, 0, 0, length (), height (), draw_to); 
    }
    
    /// Used internally for blitting operations with drawing_areas.
    void setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                      u_int16 sl, u_int16 sh, const drawing_area * draw_to) const; 

#ifndef SWIG
    friend class screen;
#endif
};

#endif
