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


#include "atk_misc.h"


atk_misc::atk_misc () : x_pad_ (0), y_pad_ (0), x_align_ (atk_misc::LEFT), y_align_ (atk_misc::TOP) 
{
}


void atk_misc::set_padding (const u_int32 xp, const u_int32 yp)
{
    x_pad_ = xp;
    y_pad_ = yp; 
}

u_int32 atk_misc::get_x_padding () const
{
    return x_pad_; 
}


u_int32 atk_misc::get_y_padding () const
{
    return y_pad_; 
}


void atk_misc::set_alignment (const u_int8 xa, const u_int8 ya)
{
    x_align_ = xa;
    y_align_ = ya;  
}

u_int8 atk_misc::get_x_alignment () const
{
    return x_align_; 
}


u_int8 atk_misc::get_y_alignment () const
{
    return y_align_; 
}
