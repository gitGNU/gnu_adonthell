/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   map_moving.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the map_moving class.
 * 
 * 
 */

#include "map_moving.h"
#include "landmap.h"

map_moving::map_moving (landmap & mymap)
    : map_placeable (mymap), map_coordinates ()
{
    Vx = 0.0;
    Vy = 0.0; 

    Lx = 65535;
    Ly = 65535;
    
    fox = 0.0; 
    foy = 0.0; 
}

void map_moving::set_offset (u_int16 ox, u_int16 oy)
{
    map_coordinates::set_offset (ox, oy);
    fox = ox;
    foy = oy; 
}

void map_moving::set_velocity (float vx, float vy) 
{
    Vx = vx;
    Vy = vy;
}

void map_moving::set_limits (u_int16 mx, u_int16 my)
{
    Lx = mx;
    Ly = my; 
}

bool map_moving::update ()
{
    Has_moved = 0; 

    Mymap.remove(this);
    
    if (vx ()) 
    {
        Has_moved = 1;
        
        fox += vx ();
        
        while (fox < 0) 
        {
            if (X > 0)
            {
                X--;
                fox += mapsquare_size;
            }
            else fox = 0.0; 
        }

        while (fox >= mapsquare_size) 
        {
            if (X < Lx) 
            {
                X++; 
                fox -= mapsquare_size;
            }
            else
            {
                X = Lx - 1;
                fox = 39; 
            }
        }
        if (X == Lx) 
        {
            X--;
            fox = 39;
        }
        Ox = (u_int16) fox;
    }

    if (vy ()) 
    {
        Has_moved = 1; 

        foy += vy ();
        while (foy < 0) 
        {
            if (Y > 0)
            {
                foy += mapsquare_size;
                Y--;
            }
            else foy = 0.0; 
        }
        
        while (foy >= mapsquare_size) 
        {
            if (Y < Ly) 
            {
                foy -= mapsquare_size;
                Y++;
            }
            else 
            {
                Y = Ly - 1;
                foy = 39; 
            }
        }
        if (Y == Ly) 
        {
            Y--;
            foy = 39; 
        }
        Oy = (u_int16) foy; 
    }
    
    Mymap.put(this);
    return true; 
}
