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
#include <zlib.h>
#include "pnm.h"

class drawing_area : public SDL_Rect
{
  drawing_area * draw_to;
  SDL_Rect get_rects();
 public:

  drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph);
  drawing_area();
  void resize(u_int16 dl, u_int16 dh);
  void move(u_int16 dx, u_int16 dy);
  bool is_x_in(s_int16 px);
  bool is_y_in(s_int16 py);
  bool is_point_in(s_int16 px, s_int16 py);
  void assign_drawing_area(drawing_area * da);
  void detach_drawing_area();
  drawing_area &operator = (const SDL_Rect & r);
  drawing_area operator + (drawing_area & da);

  friend class image;
};

class image
{
  static SDL_Rect sr,dr;
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif
 protected:

#ifdef _EDIT_
  void * simpledata;
#endif
  drawing_area * draw_to;

  inline void get_rects(s_int16 x, s_int16 y);

 public:
  SDL_Surface * data;
  u_int16 length, height;
  u_int8 bytes_per_pixel;
  bool mask_on;
  u_int8 alpha;

  image &operator =(const image &im);
  void init();
  image();
  image (u_int16 l, u_int16 h);
  ~image();
  void clear();
  void resize(u_int16 l, u_int16 h);
  u_int16 get_length();
  u_int16 get_height();

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
  s_int8 get_raw(gzFile file);
  s_int8 load_raw(const char * fname);
  s_int8 get_pnm(SDL_RWops * file);
  s_int8 load_pnm(const char * fname);
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(char * fname);
  s_int8 put_raw(gzFile file);
  s_int8 save_raw(char * fname);
  s_int8 put_pnm(SDL_RWops * file);
  s_int8 save_pnm(char * fname);
#endif
  void assign_drawing_area(drawing_area * da);
  void detach_drawing_area();
  bool get_mask();
  void set_mask(bool m);
  u_int8 get_alpha();
  void set_alpha(u_int8 t);
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void putbox (s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void putbox_trans (s_int16 x, s_int16 y, u_int8 a, 
		     drawing_area * da_opt=NULL);
  void putbox_mask_trans (s_int16 x, s_int16 y, u_int8 a, 
			  drawing_area * da_opt=NULL);
  void get_from_screen(s_int16 x, s_int16 y);
  void zoom(image * src);
  void reverse_lr(image * src);
  void reverse_ud(image * src);
  void brightness(image * src, u_int16 cont);

  void putbox_tile_img(image * source);
  void putbox_img(image * source, u_int16 x, u_int16 y);
  void putbox_mask_img (image * source, u_int16 x, u_int16 y);
  void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			u_int16 bh, u_int16 xo, u_int16 yo);
  u_int32 get_pix(u_int16 x, u_int16 y);
  void put_pix(u_int16 x, u_int16 y, u_int32 col);
};

#endif
