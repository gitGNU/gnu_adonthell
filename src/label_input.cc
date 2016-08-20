/*
   (C) Copyright 2000/2001/2003/2004 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "label_input.h"

label_input::label_input () : label ()
{
    set_cursor_visible (true);
    set_cursor_moveable (true);
    set_editable (true); 
}

label_input::~label_input()
{
	if (input::is_text_input())
	{
		input::stop_text_input();
	}
}

void label_input::set_editable (const bool b)
{
    editable_ = b; 
}
 
bool label_input::input_update()
{
    if (!editable_) return false; 

	if (!input::is_text_input())
	{
		input::start_text_input();
	}

    label::input_update();

    if (my_font_ == NULL) return false; 
    
    int count;
    std::string c = input::get_next_unicode ();

    while (!c.empty())
    {
        cursor_undraw ();
        if (c[0] == SDLK_BACKSPACE || c[0] == SDLK_DELETE)
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

            offset_x_ = my_cursor_.offset_x;
            fillrect (my_cursor_.pos_x, my_cursor_.pos_y,
                      (*my_font_) [glyph].length (),
                      my_font_->height (), screen::trans_col ()); 
            
            build (false);
        }
        else if (c[0] == SDLK_RETURN) add_text ("\n");
        else
        {
        	u_int16 idx = 0;
        	if (my_font_->in_table (ucd(c, idx)))
        	{
        		add_text (c);
        	}
        	else
        	{
        		add_text("?");
        	}
        }

        c = input::get_next_unicode ();
    }

    return true;
}
