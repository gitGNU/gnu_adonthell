/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   mapsquares.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the mapsquare_walkable class.
 * 
 * 
 */

#include "mapsquares.h"

mapsquare_walkable::mapsquare_walkable () 
{
    set_walkable (ALL_WALKABLE);
}

s_int8 mapsquare_walkable::get (igzstream& file) 
{
    walkable << file;
    return 0; 
}

s_int8 mapsquare_walkable::put (ogzstream& file) 
{
    walkable >> file;
    return 0; 
}

