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


#include "map_object_with_gfx.h"

map_object_with_gfx::map_object_with_gfx (landmap & mymap) : map_object (mymap), map_placeable_gfx ((map_placeable &) *this) 
{
    map_placeable_area * moa;
    moa = add_state ("default");
    moa->set_area_size (4, 5);
    moa->base.set_position (3, 4);

    map_placeable_area_gfx * moag;

    animation * an = new animation();
    moag = add_gfx ("default"); 
    an->load("adontest/house.anim");
    moag->set_animation (an);
    
    set_state ("default"); 
}
