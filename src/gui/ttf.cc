/*
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
#include "ttf.h"

/* Handy routines for converting from fixed point */
#define FT_FLOOR(X)	((X & -64) / 64)
#define FT_CEIL(X)	(((X + 63) & -64) / 64)

using namespace gui;

ttf::ttf ()
{
    // by default no file,  so no valid font
    my_valid = false;  
    // init color
    set_color (0, 0, 0);   
    // set default size
    set_size (12);
    // set_default dpi 
    set_dpi (72); 
    //
    my_cursor = new gfx::image; 
}


ttf::~ttf()
{
  close ();
}


bool ttf::load (const std::string & filename)
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
    
    // load TTF font in my_face
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


bool ttf::build ()
{
  FT_Fixed scale;

  clean_cache ();
  FT_Set_Char_Size( my_face, my_size << 6, my_size << 6, my_dpi, my_dpi);
  
  /* get scalable font metrics */
  scale = my_face->size->metrics.y_scale;
  my_ascent =  FT_CEIL(FT_MulFix(my_face->bbox.yMax, scale));
  my_descent = FT_CEIL(FT_MulFix(my_face->bbox.yMin, scale));
  my_height = my_ascent - my_descent + 1;
  my_lineskip = FT_CEIL(FT_MulFix(my_face->height, scale));

  my_underline_offset = FT_FLOOR(FT_MulFix(my_face->underline_position, scale));
  my_underline_height = FT_FLOOR(FT_MulFix(my_face->underline_thickness, scale));
  if ( my_underline_height < 1 ) my_underline_height = 1;
  
  create_cursor ();
  return true; 
}

inline void ttf::set_dpi (int i) { my_dpi = i; }

inline void ttf::set_size (int i) { my_size = i; }


void ttf::create_cursor ()
{
  //create the cursor
  if ( my_cursor ) delete my_cursor;
  
  my_cursor = new gfx::image (my_size / 3, my_size );
  
  my_cursor->lock ();
  for (int i = 0; i < my_size; ++i)
    for (int j = 0; j < (my_size/3); ++j)
      my_cursor->put_pix (j,i, my_r, my_g, my_b);
  my_cursor->unlock ();
}

void ttf::close ()
{
    if (!my_valid) return; 
    FT_Done_Face(my_face);
    FT_Done_FreeType(my_library); 
    if (my_cursor) delete my_cursor;
    clean_cache ();
}




void ttf::info ()
{
    std::cout << "Filename : " << my_filename << std::endl;
    std::cout << "Number face : " << my_face->num_faces << std::endl; 
    std::cout << "Num_glyphs : " << my_face->num_glyphs << std::endl; 
    std::cout << "Fixed size : " << my_face->num_fixed_sizes << std::endl;  
    std::cout << "Num char map: " << my_face->num_charmaps << std::endl;
}



void ttf::set_color (u_int8 r, u_int8 g, u_int8 b)
{
  my_r = r; 
  my_g = g;
  my_b = b; 
}


u_int16 ttf::height ()
{
  return my_height;
}

void ttf::clean_cache ()
{
  //remove from memory all image *
  for (std::map<u_int16, glyph_info *> ::iterator it = my_cache.begin (); it != my_cache.end ();
       ++it)
    delete it->second;
  my_cache.clear ();
}


void ttf::copy_bitmap_to_image (u_int8 * bitmap_data, gfx::image * dest, s_int16 dx, s_int16 dy)
{
  u_int8 * pbmp = bitmap_data; 
  
  dest->lock ();  
  u_int32 pixelcol = gfx::screen::display.map_color (my_r, my_g, my_b); 
  
  for (int j = 0; j < dest->height (); j++)
    for (int i = 0; i < dest->length () ; i++)
      {
	if (*pbmp > 70)  dest->put_pix (dx + i, dy +j, pixelcol); 
	else dest->put_pix (dx + i, dy + j, gfx::screen::trans_col ()); 
	pbmp++; 
      } 
  dest->unlock ();  
  dest->set_mask (true);
  std::cout << "Copy to Bitmap done\n";
}


glyph_info & ttf::operator [] (wchar_t letter)
{
  // the first step is to find if the letter is in the cache.
  // if not in the cache try to see if the letter exist
  // if the letter exist add in the cache the new letter.
  //We must convert the letter!
  FT_UInt  glyph_index;
  glyph_index = FT_Get_Char_Index( my_face, letter);

  //  in_table (letter);
  
  if (glyph_index == 0) return (*this)[' '];
  
  std::map<u_int16, glyph_info *> ::iterator it = my_cache.find (glyph_index);
  
  if ( it != my_cache.end ()) return *(it->second);
  
  //nothing about the letter
  //try to find the letter in the TFF font !
  
  glyph_info * tmp = new glyph_info (*this, letter);
  if (!tmp->is_ok()) return (*this)[' '];
  
  my_cache[glyph_index] = tmp;
  
  //  std::cout << letter << "  " << slot->bitmap_left << "  " << slot->bitmap_top << std::endl;
  //  std::cout << my_size << "  " << slot->bitmap.rows << "  "  << slot->horiBearingY << "  " << slot->bitmap.rows + slot->horiBearingY <<  std::endl;
  
  return *tmp;
}


gfx::image & ttf::get_cursor ()
{
  return *my_cursor;
}

bool ttf::in_table (wchar_t letter)
{
  FT_UInt  glyph_index;
  glyph_index = FT_Get_Char_Index( my_face, letter);
  return (glyph_index != 0);
}

/*
void ttf::set_color (u_int32 col)
{
  my_r = col & 255; 
  my_g = (col >> 8) & 255; 
  my_b = (col >> 16) & 255;
}*/


glyph_info::glyph_info (ttf & ft, wchar_t ch) : my_glyph (NULL)
{
  FT_Face face = ft.my_face;
  FT_GlyphSlot glyph;
  FT_Glyph_Metrics* metrics;
  FT_Outline* outline;
  
  wchar_t index = FT_Get_Char_Index (face, ch);
  
  if (FT_Load_Glyph( face, index, FT_LOAD_DEFAULT )) return;
  
  glyph = face->glyph;
  metrics = &glyph->metrics;
  outline = &glyph->outline;

  my_minx = FT_FLOOR(metrics->horiBearingX);
  my_maxx = my_minx + FT_CEIL(metrics->width);
  my_maxy = FT_FLOOR(metrics->horiBearingY);
  my_miny = my_maxy - FT_CEIL(metrics->height);
  my_yoffset = ft.my_ascent - my_maxy + ft.my_descent;
  my_advance = FT_CEIL(metrics->horiAdvance);

  if (FT_Render_Glyph( face->glyph, ft_render_mode_normal )) return;
  
  my_glyph = new gfx::image (glyph->bitmap.width, glyph->bitmap.rows);
  my_glyph->set_mask (true);
  std::cout << "New Image: " << my_glyph->length() << " " << my_glyph->height () << "\n";
  
  ft.copy_bitmap_to_image ( glyph->bitmap.buffer, my_glyph);
}


bool glyph_info::is_ok ()
{
  return (my_glyph != NULL);
}

glyph_info::~glyph_info ()
{
  if (my_glyph) delete my_glyph;
}


u_int16 glyph_info::height()
{
  if (my_glyph) return my_glyph->height();
  return 0;
}
