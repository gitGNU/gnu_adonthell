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

#include "types.h"
#include "prefs.h"

class screen
{
  static u_int16 width;
  static u_int16 height;
  static u_int32 SDL_flags; // Used for fullscreen and other flags

  static void init_gfx_buffers();
  // Called by init_display to init the drawable fx buffers.
 public:

  static u_int8 bytes_per_pixel;
  static u_int8 frames_to_do;
  static u_int32 trans;
  static u_int32 trans_pix;

  static SDL_Surface * vis;

  static void set_video_mode(u_int16 w, u_int16 h, config * myconfig=NULL);
  static void init_display(config *myconfig);
  static void set_fullscreen(bool mode);
  static bool get_fullscreen();
  static void show();
  // update the screen.

  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color);
  // Draw a box of a given color.

  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
  // FIXME: Used in mapeditor only, move it somewhere else

  static void mouse_cursor_off();
  static void mouse_cursor_on();
  // Mouse cursor display functions.

  static u_int16 get_width();
  static u_int16 get_height();
};

#endif
