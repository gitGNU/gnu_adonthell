/*
   (C) Copyright 2000/2001/2004 Joel Vennin
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

#ifndef LABEL_H_
#define LABEL_H_

#include <vector> 
#include <string>
#include "input.h"
#include "win_font.h"

using namespace std; 

class label: public image
{
public :
    /**
       Constructor
       by default,  cursor is not moveable,  cursor is not visible,  and the form is set as NOTHING,  the default size is (0, 0) 
    */
    label (); 

    
    /**
       Destructor
    */
    virtual ~label ();

    
    /**
       Set the font
    */
    void set_font (win_font & font);  


    /**
       Set the text 
    */
    void set_text (const string & text);
    
    
    /**
       Add text
    */
    void add_text (const string & text);


    /**
       Set the form of the display
       NOTHING, AUTO_SIZE, AUTO_HEIGHT
    */
    void set_form (const u_int8 form); 

    
    /**
       Set visible cursor
    */
    void set_cursor_visible (const bool b);
    

    /**
       Set if the cursor can be moved with arrow key
    */
    void set_cursor_moveable (const bool b);
    
    
    /**
       Update the label
    */
    bool update (); 


    /**
       Update input label, you can move the cursor if the cursor is moveable
    */
    bool input_update (); 


    /**
       Get the text in string
    */
    const string & text_string () const;


    /**
       Get the text in char
    */
    const char * text_char () const;


    /**
       Resize the label
    */
    void resize (u_int16 l, u_int16 h);
    
    
#ifdef SWIG
    
#define label_NOTHING 0;
#define label_AUTO_HEIGHT 1;
#define label_AUTO_SIZE 2;

#define label_KEY_CURSOR_NEXT SDLK_RIGHT;
#define label_KEY_CURSOR_PREVIOUS SDLK_LEFT;
     
#endif
    
#ifndef SWIG 
    
    static const u_int8 NOTHING = 0;
    static const u_int8 AUTO_HEIGHT = 1;
    static const u_int8 AUTO_SIZE = 2; 

    
    const static SDL_Keycode KEY_CURSOR_NEXT = SDLK_RIGHT;
    const static SDL_Keycode KEY_CURSOR_PREVIOUS = SDLK_LEFT;

    

protected :

    void fit_text_width();

    u_int16 ucd (u_int16 & idx) const
    {
    	return ucd(my_text_, idx);
    }

    u_int16 ucd (const std::string & text, u_int16 & idx) const;
    
    struct Sline_text
    {
        u_int16 idx_beg;
        s_int16 idx_end;
        u_int16 pos_x;
        s_int16 offset_x;
    };
    
    struct Scursor
    {
        u_int16 pos_x;
        u_int16 pos_y;
        u_int16 idx;
        u_int16 line; 
        s_int16 offset_x;
    };
    
    /**
       Init vector and cursor,  don't erase my_text_
    */
    void init_vec_cursor (); 


    /**
       Build label 
       
    */
    void build(const bool erase_all); 
    

    /**
       
     */
    void build_form_nothing (); 


    /**

    */ 
    void build_form_auto_height ();

    /**
     */ 
    void build_form_auto_size(); 

    /**

    */
    void clean_surface (const bool erase_all); 

    /**
     */
    u_int8 find_word (u_int16 & index, u_int16 & wlength, u_int16 & wlengthpix, s_int16 & woffset, u_int16 & last_letter, const u_int16 rlength);

    /**
     */
    void draw_string (const bool at_cursor); 


    /**

    */
    void update_cursor (); 

    /**
     */
    void cursor_next ();

    /**
     */
    
    void cursor_previous ();

    /**
     */
    void cursor_draw ();

    void cursor_undraw (); 
    
    bool last_letter (u_int16 idx);
    
    // my_font
    win_font * my_font_; 
    
    
    // my text
    string my_text_;
    
    // temporary for gathering utf-8 text
    string new_text_;
    
    // form display
    u_int8 my_form_;  
    

    // visible cursor
    bool visible_cursor_; 

    // moveable_cursor
    bool moveable_cursor_; 
    
    
    u_int16 cursor_cur_blink_;
    

    static u_int16 cursor_blink_cycle; 
       
    // my cursor
    Scursor my_cursor_;   

    // my old cursor
    Scursor my_old_cursor_; 
    
    // it is a vector which represent each line in the label
    vector<Sline_text> my_vect_;   
    
    u_int16 start_line_;

#endif
    // it's the endif of swig
    
}; 
#endif




