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
 * @file   object_with_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the object_with_gfx class.
 * 
 * 
 */

#include "object_with_gfx.h"

using namespace map;

object_with_gfx::object_with_gfx (landmap & mymap) : object (mymap), placeable_gfx ((placeable &) *this) 
{
    //     placeable_area * moa;
    //     moa = add_state ("default");
    //     moa->set_area_size (4, 5);
    //     moa->base.set_position (3, 4);
    //     moa->base.set_offset (40, 40);

    //     placeable_area_gfx * moag;

    //     moag = add_gfx ("default"); 
    //     moag->get_animation()->load("adontest/house.anim");
    
    //     load("adontest/house.mdl");
    set_state("default"); 
}

void object_with_gfx::put(ogzstream & file) const
{
    object::put(file);
    placeable_model_gfx::put(file);
}

void object_with_gfx::get(igzstream & file)
{
    object::get(file);
    placeable_model_gfx::get(file);
    set_gfx(current_state_name());
}

s_int8 object_with_gfx::save(const std::string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    Filename = fname;
    return ret;
}

s_int8 object_with_gfx::load(const std::string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    Filename = fname;
    return ret;
}

void object_with_gfx::draw (s_int16 x, s_int16 y, const gfx::drawing_area * da_opt,
                            gfx::surface * target) const
{
    placeable_model_gfx::draw(x, y, da_opt, target);
}
