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

#ifndef _screen_h
#define _screen_h

// FIXME: drawbox, etc... must be capable of converting the argument given
// color from a hex triplet to the screen format using SDL_MapRGB

#include "drawing_area.h"

#define FTD_LIMIT 100

class screen
{
 public:
  static void set_video_mode(u_int16 nl, u_int16 nh);
  static void set_fullscreen(bool mode);
  static bool is_fullscreen();
  static void init_frame_counter();
  static void show();
  // update the screen.

  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color, drawing_area * da_opt=NULL);
  // Draw a box of a given color.

  static void clear();

  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
  // FIXME: Used in mapeditor only, move it somewhere else

  static void set_mouse_cursor_visible(bool b);
  static bool is_mouse_cursor_visible() {return cur_visible;}
  // Mouse cursor display functions.

  static u_int16 length() { return l;}
  static u_int16 height() { return h;}

  static u_int32 trans_col() {return trans;}
  static u_int32 trans_col_rgb() {return trans_rgb;}

  static u_int8 frames_to_do() { return ftd; }

 private:
  static u_int16 l;
  static u_int16 h;
  static u_int32 SDL_flags;         // Used for fullscreen and other flags
  static const u_int32 CYCLE_LENGTH;// Length of a game cycle in milliseconds 
  static u_int8 ftd;
  static u_int32 timer1, timer2;
  static bool cur_visible;

  static u_int8 bytes_per_pixel;
  static u_int32 trans;
  static u_int32 trans_rgb;
  
  static SDL_Surface * vis;
  
#ifndef SWIG
  friend class image;
#endif
};

#endif
