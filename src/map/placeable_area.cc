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
 * @file   placeable_area.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable_area class.
 * 
 * 
 */

#include "placeable_area.h"

using namespace map;

void placeable_area::set_area_size(u_int16 nx, u_int16 ny)
{
    area.resize (nx);
    for (std::vector <std::vector <square_walkable_info> >::iterator i = area.begin ();
         i != area.end (); i++)
        i->resize (ny);
}

u_int16 placeable_area::area_height() const
{
    if (area.size ()) return area[0].size ();
    else return 0; 
}

void placeable_area::put(ogzstream & file)
{
    u_int32 i, j;
    area_height() >> file;
    area_length() >> file;
    for (j = 0; j < area_height(); j++)
        for (i = 0; i < area_length(); i++)
            area[i][j].put(file);
    
    base.x() >> file;
    base.y() >> file;
    base.ox() >> file;
    base.oy() >> file;

    zsize >> file;
}

void placeable_area::get(igzstream & file)
{
    u_int16 l, h;
    u_int32 i, j;
    h << file;
    l << file;
    
    set_area_size(l, h);
    for (j = 0; j < area_height(); j++)
        for (i = 0; i < area_length(); i++)
            area[i][j].get(file);
    
    u_int16 x, y, ox, oy;
    x << file;
    y << file;
    ox << file;
    oy << file;
    base.set_position(x, y);
    base.set_offset(ox, oy);

    zsize << file;
}
