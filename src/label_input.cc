/*
   $Id$

   (C) Copyright 2000/2001/2003/2004 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "label_input.h"

label_input::label_input () : label ()
{
    set_cursor_visible (true);
    set_cursor_moveable (true);
    set_editable (true); 
}

void label_input::set_editable (const bool b)
{
    editable_ = b; 
}
 
bool label_input::input_update()
{
    if (!editable_) return false; 
    
    label::input_update();

    if (my_font_ == NULL) return false; 
    
    int count;
    static s_int32 c; 

    while ((c = input::get_next_unicode ()) > 0)
    {
        cursor_undraw ();
        if (c == SDLK_BACKSPACE || c == SDLK_DELETE)
        {            
            if (my_text_.empty () || my_cursor_.idx == 0) return true;
            
            // possibly delete multi-byte utf-8 char
            if (my_cursor_.idx > 2 && (u_int8) my_text_[my_cursor_.idx-3] == 0xEF) count = 3;
            else if (my_cursor_.idx > 1 && (u_int8) my_text_[my_cursor_.idx-2] == 0xC3) count = 2;
            else count = 1;
            
            my_cursor_.idx -= count;
            u_int16 idx = my_cursor_.idx;
            u_int16 glyph = ucd (idx);
            my_text_.erase (my_cursor_.idx, count);

            update_cursor ();
            my_old_cursor_ = my_cursor_; 

            lock (); 
            fillrect (my_cursor_.pos_x, my_cursor_.pos_y,
                      (*my_font_) [glyph].length (),
                      my_font_->height (), screen::trans_col ()); 
            unlock (); 
            
            build (false);
        }
        else if (c == SDLK_RETURN) add_text ("\n"); 
        else if (my_font_->in_table (c))
        {
            char r[3];
            
            // convert unicode to utf-8
            if (c < 0x80) count = 1;
            else if (c < 0x800) count = 2;
            else if (c < 0x10000) count = 3;
            
            switch (count) { /* note: code falls through cases! */
                case 3: r[2] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x800;
                case 2: r[1] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0xc0;
                case 1: r[0] = c;
            }
            
            add_text (string (r, count)); 
        }
    }  
    return true;
}

 


