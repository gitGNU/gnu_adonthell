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


#ifndef MAP_COORDINATES_H
#define MAP_COORDINATES_H

#include "types.h"

class map_coordinates
{
protected :
    u_int16 X, Y;
    s_int32 Z;
    u_int16 Ox, Oy;

public:
    map_coordinates ();
    map_coordinates (u_int16 x, u_int16 y, u_int16 z, u_int16 ox = 0, u_int16 oy = 0); 

    u_int16 x () const
    {
        return X; 
    }

    u_int16 y () const
    {
        return Y; 
    }

    s_int32 z() const
    {
        return Z;
    }

    u_int16 ox () const
    {
        return Ox; 
    }

    u_int16 oy () const
    {
        return Oy; 
    }
    
    void set_position (u_int16 x, u_int16 y) 
    {
        X = x;
        Y = y; 
    }

    void set_altitude(s_int32 z)
    {
        Z = z;
    }

    void set_offset (u_int16 ox, u_int16 oy) 
    {
        Ox = ox;
        Oy = oy; 
    }
    
    bool operator == (const map_coordinates & mc); 
    bool operator < (const map_coordinates & mc); 
    bool operator <= (const map_coordinates & mc); 
    bool operator > (const map_coordinates & mc); 
    bool operator >= (const map_coordinates & mc); 

    map_coordinates & operator + (const map_coordinates & mc); 
    map_coordinates & operator - (const map_coordinates & mc); 
}; 


#endif
