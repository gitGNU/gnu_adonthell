/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/** @file screen.h
 *  @brief Declares the screen class.
 *  @todo drawbox, etc... must be capable of converting the argument given
 *        color from a hex triplet to the %screen format using SDL_MapRGB.
 */ 

#ifndef _screen_h
#define _screen_h
  
#include "drawing_area.h"

/** The %game slow down if there are more than FTD_LIMIT numbers of frames
 *  to calculate before displaying, in which case it calculates exactly
 *  FTD_LIMIT frames.
 */ 
#define FTD_LIMIT 100



/** Screen access is made through this class.
 *  This static class sets video modes, flush the frame buffer to the physical
 *  screen and make abstraction of the real screen depth to ease the graphic
 *  programmer's task.
 */
class screen
{
public:

    /** Sets the video mode.
     *  @param nl X screen resolution.
     *  @param nh Y screen resolution.
     */ 
    static void set_video_mode (u_int16 nl, u_int16 nh);

    /** Sets fullscreen/windowed mode.
     *  @param mode
     *     - true: fullscreen mode.
     *     - false: windowed mode.
     */ 
    static void set_fullscreen (bool mode);

    /** Returns whether the current mode is fullscreen or windowed.
     *  @return
     *     - true: fullscreen.
     *     - false: windowed.
     */ 
    static bool is_fullscreen ();

    /** Reinits the frame counter.
     *  After a period when the screen hasn't been updated (data loading, ...)
     *  you will want to call this function to reset the frame counter, otherwise
     *  next time you'll update () something it will assume you've skipped some frames.
     */ 
    static void init_frame_counter ();

    /** Copy the framebuffer to the physical screen and udpate the frame counter
     */ 
    static void show ();

    /** Draw a box of a given color.
     *  @param x X position.
     *  @param y Y position.
     *  @param w box's length.
     *  @param h box's height.
     *  @param color color in RGB format.
     *  @param da_opt drawing_area assigned to this operation.
     */ 
    static void drawbox (u_int16 x, u_int16 y, u_int16 w, u_int16 h,
                         u_int32 color, drawing_area * da_opt = NULL);

    /** Turns the screen into black.
     */ 
    static void clear ();

    
    /// @todo Used in mapeditor only, move it somewhere else.
    static void makesquare (u_int16 px, u_int16 py, u_int16 factor);

    /** Sets whether the mouse cursor should be visible or not.
     *  @param b
     *     - true: set cursor visible.
     *     - false: set cursor invisible.
     */ 
    static void set_mouse_cursor_visible (bool b);

    /** Returns whether the mouse cursor is visible or not.
     *  @return
     *     - true: mouse cursor is visible.
     *     -false: mouse cursor isn't visible.
     */ 
    static bool is_mouse_cursor_visible ()
    {
        return cur_visible;
    }

    /** Returns the length of the screen.
     *  @return length of the screen.
     */ 
    static u_int16 length ()
    {
        return l;
    }

    /** Returns the height of the screen.
     *  @return height of the screen.
     */ 
    static u_int16 height ()
    {
        return h;
    }

    /** Returns the translucent color in %screen's depth format
     *  For manipulation on images that will only be displayed, this is
     *  the right function to call for getting the translucent color.
     *  @return the translucent color in %screen's depth format.
     */ 
    static u_int32 trans_col ()
    {
        return trans;
    }

    /** Returns the translucent color in RGB format
     *  For manipulation on images that will be displayed and saved, this is
     *  the right function to call for getting the translucent color.
     *  @return the translucent color in RGB format.
     */  
    static u_int32 trans_col_rgb ()
    {
        return trans_rgb;
    }

    /** Returns the number of frames to calculate before displaying.
     *  Use this function to know how many update () you should perform
     *  on your objects before actually drawing (draw ()) them on the screen.
     *  @return the number of frames to perform before drawing and calling
     *          show ().
     */ 
    static u_int8 frames_to_do ()
    {
        return ftd;
    }
    
private:
    /// Length.
    static u_int16 l;

    /// Height.
    static u_int16 h; 

    static u_int32 SDL_flags;	// Used for fullscreen and other flags

    static const u_int32 CYCLE_LENGTH;	// Length of a game cycle in milliseconds 

    /// Frames to do.
    static u_int8 ftd;

    /// Timers used to calculate the delay between 2 show () calls.
    static u_int32 timer1, timer2;

    /// Mouse cursor visite or not.
    static bool cur_visible;
    
    /// Screen depth in bytes per pixel.
    static u_int8 bytes_per_pixel;

    /// Translucent color in %game's format.
    static u_int32 trans;

    /// Translucent color in RGB format.
    static u_int32 trans_rgb;
    
    /// Actual screen.
    static SDL_Surface *vis;
    
#ifndef SWIG
    friend class drawable; 
    friend class image;
#endif
};

#endif
