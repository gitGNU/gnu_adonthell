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

#ifndef _gfx_h
#define _gfx_h

#include "types.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class screen
{
  static u_int16 screenwidth;
  static u_int16 screenheight;
  static u_int8 frames_to_do;
  static u_int8 bytes_per_pixel;
  static u_int8 sizefactor;
  static u_int32 trans;
 public:

  static SDL_Surface * vis;
  static SDL_Surface * getbuffer;

  static void set_video_mode(u_int16 w, u_int16 h, u_int8 sf=1);
  // Set a video mode. sf is the size factor of all the drawables, set it to
  // 2 if you want to double the drawables size.
  // This function should only be used in very special cases, like editing
  // tools.
  static void init_display(u_int8 vidmode = 0);
  // Display init for the game. If vidmode != 0 the screen and drawable size
  // will be doubled by a factor of vidmode.
  static void show();
  // update the screen.
  static u_int8 get_bytes_per_pixel();
  static u_int8 get_sizefactor();
  static u_int32 get_trans();
  static u_int8 get_frames_to_do();
  static u_int8 get_screenwidth();
  static u_int8 get_screenheight();
  // Protected variable access.
  static void init_gfx_buffers();
  // Called by init_display to init the drawable fx buffers.
  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color);
  // Draw a box of a given color.
  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
  // FIXME: Used in mapeditor only, move it somewhere else
  static void mouse_cursor_off();
  static void mouse_cursor_on();
  // Mouse cursor display functions.
};

class image
{
 protected:
  u_int16 lenght, height;
  static u_int8 bytes_per_pixel, sizefactor;
  static SDL_Rect sr,dr;

  SDL_Surface * data;
  bool mask_on;
  bool trans_on;

 public:
  image();
  ~image();
  u_int16 get_lenght();
  u_int16 get_height();
  static void set_sizefactor(u_int8 x);
  static u_int8 get_sizefactor();
  void gfxrealloc(u_int32 l);
  void putbox (u_int16 x, u_int16 y);
  void putbox_mask (u_int16 x, u_int16 y);
  void putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
		    u_int16 xo, u_int16 yo);
  void putbox_trans (u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo);
  void putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			  u_int16 bh, u_int16 xo, u_int16 yo, 
			  u_int8 alpha);
  void putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			       u_int16 xo, u_int16 yo, u_int8 alpha);
  void resize(u_int16 l, u_int16 h);
  void size(u_int16 l, u_int16 h);
  // Same as the top seen putbox functions, but act on the image's data instead
  // of the screen. The first parameter is the image to draw on the image's 
  // data
  void putbox_tile_img(image * source);
  void putbox_img(image * source, u_int16 x, u_int16 y);
  void putbox_mask_img (image * source, u_int16 x, u_int16 y);
  void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			u_int16 bh, u_int16 xo, u_int16 yo);
  void putbox_font_img (void * source, u_int16 xo, u_int16 totlen);
  void putbox_trans_img (image * source, u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_mask_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw,
			     u_int16 bh, u_int16 xo, u_int16 yo);
  void putbox_mask_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha);
  void putbox_part_trans_img (image * source, u_int16 x, u_int16 y, u_int16 bw,
 			      u_int16 bh, u_int16 xo, u_int16 yo, 
			      u_int8 alpha);
  void putbox_mask_part_trans_img(image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh, u_int16 xo, 
				  u_int16 yo, u_int8 alpha);
  s_int8 get(FILE * file);
  s_int8 get(char * file);
  s_int8 load(char * fname);
  s_int8 simpleget(FILE * file);
  s_int8 simpleload(char * fname);
  void adapttodepth();
  friend void screen::init_gfx_buffers();
};

class sprite
{
  image * frame;
  u_int8 nbr_of_frames;          // u_int16!!
  u_int8 counterlimit;
  u_int8 currentframe;
  u_int8 alpha;
 public:
  sprite();
  ~sprite();
  void get(FILE * file);
};

#endif
