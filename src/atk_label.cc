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

#include "atk_label.h"



atk_label::atk_label () : font_(NULL)
{ 
}


void atk_label::set_text (const std::string & text)
{
    text_ = text;
    
    transform_in_line (); 
}


std::string & atk_label::get_text ()
{
    return text_; 
}


void atk_label::set_font (atk_font & font)
{
    font_ = &font; 
}


atk_font * atk_label::get_font () const
{
    return font_; 
}


atk_label::~atk_label ()
{
    // normaly it's not necessary
    line_.clear (); 
}


void atk_label::transform_in_line ()
{
    line_.clear ();
    
    u_int32 begin = 0;
    u_int32 end = 0; 

    while (end < text_.length ())
    {
        if (text_[end] == '\n' || end == text_.length () - 1)
        {
            line_.push_back (text_.substr (begin, end - begin));  
            begin = end + 1; 
        }
        end++;
    } 
}


void atk_label::realize ()
{
    if (!font_) return;
    
    u_int32 max_line_size = 0; 
    u_int32 tmp_x; 
    u_int32 tmp_y;
    u_int16 i;
    
    // find the max line size
    for (i = 0; i < line_.size (); i++) 
        if ((tmp_x = font_->get_length_of (line_[i]))  > max_line_size)
            max_line_size = tmp_x;      

    // calcul the height
    tmp_y = font_->get_size () * (i); 
    

    // check if we must change size of this widget
    if (tmp_x + (get_x_padding () << 1)  > get_length () && tmp_y + (get_y_padding () << 1)  > get_height ())
        set_size (tmp_x + (get_x_padding () << 1) , tmp_y + (get_y_padding () << 1) );
    else if (tmp_x + (get_x_padding () << 1)  > get_length ()) set_size (tmp_x + (get_x_padding () << 1) ,get_height ());
    else if (tmp_y + (get_y_padding () << 1)  > get_height ())  set_size (get_length (), tmp_y + (get_y_padding () << 1) );   
}



bool atk_label::draw (gfx::drawing_area * da, gfx::surface * sf) 
{
  if (!atk_misc::draw (da, sf)) return false;
  if (!font_) return false; 

  s_int32 y_to_start;
  s_int32 x_to_start; 
  
  // check y position to start draw
  if (get_y_alignment () == TOP) y_to_start = get_y_real () + get_y_padding ();
  else if (get_y_alignment () == BOTTOM) y_to_start = get_y_real () + get_height () - (font_->get_size () * (line_.size ())) - get_y_padding ();
    else y_to_start = get_y_real () + (get_height () - (font_->get_size () * (line_.size ()))) >> 1; 
    
    for (u_int16 i = 0; i < line_.size (); i++)
    {
        // search x_to_start position
        if (get_x_alignment () == LEFT) x_to_start = get_x_real () + get_x_padding ();
        else if (get_x_alignment () == RIGHT) x_to_start = get_x_real () + get_length () - font_->get_length_of (line_[i]) - get_x_padding ();
        else x_to_start = get_x_real () + ((get_length () - font_->get_length_of (line_[i]))  >> 1); 
        
        // draw each line
        font_->draw (line_[i], x_to_start, y_to_start, da, sf); 
        
        // go to next line
        y_to_start += font_->get_size (); 
    }
    return true;
}




