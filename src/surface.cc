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
 * @file surface.cc
 * Defines the surface class.
 */ 

#include "surface.h"
#include "screen.h"

using namespace std; 


SDL_Rect surface::srcrect; 
SDL_Rect surface::dstrect; 


surface::surface () : drawable () 
{ 
    vis = NULL;
    set_alpha (255);
    set_mask (false); 
    not_screen = true; 
}

surface::~surface () 
{
    if (vis && not_screen) SDL_FreeSurface (vis); 
}

void surface::set_mask (bool m)
{
    if ((m) && (!mask_on) && vis)
        SDL_SetColorKey (vis, SDL_SRCCOLORKEY | SDL_RLEACCEL, screen::trans_col ()); 
    else if ((!m) && (mask_on) && vis)
        SDL_SetColorKey (vis, 0, 0);

    mask_on = m; 
}


void surface::set_alpha (u_int8 t)
{
#ifdef REVERSE_ALPHA
    if ((t == 255) && (alpha_ != 255) && vis)
        SDL_SetAlpha (vis, 0, 0);
    alpha_ = t;
#else
    if ((t == 0) && (alpha_ != 0) && vis)
        SDL_SetAlpha (vis, 0, 0);
    alpha_ = 255 - t;
#endif
}
 
void surface::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                         u_int16 sh, drawing_area * da_opt = NULL,
                         surface * target = NULL)
{ 
    if (target == NULL) target = &screen::display; 

    setup_rects (x, y, sx, sy, sl, sh, da_opt); 
    
    if (!dstrect.w || !dstrect.h)
        return;
    
    if (alpha () != 255)
        SDL_SetAlpha (vis, SDL_SRCALPHA, alpha_);
    
    SDL_BlitSurface (vis, &srcrect, target->vis, &dstrect); 
}

void surface::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int32 col, 
                        drawing_area * da_opt = NULL)
{
    if (da_opt) 
    {
        dstrect = da_opt->setup_rects (); 
    }
    else
    { 
        dstrect.x = x;
        dstrect.y = y;
        dstrect.w = l;
        dstrect.h = h;
    }
    
    SDL_FillRect (vis, &dstrect, col); 
}

void surface::put_pix (u_int16 x, u_int16 y, u_int32 col) 
{
    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x*vis->format->BytesPerPixel;
     
    switch (vis->format->BytesPerPixel) 
    {
        case 1:
            *((Uint8 *) (offset)) = (Uint8) col;
            break;
        case 2:
            *((Uint16 *) (offset)) = (Uint16) col;
            break;
        case 3:
        {
            u_int8 r, g, b;
            
            r = (col >> vis->format->Rshift);
            g = (col >> vis->format->Gshift);
            b = (col >> vis->format->Bshift);
            *((offset) + (vis->format->Rshift >> 3)) = r; 
            *((offset) + (vis->format->Gshift >> 3)) = g;
            *((offset) + (vis->format->Bshift >> 3)) = b;
            break;
        }
        case 4:
            *((Uint32 *) (offset)) = (Uint32) col;
            break;
    }
    
}

void surface::get_pix (u_int16 x, u_int16 y, u_int32& col) 
{
    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x * vis->format->BytesPerPixel;
    
    switch (vis->format->BytesPerPixel) 
    {
        case 1:
            (Uint8)col = *((Uint8 *)(offset));
            break;
        case 2:
             (Uint16)col = *((Uint16 *)(offset));
            break;
        case 3:
        {
            u_int8 r, g, b;
            col = 0;
            u_int32 t;
            
            r = *((offset) + (vis->format->Rshift >> 3)); 
            g = *((offset) + (vis->format->Gshift >> 3));
            b = *((offset) + (vis->format->Bshift >> 3));

            t = r << vis->format->Rshift;
            col |= t; 
            t = g << vis->format->Gshift;
            col |= t; 
            t = b << vis->format->Bshift;
            col |= t; 
            
            break;
        }
        case 4:
            *((Uint32 *)(offset)) = (Uint32)col;
            break;
    }
    
}
 
surface& surface::operator = (surface& src)
{
    (drawable&) (*this) = (drawable&) src; 
    mask_on = src.mask_on;
    alpha_ = src.alpha_;
    not_screen = src.not_screen;
    if (vis) SDL_FreeSurface (vis);
    if (!src.vis) 
        vis = NULL;
    else
        vis = SDL_DisplayFormat (src.vis);
    return *this; 
}
 


// Protected methods



void surface::resize (u_int16 l, u_int16 h)
{
    if (l == length () && h == height ()) return;
    
    if (vis) SDL_FreeSurface (vis); 

    set_length (l);
    set_height (h); 

    vis = SDL_CreateRGBSurface (SDL_HWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA,
                                length (), height (),
                                screen::bytes_per_pixel () * 8,
                                screen::display.vis->format->Rmask,
                                screen::display.vis->format->Gmask,
                                screen::display.vis->format->Bmask,
                                screen::display.vis->format->Amask); 
    set_mask (is_masked ());
    set_alpha (alpha ()); 
}

void surface::clear () 
{
    if (vis) 
    {
        SDL_FreeSurface (vis);
        vis = NULL; 
        set_length (0);
        set_height (0); 
        set_alpha (255);
        set_mask (false); 
    }
}



// Private methods



void surface::setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                           u_int16 sl, u_int16 sh, drawing_area * draw_to) 
{
    if (draw_to)
    { 
        drawing_area im_zone (x, y, sl, sh);
        SDL_Rect tr = draw_to->setup_rects ();

        drawing_area da_int;
        da_int = tr;
        
        im_zone.assign_drawing_area (&da_int);
        tr = im_zone.setup_rects ();

        dstrect = tr; 
        srcrect = dstrect;
        srcrect.x = x < dstrect.x ? sx + dstrect.x - x : sx;
        srcrect.y = y < dstrect.y ? sy + dstrect.y - y : sy;
    }
    else
    {
        srcrect.x = sx;
        srcrect.y = sy;
        srcrect.w = sl;
        srcrect.h = sh;

        dstrect = srcrect;
        
        dstrect.x = x;
        dstrect.y = y;
    } 
}