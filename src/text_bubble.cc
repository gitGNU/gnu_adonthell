/*
   $Id$

   Copyright (C) 2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   text_bubble.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the text_bubble class.
 * 
 * 
 */
 
#include "nls.h"
#include "text_bubble.h"
#include "win_manager.h"

text_bubble::text_bubble (const string & text, const string & textcolor,
                          const string & themename, u_int16 len) : win_label ()
{
    remain = 75 + text.length () * 4;

    win_font * font = win_manager::get_font (textcolor);  
    win_theme * theme = win_manager::get_theme (themename); 

    set_font (*font);
    set_background (*theme);
    set_border (*theme, win_border::MINI);

    label::resize (len, 0);
    set_form (label::AUTO_HEIGHT); 
    set_text (nls::translate (text));
    set_visible_border (true);
    set_visible_background (true);
    set_trans_background (true);
    set_visible (true);
    pack (); 
}

text_bubble::~text_bubble () 
{
}

bool text_bubble::update () 
{
    if (win_label::update ()) 
    {
        remain--;
        if (!remain) return false; 
    }
    return true; 
}
