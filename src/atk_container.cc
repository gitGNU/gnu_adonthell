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

#include "atk_border.h"
#include "atk_container.h"


atk_container::atk_container () : border_width_ (5)
{ 
    border_ = new atk_border (this); 
}


void atk_container::set_border_width (const u_int16 b)
{
    border_width_ = b; 
}


u_int16 atk_container::get_border_width () const
{
    return border_width_; 
}



atk_container::~atk_container ()
{
    delete border_; 
}














