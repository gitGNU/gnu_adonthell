/*
   $Id$

   Copyright (C) 1999/2000/2001 Alexandre Courbot
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

    static bool dblmode;

    /**
     * The actual screen surface.
     * It is publicly available so you can do fast operations on the screen.
     * Manipulate it just as a classic surface.
     * 
     */
    static surface display; 

    /** Sets the video mode.
     *  @param nl X screen resolution.
     *  @param nh Y screen resolution.
     *  @param depth desired screen depth.
     */ 
    static void set_video_mode (u_int16 nl, u_int16 nh, u_int8 depth = 0, bool dbl = false, bool fscreen = false);

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
     * Totally clears the screen with black.
     * 
     */
    static void clear () 
    {
        display.fillrect (0, 0, display.length (), display.height (), 0x0); 
    }
    
    /** Ensure the framebuffer is copied to the physical screen.
     */ 
    static void show (); 

    /** Returns whether the current mode is fullscreen or windowed.
     *  @return
     *     - true: fullscreen.
     *     - false: windowed.
     */ 
    static bool is_fullscreen ()
    {
        return fullscreen_; 
    }

    /** Sets fullscreen/windowed mode.
     *  @param mode
     *     - true: fullscreen mode.
     *     - false: windowed mode.
     *  @return
     *    @li true if the operation succeed.
     *    @li false if the mode is already set, or the system doesn't support
     *        this mode.
     */ 
    static bool set_fullscreen (bool m); 
    
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
    /// Bytes per pixel.
    static u_int8 bytes_per_pixel_; 

    /// Transparent color.
    static u_int32 trans; 

    /// Whether fullscreen is on or not.
    static bool fullscreen_; 
}; 


#endif
