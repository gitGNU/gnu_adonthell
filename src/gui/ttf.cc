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
#include "font.h"


win_ttf::win_ttf ()
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


bool win_ttf::load (const std::string & filename)
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


bool win_ttf::build ()
{
  FT_Set_Char_Size( my_face, my_size << 6, my_size << 6, my_dpi, my_dpi);
  return true; 
}

inline void win_ttf::set_dpi (int i) { my_dpi = i; }

inline void win_ttf::set_size (int i) { my_size = i; }


void win_ttf::close ()
{
    if (!my_valid) return; 
    FT_Done_Face(my_face);
    FT_Done_FreeType(my_library); 
}
