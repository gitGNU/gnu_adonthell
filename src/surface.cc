/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
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

#include <iostream>

using namespace std; 


SDL_Rect surface::srcrect; 
SDL_Rect surface::dstrect; 

void surface::resize_aux (u_int16 l, u_int16 h)
{
    if (l == length () && h == height ()) return;
    
    if (vis) SDL_FreeSurface (vis); 

    set_length (l);
    set_height (h); 

    vis = SDL_CreateRGBSurface (SDL_SRCCOLORKEY | SDL_SRCALPHA | SDL_ASYNCBLIT,
                                l, h,
                                screen::bytes_per_pixel () * 8,
                                screen::display.vis->format->Rmask,
                                screen::display.vis->format->Gmask,
                                screen::display.vis->format->Bmask,
                                screen::display.vis->format->Amask); 
    changed = true; 
}

void surface::double_size(const surface & src)
{
    u_int32 col;

    lock ();
    src.lock ();
    
    resize(src.length(), src.height());
    for (u_int16 j = 0; j < height(); j++)
        for (u_int16 i = 0; i < length(); i++)
        {
            src.get_pix_aux(i, j, col);
            put_pix(i, j, col);
        }
    
    src.unlock ();
    unlock ();
}

void surface::half_size(const surface & src)
{
    u_int32 col;

    lock ();
    src.lock ();

    resize_aux(src.length(), src.height());
    for (u_int16 j = 0; j < src.height() - 1; j++)
        for (u_int16 i = 0; i < src.length() - 1; i++)
        {
            src.get_pix(i, j, col);
            put_pix_aux(i, j, col);
        }
        
    src.unlock ();
    unlock ();
}

void surface::get_pix_aux (u_int16 x, u_int16 y, u_int32& col) const
{
    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x * vis->format->BytesPerPixel;
    
    switch (vis->format->BytesPerPixel) 
    {
        case 1:
            col = *((Uint8 *)(offset));
            break;
        case 2:
            col = *((Uint16 *)(offset));
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
            col = *((Uint32 *)(offset));
            break;
    }
}

void surface::put_pix_aux (u_int16 x, u_int16 y, u_int32 col) 
{
    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x*vis->format->BytesPerPixel;
     
    switch (vis->format->BytesPerPixel) 
    {
        case 2:
            *((Uint16 *) (offset)) = (Uint16) col;
            if (screen::dblmode)
            {
                *((Uint16 *) (offset+vis->format->BytesPerPixel)) = (Uint16) col;
                *((Uint16 *) (offset+vis->pitch)) = (Uint16) col;
                *((Uint16 *) (offset+vis->pitch+vis->format->BytesPerPixel)) = (Uint16) col;
            }
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
            if (screen::dblmode)
            {
                *((offset+vis->format->BytesPerPixel) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->format->BytesPerPixel) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->format->BytesPerPixel) + (vis->format->Bshift >> 3)) = b;
                *((offset+vis->pitch) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->pitch) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->pitch) + (vis->format->Bshift >> 3)) = b;
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Bshift >> 3)) = b;
            }
            break;
        }
        case 4:
            *((Uint32 *)(offset)) = (Uint32) col;
            if (screen::dblmode)
            {
                *((Uint32 *) (offset+vis->format->BytesPerPixel)) = (Uint32) col;
                *((Uint32 *) (offset+vis->pitch)) = (Uint32) col;
                *((Uint32 *) (offset+vis->pitch+vis->format->BytesPerPixel)) = (Uint32) col;
            }
            break;
    }
    changed = true; 
}


surface::surface () : drawable () 
{ 
    vis = NULL;
    alpha_ = 255; 
    mask_on = false; 
    not_screen = true; 
    changed = false; 
}

surface::~surface () 
{
    if (vis && not_screen) SDL_FreeSurface (vis); 
}

void surface::set_mask (bool m)
{
    if (m != is_masked ())
    {
        mask_on = m;
        changed = true; 
    }
}


void surface::set_alpha (u_int8 t)
{
    if ((t == 255) && (alpha_ != 255) && vis)
        SDL_SetAlpha (vis, 0, 0);
    alpha_ = t;
}
 
