/*
   $Id$

   Copyright (C) 1999/2000/2001/2004 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   screen.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the screen class.
 * 
 * 
 */
 
#ifndef SCREEN_H_
#define SCREEN_H_

#include "surface.h"
#include <string>


#ifndef SWIG
using namespace std; 
#endif


/** Screen access is made through this class.
 *  This static class sets video modes, flush the frame buffer to the physical
 *  screen and make abstraction of the real screen depth to ease the graphic
 *  programmer's task.
 */
class screen
{
public:

	static void cleanup();

    static SDL_Renderer *get_renderer()
    {
        return Renderer;
    }

    static u_int32 format()
    {
        return Window != NULL ? SDL_GetWindowPixelFormat(Window) : SDL_PIXELFORMAT_UNKNOWN;
    }

    /**
     * The actual screen surface.
     * It is publicly available so you can do fast operations on the screen.
     * Manipulate it just as a classic surface.
     * 
     */
    static surface display; 

    /** Initializes the video subsystem and creates the required resources.
     *  @param nl X screen resolution.
     *  @param nh Y screen resolution.
     *  @param depth desired screen depth.
     *  @param display the display to use.
     *  @param screen_mode whether to start in window or fullscreen mode
     *  @return true on success, false otherwise.
     */ 
    static bool init (u_int16 nl, u_int16 nh, u_int8 depth, u_int8 display, u_int8 screen_mode);

    /** Returns the length of the screen.
     *  @return length of the screen.
     */  
    static u_int16 length () 
    {
        return display.length (); 
    }

    /** Returns the height of the screen.
     *  @return height of the screen.
     */ 
    static u_int16 height () 
    {
        return display.height ();
    }

    /** Returns the screen depth, in bytes per pixel.
     *  @return screen depth,  in bytes per pixel.
     */ 
    static u_int8 bytes_per_pixel () 
    {
        return bytes_per_pixel_; 
    }

    /** Returns the translucent color in %screen's depth format.
     *  For manipulation on images that will only be displayed, this is
     *  the right function to call for getting the translucent color.
     *  @return the translucent color in %screen's depth format.
     */ 
    static u_int32 trans_col ()
    {
        return trans;
    }

    /**
     * Scale factor of the screen.
     */
    static u_int8 scale ()
    {
    	return scale_;
    }

    /**
     * X offset of the viewport
     */
    static u_int16 offset_x()
    {
    	return offset_x_;
    }

    /**
     * Y offset of the viewport
     */
    static u_int16 offset_y()
    {
    	return offset_y_;
    }

    /** 
     * Totally clears the screen with black.
     * 
     */
    static void clear () 
    {
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(Renderer);
    }
    
    /** Ensure the framebuffer is copied to the physical screen.
     */ 
    static void show ()
    {
        SDL_RenderPresent(Renderer);
    }

    /** Returns whether the current mode is fullscreen or windowed.
     *  @return
     *     - 0: windowed mode.
     *     - 1: letterbox mode
     *     - 2: fullscreen mode
     */ 
    static u_int8 mode ()
    {
        return mode_; 
    }

    /** Sets fullscreen/windowed mode.
     *  @param mode
     *     - 0: windowed mode.
     *     - 1: letterbox mode
     *     - 2: fullscreen mode
     *  @return
     *    @li true if the operation succeed.
     *    @li false if the mode is already set, or the system doesn't support
     *        this mode.
     */ 
    static bool set_fullscreen (const u_int8 & m);
    
    /** 
     * Returns information about the current screen settings,
     * suitable for being displayed to the user.
     * 
     * 
     * @return printable information about the current screen settings.
     */
    static string info (); 

    /** 
     * Make a nice transition effect.
     * 
     * @param i advancement of the transition (finished when i == screen::length () / 2)
     */
    static void transition (u_int16 i); 

private:

	static u_int8 get_scale_for_display(u_int8 screen, u_int16 nl, u_int16 nh);
	static void update_scale();

	/// Bytes per pixel.
    static u_int8 bytes_per_pixel_; 

    /// Transparent color.
    static u_int32 trans; 

    /// Whether fullscreen is on or not.
    static u_int8 mode_;
    
    /// The output window
    static SDL_Window *Window;

    /// the render target
    static SDL_Renderer *Renderer;

    /// scale factor of the screen
    static u_int8 scale_;

    /// x offset of viewport
    static u_int16 offset_x_;

    /// y offset of viewport
    static u_int16 offset_y_;
};


#endif
