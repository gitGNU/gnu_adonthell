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
//     map_placeable_area * moa;
//     moa = add_state ("default");
//     moa->set_area_size (4, 5);
//     moa->base.set_position (3, 4);
//     moa->base.set_offset (40, 40);

//     map_placeable_area_gfx * moag;

//     moag = add_gfx ("default"); 
//     moag->get_animation()->load("adontest/house.anim");
    
//     load("adontest/house.mdl");
    set_state("default"); 
}

void map_object_with_gfx::put(ogzstream & file) const
{
    map_object::put(file);
    map_placeable_model_gfx::put(file);
}

void map_object_with_gfx::get(igzstream & file)
{
    map_object::get(file);
    map_placeable_model_gfx::get(file);
}

s_int8 map_object_with_gfx::save(string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    return ret;
}

s_int8 map_object_with_gfx::load(string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    return ret;
}
