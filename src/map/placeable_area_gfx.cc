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


#include "placeable_area_gfx.h"

map::placeable_area_gfx::placeable_area_gfx() : drawable()
{
    anim = new gfx::animation(); 
}

map::placeable_area_gfx::~placeable_area_gfx()
{
    delete anim;
}

void map::placeable_area_gfx::draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                                   gfx::surface * target) const
{
    if(anim)
        anim->draw (x, y, da_opt, target); 
}

void map::placeable_area_gfx::set_area_size(u_int16 nx, u_int16 ny)
{
    drawable::set_length (nx * square_size); 
    drawable::set_height (ny * square_size); 
}
