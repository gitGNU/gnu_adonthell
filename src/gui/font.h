/*
   $Id$

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
#ifndef FONT_H_
#define FONT_H_

#include <string>  
#include <vector> 

#include <ft2build.h>
#include FT_FREETYPE_H
#include "types.h"
#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "gfx/image.h"


namespace gui
{

  /**
   * Designed to manipulate font TTF. You can load TTF font, you set up your configuration (size, dpi, color), next call build ()  to apply changement.
   * 
   *
   */ 
  class font
    {
      public : 

	/** Constructor initialize some variables : 
	 *     - Set color to black color
	 *     - Set size to 12
	 *     - Set dpi to 72 
	 */ 
	font (); 


      /** Destructor
       *     - Free memory used by font
       */
      ~font (); 
    
     
      /** Load a TTF font file,  before load free memory
       * Important : this method doesn't call build ()  method 
       *
       * @param filename the TTF file
       * @return true : if open successful,  false otherwise
       */
      bool load (const std::string & filename); 

    
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


      /** Set color
       *
       * @param col : color 
       */
      void set_color (u_int32 col); 
    
    
      /** Information on the TTF font, write font information in console
       *
       */
      void info ();  
    
 
      /**
       * Build the font with cur configuration (size, color, dpi) 
       * After build, you can draw some text;) 
       */
      bool build ();
    
     

      /** 
       * Draw text at x, y position on target,  if target is NULL draw text on the screen
       * Important,  your font must be build
       *
       * @param text : text to draw
       * @param x : x position
       * @param y : y position
       * @param target : draw on target,  if not target,  draw on the screen
       */
      void draw (const std::string & text, s_int32 x, s_int32 y, gfx::drawing_area * da = NULL, gfx::surface * target = NULL);  

      
      /**
       * length_of calcul and return length of text
       * Important : if your text is "foo\nfoo" and length of "foo" is 12 px, this method return 24.
       *
       * @param text : text to calcul
       * @return length of the text in pixel
       */
      u_int32 get_length_of (const std::string & text);
  
      /**
       *
       */
      u_int16 get_size();

      private : 

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
};
#endif

