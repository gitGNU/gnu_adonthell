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
#ifndef TTF_H_
#define TTF_H_

#include <string>  
#include <map> 

#include "ft2build.h"
#include FT_FREETYPE_H
#include "types.h"
#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "gfx/image.h"


namespace gui {

  class ttf;

  class glyph_info 
    {
    public:
      glyph_info (ttf & ft, wchar_t ch);
      
      ~glyph_info ();

      bool is_ok ();
      
      u_int16 height();

      int my_minx;
      int my_miny;
      int my_maxx;
      int my_maxy;
      int my_yoffset;
      int my_advance;
      gfx::image * my_glyph;
    };
  
  class ttf
    {
    public:
      /**
       * Default constructor 
       */
      ttf ();
      
      /**
       * Destructor
       */
      ~ttf ();
      
      
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
      

      /**
       * @return the height of the font
       */
      u_int16 height ();
      
      
      /**
       * Display some font information
       */
      void info ();

      /**
       * The letter must be in UTF-16 !
       * Return the gfx::image which correspond at the letter in []
       */
      glyph_info & operator[](wchar_t letter);
	
      /**
       * @return the cursor
       */
      gfx::image & get_cursor ();

      /**
       * @return true if the letter is in the ttf
       */
      bool in_table (wchar_t letter);

      /* copy bitmap to image */
      void copy_bitmap_to_image (u_int8 * bitmap_data, gfx::image * dest, s_int16 dx = 0, s_int16 dy = 0); 

      friend class glyph_info;
    private:

      /* clean cache */
      void clean_cache ();
      

      /* close the library and face */
      void close (); 
      
      /**
       * create a cursor
       */
      void create_cursor ();

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
      
      u_int16 my_height;
      u_int16 my_ascent;
      u_int16 my_descent;
      u_int16 my_lineskip;
      u_int16 my_underline_offset;
      u_int16 my_underline_height;
      
      int my_style;
      
      /* color variable */
      u_int8 my_r, my_g, my_b;  
      
      /* font cache */
      std::map <u_int16, glyph_info *> my_cache;

      /* this is the cursor */
      gfx::image * my_cursor;
    };

}

#endif
