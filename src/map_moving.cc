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
        u_int16 nX = X;
        float nfox = fox;

        Has_moved = 1;
        
        nfox += vx ();
        
        while (nfox < 0) 
        {
            if (nX > 0)
            {
                nX--;
                nfox += mapsquare_size;
            }
            else nfox = 0.0; 
        }

        while (nfox >= mapsquare_size) 
        {
            if (nX < Lx) 
            {
                nX++; 
                nfox -= mapsquare_size;
            }
            else
            {
                nX = Lx - 1;
                nfox = 39; 
            }
        }
        if (nX == Lx - 1) 
        {
            nfox = 0;
        }

        // Now check for walkability
        u_int16 nbr_sqr = nX > X ? nX - X : X - nX;
        map_placeable_area * state = current_state();
        
        for (u_int16 j = 0; j < state->area_height(); j++)
            for (u_int16 i = 0; i < state->area_length(); i++)
            {
                if (state->get(i, j).is_walkable()) continue;

                u_int16 px = x() - state->base.x() + i - (vx() < 0 ? nbr_sqr : 0);
                u_int16 py = y() - state->base.y() + j;
                
                u_int16 nbx = 1 + (nfox != 0) + (vx() > 0 ? nbr_sqr : 0);
                u_int16 nby = 1 + (oy() != 0);

                for (u_int16 l = 0; l < nby; l++)
                    for (u_int16 k = 0; k < nbx; k++)
                    {
                        mapsquare * msqr = Mymap.get(px + k, py + l);
                        for (mapsquare::iterator it = msqr->begin(); it != msqr->end(); it++)
                        {
                            if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) continue;

                            if (z() + climb_capability < it->z() + it->obj->current_state()->zsize && 
                                z() + state->zsize > it->z())
                                goto ytest;
                        }
                    }
            }

        X = nX;
        fox = nfox;
        Ox = (u_int16) fox;
    }

 ytest:

    if (vy ()) 
    {
        u_int16 nY = Y;
        float nfoy = foy;

        Has_moved = 1; 

        nfoy += vy ();
        while (nfoy < 0) 
        {
            if (nY > 0)
            {
                nfoy += mapsquare_size;
                nY--;
            }
            else nfoy = 0.0; 
        }
        
        while (nfoy >= mapsquare_size) 
        {
            if (nY < Ly) 
            {
                nfoy -= mapsquare_size;
                nY++;
            }
            else 
            {
                nY = Ly - 1;
                nfoy = 39; 
            }
        }
        if (nY == Ly - 1) 
        {
            nfoy = 0; 
        }

        // Now check for walkability
        u_int16 nbr_sqr = nY > Y ? nY - Y : Y - nY;
        map_placeable_area * state = current_state();
        
        for (u_int16 j = 0; j < state->area_height(); j++)
            for (u_int16 i = 0; i < state->area_length(); i++)
            {
                if (state->get(i, j).is_walkable()) continue;

                u_int16 px = x() - state->base.x() + i;
                u_int16 py = y() - state->base.y() + j - (vy() < 0 ? nbr_sqr : 0);
                
                u_int16 nbx = 1 + (ox() != 0);
                u_int16 nby = 1 + (nfoy != 0) + (vy() > 0 ? nbr_sqr : 0);

                for (u_int16 l = 0; l < nby; l++)
                    for (u_int16 k = 0; k < nbx; k++)
                    {
                        mapsquare * msqr = Mymap.get(px + k, py + l);
                        for (mapsquare::iterator it = msqr->begin(); it != msqr->end(); ++it)
                        {
                            if (it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) continue;

                            if (z() + climb_capability < it->z() + it->obj->current_state()->zsize && 
                                z() + state->zsize > it->z())
                                goto ztest;
                        }
                    }
            }

        Y = nY;
        foy = nfoy;
        Oy = (u_int16) foy;
    }

 ztest:

    s_int32 prevz = z();

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

    map_placeable_area * state = current_state();

    Is_falling = true;
    
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
                        if (vz() > 0)
                        {
                            s_int32 objz = it->z();
                            if (objz > z() + current_state()->zsize || objz < prevz + current_state()->zsize) continue;

                            if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) 
                            {
                                set_altitude(objz - (current_state()->zsize + 1));
                                set_vertical_velocity(0.0);
                                break;
                            }
                        }
                        else
                        {
                            s_int32 objz = it->z() + it->obj->current_state()->zsize;
                            if (objz < z() || objz > prevz + climb_capability) continue;
                            
                            if (!it->obj->current_state()->get(px + k - it->x() + it->obj->current_state()->base.x(),
                                                               py + l - it->y() + it->obj->current_state()->base.y())
                                .is_walkable()) 
                            {
                                set_altitude(objz);
                                Is_falling = false;
                                set_vertical_velocity(0.0);
                                break;
                            }
                        }
                    }
                }
        }
}

bool map_moving::update()
{
    Mymap.remove(this);

    update_pos();

    Mymap.put(this);
    return true; 
}
