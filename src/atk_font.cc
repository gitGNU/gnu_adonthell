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
#include "surface.h"
#include "image.h"
#include "screen.h"
#include "atk_font.h"


atk_font::atk_font ()
{
    // by default no file,  so no valid font
    valid = false;  

    // init color
    set_color (0, 0, 0);   

    // set default size
    set_size (12);

    // set_default dpi
    set_dpi (72); 
}


bool atk_font::load (const std::string & filename)
{
    int error; 
    
    close (); 
    free_vector (); 
    
    // copy the new filename
    this->filename = filename; 
    
    valid = false; 
    
    // Initialise library
    error = FT_Init_FreeType( &library ); 
    if (error) 
    {
        std::cout << "ATK_FONT : Initialisation error!\n"; 
        return false;
    }
    
    // load TTF font in face
    error = FT_New_Face( library, filename.c_str () , 0, &face );
    if ( error == FT_Err_Unknown_File_Format )
    {
        std::cout << "ATK_FONT : Unknown font format\n";
        close (); 
        return false; 
    }
    else if ( error )
    {
        std::cout << "ATK_FONT : Error undefined\n";
        close (); 
        return false; 
    }

    // Check if the font is no scalable
    // WARNING : Implement in the futur
    if (face->num_fixed_sizes)
    {
        std::cout << "ATK_FONT : Fixed font not only supported\n"; 
        close ();
        return false; 
    }
    
    valid = true; 
    return true; 
}


inline void atk_font::set_dpi (int i)
{
    dpi = i; 
}

inline void atk_font::set_size (int i)
{
    size = i; 
}


bool atk_font::build ()
{
    int error;
    char_info tmp;  
    
    if (!valid) return false;
    
    // Free memory from vecteur
    free_vector (); 
    
    // set the size
    FT_Set_Char_Size(face,size << 6,size << 6,dpi,dpi);
    
    // copy all characters in the vector
    for (int i = 0;i < face->num_glyphs; i++)
    {
        error = FT_Load_Char( face, i, FT_LOAD_RENDER); 
        if (error) std::cout << "ATK_font : Load char error\n";  

        // create an image
        tmp.picture = new image ();

        // size the image
        tmp.picture->resize (face->glyph->bitmap.width, face->glyph->bitmap.rows);  
        
        // copy bitmap to image
        copy_bitmap_to_image ( face->glyph->bitmap.buffer, tmp.picture);

        tmp.picture->set_mask (true); 
        
        // set position font
        tmp.top = face->glyph->bitmap_top;
        tmp.left = face->glyph->bitmap_left;
        tmp.advance_x = face->glyph->advance.x >> 6; 
        
        // add image to vector
        chars.push_back (tmp); 
    }
    return true; 
}



void atk_font::close ()
{
    if (!valid) return; 
    FT_Done_Face(face);
    FT_Done_FreeType(library); 
}


void atk_font::copy_bitmap_to_image (u_int8 * bitmap_data, image * dest)
{
    u_int8 * pbmp = bitmap_data; 

    
    dest->lock ();

    u_int32 pixelcol = screen::display.map_color (r, g, b); 
    
    for (int j = 0; j < dest->height (); j++)
        for (int i = 0; i < dest->length () ; i++)
        {
            if (*pbmp > 128) 
                dest->put_pix (i, j, pixelcol); 
            else dest->put_pix (i, j, screen::trans_col ()); 
            pbmp++; 
        } 
    dest->unlock ();  
}

void atk_font::info ()
{
    std::cout << "Filename : " << filename << std::endl;
    std::cout << "Number face : " << face->num_faces << std::endl; 
    std::cout << "Num_glyphs : " << face->num_glyphs << std::endl; 
    std::cout << "Fixed size : " << face->num_fixed_sizes << std::endl;  
}
 
void atk_font::draw (const std::string & text, s_int32 x, s_int32 y, surface * target = NULL)
{
    s_int32 tmp_x = x; 
    for (u_int32 i = 0; i < text.length (); i++)
    {
        if ((unsigned) text[i] < chars.size ()) 
        {
            if (text[i] != '\n')
            {                 
                chars[text[i]].picture->draw (tmp_x + chars[text[i]].left , y - chars[text[i]].top, NULL, target ); 
                tmp_x += chars[text[i]].advance_x;
            }
            else 
            {
                y += size; 
                tmp_x = x; 
            }
        }
    } 
}

u_int32 atk_font::get_length_of (const std::string & text)
{
    u_int32 size = 0;
    for (u_int32 i = 0; i < text.length (); i++)
        if ((unsigned) text[i] < chars.size ())
            size += chars[text[i]].advance_x;
    return size; 
}


u_int16 atk_font::get_size()
{
  return size;
}

void atk_font::free_vector ()
{
    for (std::vector <char_info>::iterator it = chars.begin (); it != chars.end (); it++)
        delete it->picture; 
    chars.clear (); 
}


atk_font::~atk_font ()
{
    free_vector ();
    close (); 
}



void atk_font::set_color (u_int8 r, u_int8 g, u_int8 b)
{
    this->r = r; 
    this->g = g;
    this->b = b; 
}


void atk_font::set_color (u_int32 col)
{
    this->r = col & 255; 
    this->g = (col >> 8) & 255; 
    this->b = (col >> 16) & 255;
}
