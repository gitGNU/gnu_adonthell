/*
   $Id$

   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <SDL/SDL_endian.h>
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
    
    static s_int32 c; 

    while((c=input::get_next_unicode())>0)
    {
        cursor_undraw (); 
        if((c == SDLK_BACKSPACE || c == SDLK_DELETE))
        {            
            if (my_text_.empty () || my_cursor_.idx == 0) return true;
            
            my_text_.erase(--my_cursor_.idx, 1);
            update_cursor ();
            my_old_cursor_ = my_cursor_; 

            lock (); 
            fillrect (my_cursor_.pos_x, my_cursor_.pos_y,
                      (*my_font_) [my_text_[my_cursor_.idx]].length (),
                      my_font_->height (), screen::trans_col ()); 
            unlock (); 
            
            build (false);
        }
        else if(c == SDLK_RETURN) add_text ("\n"); 
        else 
        {
            string s (1, (char) c);
            if (my_font_->in_table(c)) add_text (s); 
        }
    }  
    return true;
}

 


