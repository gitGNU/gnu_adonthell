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

#ifndef _image_h
#define _image_h

#include "screen.h"

class drawing_area
{
 public:
  u_int16 x;
  u_int16 y;
  u_int16 length;
  u_int16 height;

  drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph);
  bool is_x_in(s_int16 px);
  bool is_y_in(s_int16 py);
  bool is_point_in(s_int16 px, s_int16 py);
  friend class image;
};

class image
{
  static SDL_Rect sr,dr;
 protected:

  SDL_Surface * data;
  drawing_area * draw_to;

  inline void get_rects(s_int16 x, s_int16 y);
  inline u_int32 get_pix(u_int16 x, u_int16 y);
  inline void put_pix(u_int16 x, u_int16 y, u_int32 col);

 public:
  u_int16 length, height;
  u_int8 bytes_per_pixel;
  bool mask_on;
  bool trans_on;
  
  image();
  image (u_int16 l, u_int16 h);
  ~image();
  void clear();
  void resize(u_int16 l, u_int16 h);
  s_int8 get(SDL_RWops * file);
  s_int8 get(char * file);
  s_int8 load(char * fname);
  void assign_drawing_area(drawing_area * da);
  void detach_drawing_area();
  void putbox (s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void putbox_trans (s_int16 x, s_int16 y, u_int8 alpha, 
		     drawing_area * da_opt=NULL);
  void putbox_mask_trans (s_int16 x, s_int16 y, u_int8 alpha, 
			  drawing_area * da_opt=NULL);
  image * zoom(u_int16 pixx, u_int16 pixy);
  image * brightness(u_int16 cont);



  // Need to validate these ones!
  void putbox_tile_img(image * source);
  void putbox_img(image * source, u_int16 x, u_int16 y);
  void putbox_mask_img (image * source, u_int16 x, u_int16 y);
  void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			u_int16 bh, u_int16 xo, u_int16 yo);
};

#endif
