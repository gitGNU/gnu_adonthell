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

#include <iostream>
#include "label_input.h"

using namespace gui;

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
  //std::cout << "Youhou 1\n";
  if (!editable_) return false; 
    
    label::input_update();

    if (my_font_ == NULL) return false; 
    
    static s_int32 c; 

    while((c=input::get_next_unicode())>0)
    {
      std::cout << "Key code: " << c << std::endl;
        cursor_undraw ();
	
	if (c == SDLK_SPACE) {
	  add_text (L" ");
	  std::cout << "Space\n";
	}
        else if((c == SDLK_BACKSPACE || c == SDLK_DELETE) && my_text_.size () >0 && my_cursor_.idx > 0)
        {    
            my_text_.erase(--my_cursor_.idx, 1);
            update_cursor ();
            my_old_cursor_ = my_cursor_; 

            lock (); 
            fillrect (my_cursor_.pos_x, my_cursor_.pos_y,
                      (*my_font_) [my_text_[my_cursor_.idx]].my_advance,
                      my_font_->height (), gfx::screen::trans_col ()); 
            unlock (); 
            
            build (false);
        }
        else if(c == SDLK_RETURN) add_text (L"\n"); 
        else if(my_font_->in_table(c)) add_text((wchar_t*)&c); 
    }  
    return true;
}
