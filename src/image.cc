/*
   $Id$

   Copyright (C) 1999/2000/2001/2002/2004 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   image.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the image class.
 * 
 * 
 */

#include "image.h"
#include "pnm.h"
#include <iostream>

using namespace std; 

image::image () : surface ()
{
}

image::image (u_int16 l, u_int16 h, const u_int8 & scale) : surface (scale)
{
    resize (l, h);
}

image::image (SDL_Surface *s, const SDL_Color & color) : surface (screen::scale())
{
	set_alpha(SDL_ALPHA_OPAQUE, true);
	resize((s->w + screen::scale()/2)/screen::scale(), s->h/screen::scale());

    SDL_Surface *dest = to_sw_surface(NULL);
    SDL_SetColorKey (s, 1, SDL_MapRGB (s->format, color.r, color.g, color.b));
    SDL_BlitSurface (s, NULL, dest, NULL);

    unlock();

    SDL_FreeSurface (s);
}

image::~image () 
{
}

void image::resize (u_int16 l, u_int16 h)
{
    surface::resize (l, h); 
}

void image::clear ()
{
    surface::clear ();  
}

s_int8 image::get (igzstream& file)
{
    s_int8 ret;

    u_int8 m;
    u_int8 a;
    
    m << file;
    a << file;
    
    ret = get_raw (file);
    if (!ret)
    {
        set_mask (m); 
        set_alpha (a);
    }
    return ret;
}
 
s_int8 image::load (string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = get (file);
    file.close (); 
    return ret;
}

s_int8 image::get_raw (igzstream& file)
{
    void * rawdata; 

    u_int16 l, h; 

    clear (); 
    
    l << file;
    h << file; 

    rawdata = new char[l * h * 3];
    file.get_block (rawdata, l * h * 3); 
    
    raw2display (rawdata, l, h); 

    delete[] (char *) rawdata;
    
    if (!Surface) return -1;

    return 0;
}


s_int8 image::load_raw (string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
	{
    	std::cout << "cannot find file " << fname << std::endl;
    	return 1;
	}
    ret = get_raw (file);
    file.close (); 
    return ret;
}


s_int8 image::get_pnm (SDL_RWops * file)
{
    void *rawdata;
    u_int16 l, h; 

    clear (); 

    rawdata = pnm::get (file, &l, &h);
    
    raw2display (rawdata, l, h); 

    free (rawdata);

    if (!Surface) return -1;

    return 0;
}


s_int8 image::load_pnm (string fname)
{
    SDL_RWops *file;
    s_int8 ret = 0; 

    file = SDL_RWFromFile (fname.c_str (), "rb"); 
    if (!file)
    {
    	std::cout << "cannot find file " << fname << std::endl;
    	return 1;
    }
    ret = get_pnm (file);
    SDL_RWclose (file);
    return ret;
}

s_int8 image::put (ogzstream& file) const
{
    bool m = is_masked (); 
    s_int8 a = alpha ();
    
    m >> file;
    a >> file;
    
    put_raw (file); 
    
    return 0; 
}

s_int8 image::save (string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = put (file);
    file.close (); 
    return ret;
}

s_int8 image::put_raw (ogzstream& file) const
{
    length () >> file;
    height () >> file; 

    if (!length () || !height ()) return 0; 

    void *rawdata = get_data(3, R_MASK, G_MASK, B_MASK, 0);
	file.put_block ((u_int8 *) rawdata, length () * height() * 3);
	free(rawdata);

    return 0;
}

s_int8 image::save_raw (string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 

    if (!file.is_open ())
        return 1;
    ret = put_raw (file);
    file.close (); 
    return ret;
}

s_int8 image::put_pnm (SDL_RWops * file) const
{
    void *rawdata = get_data(3, R_MASK, G_MASK, B_MASK, 0);
    pnm::put (file, rawdata, length (), height ());
    free(rawdata);

    return 0; 
}

s_int8 image::save_pnm (string fname) const
{
    SDL_RWops *file;
    s_int8 ret = 0; 

    file = SDL_RWFromFile (fname.c_str (), "wb"); 
    if (!file)
        return 1;
    ret = put_pnm (file);
    SDL_RWclose (file);
    return ret;
}
 
void image::zoom (const surface& src, u_int16 l, u_int16 h, u_int16 x, u_int16 y)
{ 
    // Calculate the step per pixel.
    // While the surface positions are u_int16s, we use u_int32s for step
    // and position during zoom, that we'll divide by 65535 ( >> 16). That
    // way,  we can perform our zoom without having to divide two times per
    // pixel we proceed (we can replace the divides with shift, much much
    // faster.
    u_int32 xstep = (u_int32) (((double) src.length () / (double) l) * 65535); 
    u_int32 ystep = (u_int32) (((double) src.height () / (double) h) * 65535); 
    u_int32 xcur;
    u_int32 ycur;

    u_int32 col;  
         
    lock ();
    src.lock (); 
    ycur = 0;
    u_int16 i, j;
    for (j = y; j < h + y; j++)
    {
        xcur = 0; 
        for (i = x; i < l + x; i++)
        {
            col = src.get_pix ((u_int16)(xcur >> 16), (u_int16)(ycur >> 16));
            put_pix (i, j, col); 
            xcur += xstep; 
        }
        ycur += ystep;  
    }
    src.unlock ();
    unlock (); 
}

void image::tile (const surface& src, u_int16 l, u_int16 h, u_int16 x, u_int16 y)
{
    u_int16 posx;
    u_int16 posy;

    drawing_area da (x, y, l, h); 
    
    for (posy = 0; posy < h; posy += src.height ())
        for (posx = 0; posx < l; posx += src.length ())
            src.draw (x + posx, y + posy, &da, this); 
}

void image::brightness (const surface& src, u_int8 cont, bool proceed_mask)
{
    u_int16 i, j;
    u_int8 ir, ig, ib, ia;
    u_int32 temp = 0;

    clear();
    set_scale(src.scale());
    resize (src.length (), src.height ());

    lock ();
    src.lock ();
    for (j = 0; j < height () * scale(); j++)
        for (i = 0; i < length () * scale(); i++)
        {
            temp = src.get_pix (i, j);
            if ((proceed_mask) || temp != screen::trans_col ())
            {
            	unmap_color (temp, ir, ig, ib, ia);
                ir = (ir * cont) >> 8;
                ig = (ig * cont) >> 8;
                ib = (ib * cont) >> 8;
                temp = map_color(ir, ig, ib, ia);
            }
            put_pix (i, j, temp);
        }
    src.unlock ();
    unlock ();
}  

image& image::operator = (const image& src)
{
    (surface&) (*this) = (surface&) src;    
    return *this; 
}




// Private methods



void image::raw2display (void * rawdata, u_int16 l, u_int16 h)
{
	set_data(rawdata, l, h, 3, R_MASK, G_MASK, B_MASK, 0);
}
