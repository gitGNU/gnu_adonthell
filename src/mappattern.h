/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _mappattern_h
#define _mappattern_h

#include "image.h"

class mappattern
{
    image * frame;
    u_int16 nbr_of_frames;
    u_int16 framecounter;
    u_int16 counterlimit;
    u_int16 currentframe;

     public:
    ~mappattern ();
    void update ();
    s_int8 get (SDL_RWops * file);
    //    s_int8 put (SDL_RWops * file);
    s_int8 load (char name[]);
	// NULL!
    void putbox (u_int16 x, u_int16 y, drawing_area * da_opt);
    void putbox_mask (u_int16 x, u_int16 y, drawing_area * da_opt=NULL);
    //    void putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
    //		      u_int16 xo, u_int16 yo);
    void putbox_trans (u_int16 x, u_int16 y, u_int8 alpha, 
		       drawing_area * da_opt=NULL);
    //    void putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
    //			   u_int16 xo, u_int16 yo);
    void putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha, 
			    drawing_area * da_opt=NULL);
    //    void putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
    //			    u_int16 bh, u_int16 xo, u_int16 yo, 
    //			    u_int8 alpha);
    //    void putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
    //				 u_int16 xo, u_int16 yo, u_int8 alpha);
};

#endif
