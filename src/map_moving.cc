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
    Vz = 0.0;

    Lx = 65535;
    Ly = 65535;
    
    fox = 0.0; 
    foy = 0.0; 
    foz = 0.0;

    Has_moved = false;
    Is_falling = false;
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

void map_moving::set_vertical_velocity(float vz)
{
    Vz = vz;
}

void map_moving::set_limits (u_int16 mx, u_int16 my)
{
    Lx = mx;
    Ly = my; 
}

void map_moving::update_pos()
{
    Has_moved = 0; 

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
        if (X == Lx - 1) 
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
        if (Y == Ly - 1) 
        {
            Y--;
            foy = 39; 
        }
        Oy = (u_int16) foy; 
    }

    if (vz())
    {
        Has_moved = 1;
        foz += vz();
        while (foz <= -1.0)
        {
            Z--;
            foz += 1.0;
        }
        while (foz >= 1.0)
        {
            Z++;
            foz -= 1.0;
        }
    }
}

bool map_moving::should_fall(s_int32 prevz)
{
    map_placeable_area * state = current_state();

    for (int j = 0; j < state->area_height(); j++)
        for (int i = 0; i < state->area_length(); i++)
        {
            if (state->get(i, j).is_walkable()) continue;
            u_int16 px = x() - state->base.x() + i;
            u_int16 py = y() - state->base.y() + j;

            u_int16 nbx = 1 + (ox() != 0);
            u_int16 nby = 1 + (oy() != 0);

            for (u_int16 l = 0; l < nby; l++)
                for (u_int16 k = 0; k < nbx; k++)
                {
                    mapsquare * msqr = Mymap.get(px + k, py + l);
                    for (mapsquare::iterator it = msqr->begin(); it != msqr->end(); it++)
                    {
                        s_int32 objz = it->z() + it->obj->current_state()->zsize;
                        if ( objz < z() || objz > prevz) continue;

                        if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                           py + l - it->y() + it->obj->current_state()->base.y())
                            .is_walkable()) 
                        {
                            set_altitude(objz);
                            return false;
                        }
                    }
                }
        }

    return true;
}

bool map_moving::update()
{
//     map_coordinates prevpos = *this;
//     float prevfox = fox;
//     float prevfoy = foy;
//     float prevfoz = foz;
    s_int32 prevz = z();

    Mymap.remove(this);

    update_pos();

//     {
//         fox = prevfox;
//         foy = prevfoy;
//         *((map_coordinates*)this) = prevpos;
//     }

    if (!should_fall(prevz))
    {
        Is_falling = false;
    }else Is_falling = true;
    
    Mymap.put(this);
    return true; 
}
