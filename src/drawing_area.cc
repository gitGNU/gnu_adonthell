/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   drawing_area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the drawing_area class.
 * 
 * 
 */

#include "drawing_area.h"

drawing_area::drawing_area ()
{
    move (0, 0);
    resize (0, 0);
    draw_to = NULL; 
}

drawing_area::drawing_area (s_int16 px, s_int16 py, u_int16 pw, u_int16 ph)
{
    move (px, py);
    resize (pw, ph); 
    draw_to = NULL; 
}

drawing_area& drawing_area::operator = (SDL_Rect& r) 
{
    rect = r;
    draw_to = NULL;
    return (*this); 
}

SDL_Rect drawing_area::setup_rects () const
{
    if (draw_to)
    {
        SDL_Rect ret;
        SDL_Rect temp = draw_to->setup_rects ();
        
        ret.x = temp.x > x ()? temp.x : x (); 
        ret.y = temp.y > y ()? temp.y : y ();

        // Precalculated for faster operation.
        s_int32 xpl = x () + length (); 
        s_int32 txw = temp.x + temp.w;
        s_int32 txwmrx = txw - ret.x;
        s_int32 xplmrx = xpl - ret.x;  
        s_int32 yph = y () + height (); 
        s_int32 tyh = temp.y + temp.h;
        s_int32 tyhmry = tyh - ret.y;
        s_int32 yphmry = yph - ret.y; 
        
        
        ret.w = txw < xpl ? txwmrx > 0 ? txwmrx : 0 : xplmrx > 0 ? xplmrx : 0;
        ret.h = tyh < yph ? tyhmry > 0 ? tyhmry : 0 : yphmry > 0 ? yphmry : 0;
        
        return ret;
    }
    else return rect;

}

bool drawing_area::point_belong (s_int16 px, s_int16 py)
{
    return (px >= x () && px <= x () + length () &&
            py >= y () && py <= y () + height ()); 
}
