/*
  (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   font.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the font TFF class.
 * 
 * 
 */
#ifndef TTF__H_
#define TTF__H_

#include <string>  
#include <vector> 

#include <ft2build.h>
#include FT_FREETYPE_H
#include "types.h"
#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "gfx/image.h"


class win_ttf
{
 public:
  /**
   * Default constructor 
   */
  win_ttf ();

  /**
   * Destructor
   */
  ~win_ttf ();

  
  /**
   * Load a TTF file. But do not a build !
   */  
  bool load (const std::string & filename);
  

  /**
   * Build the font with cur configuration (size, color, dpi) 
   * After build, you can draw some text;) 
   */
  bool build ();

  /** Set dots per inch
   * You must set to 72 or 96,  Why ? see documentation on www.freetype.org
   *
   * @param dots per inch
   */ 
  inline void set_dpi (int);
  
  
  /** Set police size
   *
   * @param font size 
   */
  inline void set_size (int size); 
  
  
  /** Set color
   *
   * @param r : red color
   * @param g : green color
   * @param b : blue color
   */
  void set_color (u_int8 r, u_int8 g, u_int8 b);
  

 
  private:
  

  /* copy bitmap to image */
  void copy_bitmap_to_image (u_int8 * bitmap_data, gfx::image * dest, s_int16 dx, s_int16 dy); 
  
  /* close the library and face */
  void close (); 
  
  int UTF8_to_UNICODE(u_int16 * unicode, const char *utf8, int len);
  
  /* filename of the font */
  std::string my_filename; 
  
  /* valid font,  true if valid */
  bool my_valid; 
  
  /* library FT */
  FT_Library my_library; 
  
  /* FT_Face */
  FT_Face my_face;
  
  /* dpi, dots-per-inch */
  int my_dpi;
  
  /* size */
  u_int16 my_size;
  
  /* color variable */
  u_int8 my_r, my_g, my_b;  
};


#endif
