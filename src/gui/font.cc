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

#include <iostream>

#include "gfx/image.h"
#include "font.h"

using namespace gui;

int font::UTF8_to_UNICODE(u_int16 * unicode, const char *utf8, int len)
{
	int i, j;
	Uint16 ch;
	
	for ( i=0, j=0; i < len; ++i, ++j ) {
		ch = ((const unsigned char *)utf8)[i];
		if ( ch >= 0xF0 ) {
			ch  =  (Uint16)(utf8[i]&0x07) << 18;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 12;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		} else
		if ( ch >= 0xE0 ) {
			ch  =  (Uint16)(utf8[i]&0x3F) << 12;
			ch |=  (Uint16)(utf8[++i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		} else
		if ( ch >= 0xC0 ) {
			ch  =  (Uint16)(utf8[i]&0x3F) << 6;
			ch |=  (Uint16)(utf8[++i]&0x3F);
		}
		unicode[j] = ch;
	}
	unicode[j] = 0;

	return j;
}



font::font ()
{
    // by default no file,  so no valid font
    my_valid = false;  
    // init color
    set_color (0, 0, 0);   
    // set default size
    set_size (12);
    // set_default dpi
    set_dpi (72); 
}


bool font::load (const std::string & filename)
{
    int error; 
    close (); 
    
    // copy the new filename
    my_filename = filename; 
    
    my_valid = false; 
    
    // Initialise library
    error = FT_Init_FreeType( &my_library ); 
    if (error) 
    {
        std::cout << "FONT : Initialisation error!\n"; 
        return false;
    }
    
    // load TTF font in face
    error = FT_New_Face( my_library, my_filename.c_str () , 0, &my_face );
    if ( error == FT_Err_Unknown_File_Format )
    {
        std::cout << "FONT : Unknown font format\n";
        close (); 
        return false; 
    }
    else if ( error )
    {
        std::cout << "FONT : Error undefined\n";
        close (); 
        return false; 
    }

    // Check if the font is no scalable
    // WARNING : Implement in the futur
    if (my_face->num_fixed_sizes)
    {
        std::cout << "FONT : Fixed font not only supported\n"; 
        close ();
        return false; 
    }
    
    my_valid = true; 
    return true; 
}


inline void font::set_dpi (int i) { my_dpi = i; }

inline void font::set_size (int i) { my_size = i; }


bool font::build ()
{
  FT_Set_Char_Size( my_face, my_size << 6, my_size << 6, my_dpi, my_dpi);
  return true; 
}



void font::close ()
{
    if (!my_valid) return; 
    FT_Done_Face(my_face);
    FT_Done_FreeType(my_library); 
}


void font::copy_bitmap_to_image (u_int8 * bitmap_data, gfx::image * dest, s_int16 dx, s_int16 dy)
{
  u_int8 * pbmp = bitmap_data; 
  
  dest->lock ();
  
  u_int32 pixelcol = gfx::screen::display.map_color (my_r, my_g, my_b); 
  
  for (int j = 0; j < dest->height (); j++)
    for (int i = 0; i < dest->length () ; i++)
      {
	if (*pbmp > 128)  dest->put_pix (dx + i, dy +j, pixelcol); 
	else dest->put_pix (dx + i, dy + j, gfx::screen::trans_col ()); 
	pbmp++; 
      } 
  dest->unlock ();  
}

void font::info ()
{
    std::cout << "Filename : " << my_filename << std::endl;
    std::cout << "Number face : " << my_face->num_faces << std::endl; 
    std::cout << "Num_glyphs : " << my_face->num_glyphs << std::endl; 
    std::cout << "Fixed size : " << my_face->num_fixed_sizes << std::endl;  
    std::cout << "Num char map: " << my_face->num_charmaps << std::endl;
}


void font::draw (const std::string & text, s_int32 x,  s_int32 y, 
		 gfx::drawing_area * da, gfx::surface * target)
{
  FT_GlyphSlot  slot = my_face->glyph; /* the slot */
  u_int16 *unicode_text; /* the unicode pointer text */
  FT_UInt  glyph_index;
  int error;
  u_int32 pixelcol = gfx::screen::display.map_color (my_r, my_g, my_b);
  u_int8 * pbmp;
  
  /* get the surface */
  if (target == NULL) target = &(gfx::screen::display);
  
  /* translate to unicode */
  unicode_text = new u_int16[ (text.length()+1)*(sizeof *unicode_text) ];
  int len = UTF8_to_UNICODE(unicode_text, text.c_str(), text.length());
  
  s_int16 txx = x;
  s_int16 tyy = y;
  
  for (int n =0; n < len; n++)
    {
      /* retrieve glyph index from character code */
      glyph_index = FT_Get_Char_Index( my_face, unicode_text[n] );
      
      /* load glyph image into the slot (erase previous one) */
      error = FT_Load_Glyph( my_face, glyph_index, FT_LOAD_DEFAULT );
      if (error) continue;
      
      /* convert to an anti-aliased bitmap */
      error = FT_Render_Glyph( my_face->glyph, ft_render_mode_normal );
      if (error) continue;
      
      pbmp = slot->bitmap.buffer;
      for (int j = 0; j < slot->bitmap.rows; j++)
        for (int i = 0; i < slot->bitmap.width ; i++)
	  {
            if (*pbmp > 128) target->put_pix (txx + i, tyy + j - ((slot->metrics.horiBearingY)>>6)
, pixelcol); 
	    else target->put_pix (txx + i, tyy + j - ((slot->metrics.horiBearingY)>>6), 
				  gfx::screen::trans_col ());
            pbmp++; 
	  }
      
      /* must improve this */
      txx += (slot->metrics.horiAdvance>>6);
    }
  /* delete unicode_text */
  delete [] unicode_text;
}





u_int32 font::get_length_of (const std::string & text)
{
  u_int32 size = 0;
  FT_GlyphSlot  slot = my_face->glyph; /* the slot */
  u_int16 *unicode_text; /* the unicode pointer text */
  FT_UInt  glyph_index;
  int error;
  
  /* translate to unicode */
  unicode_text = new u_int16[ (text.length()+1)*(sizeof *unicode_text) ];
  int len = UTF8_to_UNICODE(unicode_text, text.c_str(), text.length());
  
  for (int n =0; n < len; n++)
    {
      /* retrieve glyph index from character code */
      glyph_index = FT_Get_Char_Index( my_face, unicode_text[n] );
      
      /* load glyph image into the slot (erase previous one) */
      error = FT_Load_Glyph( my_face, glyph_index, FT_LOAD_DEFAULT );
      if (error) continue;
      
      size += (slot->metrics.horiAdvance>>6);
    }
  
  delete [] unicode_text;
  return size; 
}


u_int16 font::get_size()
{
  return my_size;
}

font::~font ()
{
    close (); 
}

void font::set_color (u_int8 r, u_int8 g, u_int8 b)
{
  my_r = r; 
  my_g = g;
  my_b = b; 
}

void font::set_color (u_int32 col)
{
  my_r = col & 255; 
  my_g = (col >> 8) & 255; 
  my_b = (col >> 16) & 255;
}
