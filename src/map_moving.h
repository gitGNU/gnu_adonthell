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

#ifndef MAP_MOVING_H
#define MAP_MOVING_H

#include "map_placeable.h"

class map_moving : public map_placeable, public map_coordinates
{
private:
    float fox, foy; 
    float Vx, Vy;
    bool Has_moved; 

    u_int16 Lx, Ly;
    
public:

    map_moving (landmap & mymap); 

    float vx () 
    {
        return Vx; 
    }

    float vy () 
    {
        return Vy; 
    }

    u_int16 lx () 
    {
        return Lx;
    }

    u_int16 ly () 
    {
        return Ly; 
    }
    
    void set_offset (u_int16 ox, u_int16 oy); 
    
    void set_limits (u_int16 mx, u_int16 my); 

    void set_velocity (float vx, float vy); 

    bool update (); 
}; 

#endif

