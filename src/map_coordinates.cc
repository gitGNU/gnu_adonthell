/*
   $Id:

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream>
#include "map_coordinates.h"

map_coordinates::map_coordinates ()
{
    X = 0;
    Y = 0;
    Ox = 0;
    Oy = 0; 
}

map_coordinates::map_coordinates (u_int16 x, u_int16 y, u_int16 ox = 0, u_int16 oy = 0)
{
    X = x; 
    Y = y; 
    Ox = ox; 
    Oy = oy; 
}

bool map_coordinates::operator == (const map_coordinates & mc)
{
    return X == mc.X && Y == mc.Y && Ox == mc.Ox && Oy == mc.Oy; 
}

bool map_coordinates::operator < (const map_coordinates & mc)
{
    if (Y < mc.Y) return true;
    if (Y != mc.Y) return false;
    if (Oy < mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X < mc.X) return true;
    if (X != mc.X) return false;
    if (Ox < mc.Ox) return true;
    return false;
}

bool map_coordinates::operator <= (const map_coordinates & mc) 
{
    if (Y <= mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy <= mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X <= mc.X) return true;
    if (X != mc.X) return false;
    if (Ox <= mc.Ox) return true;
    return false; 
}

bool map_coordinates::operator > (const map_coordinates & mc) 
{
    if (Y > mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy > mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X > mc.X) return true;
    if (X != mc.X) return false;
    if (Ox > mc.Ox) return true;
    return false; 
}

bool map_coordinates::operator >= (const map_coordinates & mc)
{
    if (Y >= mc.Y) return true;
    if (Y != mc.Y) return false; 
    if (Oy >= mc.Oy) return true;
    if (Oy != mc.Oy) return false;
    if (X >= mc.X) return true;
    if (X != mc.X) return false;
    if (Ox >= mc.Ox) return true;
    return false; 
}