void surface::draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                    u_int16 sh, const drawing_area * da_opt,
                    surface * target) const
{ 
    if (target == NULL) target = &screen::display; 

    setup_rects (x, y, sx, sy, sl, sh, da_opt); 

    if (screen::dblmode)
    {
        x <<= 1;
        y <<= 1;
        sx <<= 1;
        sy <<= 1;
        sl <<= 1;
        sh <<= 1;
        srcrect.x <<= 1;
        srcrect.y <<= 1;
        srcrect.w <<= 1;
        srcrect.h <<= 1;
        dstrect.x <<= 1;
        dstrect.y <<= 1;
        dstrect.w <<= 1;
        dstrect.h <<= 1;
    }
    
    if (!dstrect.w || !dstrect.h)
        return;
    
    if (changed)
    {
        changed = false;
        if (is_masked ()) 
            SDL_SetColorKey (vis, SDL_SRCCOLORKEY | SDL_RLEACCEL, screen::trans_col ()); 
        else
            SDL_SetColorKey (vis, 0, 0); 
    }

    if (alpha () != 255)
        SDL_SetAlpha (vis, SDL_SRCALPHA, alpha_);
    
    SDL_BlitSurface (vis, &srcrect, target->vis, &dstrect); 
    target->changed = true; 
}

void surface::fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h, u_int32 col, 
                        drawing_area * da_opt)
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

    if (screen::dblmode)
    {
        dstrect.x <<= 1;
        dstrect.y <<= 1;
        dstrect.w <<= 1;
        dstrect.h <<= 1;
    }

    SDL_FillRect (vis, &dstrect, col);
    changed = true; 
}

void surface::lock () const
{
    if (!length () || !height ()) return;   
    if (SDL_MUSTLOCK(vis)) 
        SDL_LockSurface (vis);
}

void surface::unlock () const
{
    if (!length () || !height ()) return;   
    if (SDL_MUSTLOCK(vis)) 
        SDL_UnlockSurface (vis);
}

void surface::put_pix (u_int16 x, u_int16 y, u_int32 col) 
{
    if (screen::dblmode)
    {
        x <<= 1;
        y <<= 1;
    }

    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x*vis->format->BytesPerPixel;
     
    switch (vis->format->BytesPerPixel) 
    {
        case 2:
            *((Uint16 *) (offset)) = (Uint16) col;
            if (screen::dblmode)
            {
                *((Uint16 *) (offset+vis->format->BytesPerPixel)) = (Uint16) col;
                *((Uint16 *) (offset+vis->pitch)) = (Uint16) col;
                *((Uint16 *) (offset+vis->pitch+vis->format->BytesPerPixel)) = (Uint16) col;
            }
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
            if (screen::dblmode)
            {
                *((offset+vis->format->BytesPerPixel) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->format->BytesPerPixel) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->format->BytesPerPixel) + (vis->format->Bshift >> 3)) = b;
                *((offset+vis->pitch) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->pitch) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->pitch) + (vis->format->Bshift >> 3)) = b;
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Rshift >> 3)) = r; 
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Gshift >> 3)) = g;
                *((offset+vis->pitch+vis->format->BytesPerPixel) + (vis->format->Bshift >> 3)) = b;
            }
            break;
        }
        case 4:
            *((Uint32 *)(offset)) = (Uint32) col;
            if (screen::dblmode)
            {
                *((Uint32 *) (offset+vis->format->BytesPerPixel)) = (Uint32) col;
                *((Uint32 *) (offset+vis->pitch)) = (Uint32) col;
                *((Uint32 *) (offset+vis->pitch+vis->format->BytesPerPixel)) = (Uint32) col;
            }
            break;
    }     
    changed = true; 
}

void surface::get_pix (u_int16 x, u_int16 y, u_int32& col) const
{
    if (screen::dblmode)
    {
        x <<= 1;
        y <<= 1;
    }
    u_int8 * offset = ((Uint8 *) vis->pixels) + y * vis->pitch
        + x * vis->format->BytesPerPixel;
    
    switch (vis->format->BytesPerPixel) 
    {
        case 2:
            col = *((Uint16 *)(offset));
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
            col = *((Uint32 *)(offset));
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
    changed = true; 
    return *this; 
}
 


// Protected methods



void surface::resize (u_int16 l, u_int16 h)
{
    if (l == length () && h == height ()) return;
    
    if (vis) SDL_FreeSurface (vis); 

    set_length (l);
    set_height (h); 

    if (screen::dblmode)
    {
        l <<= 1;
        h <<= 1;
    }

    vis = SDL_CreateRGBSurface (SDL_SRCCOLORKEY | SDL_SRCALPHA | SDL_ASYNCBLIT,
                                l, h,
                                screen::bytes_per_pixel () * 8,
                                screen::display.vis->format->Rmask,
                                screen::display.vis->format->Gmask,
                                screen::display.vis->format->Bmask,
                                screen::display.vis->format->Amask); 
    changed = true; 
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
        changed = true; 
    }
}



// Private methods



void surface::setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                           u_int16 sl, u_int16 sh, const drawing_area * draw_to) const
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
