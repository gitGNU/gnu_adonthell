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
 * @file   map_character_with_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the map_character_with_gfx class.
 * 
 * 
 */

#include "map_character_with_gfx.h"

map_character_with_gfx::map_character_with_gfx (landmap & mymap) : map_character (mymap),
                                                    map_placeable_gfx((map_placeable &)*this) 
{
//     map_placeable_area * moa;

//     moa = add_state ("n_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("n_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_walk");
//     moa->set_area_size (1, 3);
//     moa->base.set_position (0, 2);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("n_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("s_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("e_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state ("w_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

//     moa = add_state("say_hi");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);
//     moa->base.set_offset (40, 40);

    
//     map_placeable_area_gfx * moag;
//     animation * an;
//     image * im;
//     animationframe af;

//     moag = add_gfx ("n_stand");
//     moag->get_animation()->load("adontest/standnorth2.anim");
    
//     moag = add_gfx ("s_stand");
//     an = moag->get_animation();
//     im = new image();
//     im->load_pnm("adontest/anim/south_base.pnm");
//     an->insert_image(im, 0);
//     af.set_mask(true);
//     an->insert_frame(af, 0);

//     moag = add_gfx ("e_stand");
//     moag->get_animation()->load("adontest/standeast2.anim");

//     moag = add_gfx ("w_stand");
//     moag->get_animation()->load("adontest/standwest2.anim");

//     moag = add_gfx ("n_walk");
//     moag->get_animation()->load("adontest/walknorth2.anim");

//     moag = add_gfx ("s_walk");
//     an = moag->get_animation();
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov1.pnm");
//     an->insert_image(im, 0);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov2.pnm");
//     an->insert_image(im, 1);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov3.pnm");
//     an->insert_image(im, 2);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov4.pnm");
//     an->insert_image(im, 3);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov5.pnm");
//     an->insert_image(im, 4);
//     im = new image();
//     im->load_pnm("adontest/anim/south_mov6.pnm");
//     an->insert_image(im, 5);
//     af.set_mask(true);
//     af.set_offset(0,39);
//     af.set_image_nbr(0);
//     af.set_delay(9);
//     af.set_nextframe(1);
//     an->insert_frame(af, 0);
//     af.set_image_nbr(1);
//     af.set_delay(9);
//     af.set_nextframe(2);
//     an->insert_frame(af, 1);
//     af.set_image_nbr(2);
//     af.set_delay(9);
//     af.set_nextframe(3);
//     an->insert_frame(af, 2);
//     af.set_image_nbr(3);
//     af.set_delay(9);
//     af.set_nextframe(4);
//     an->insert_frame(af, 3);
//     af.set_image_nbr(4);
//     af.set_delay(9);
//     af.set_nextframe(5);
//     an->insert_frame(af, 4);
//     af.set_image_nbr(5);
//     af.set_delay(9);
//     af.set_nextframe(0);
//     an->insert_frame(af, 5);

//     moag = add_gfx ("e_walk");
//     moag->get_animation()->load("adontest/walkeast2.anim");

//     moag = add_gfx ("w_walk");
//     moag->get_animation()->load("adontest/walkwest2.anim");

//     moag = add_gfx ("n_run");
//     moag->get_animation()->load("adontest/runnorth2.anim");

//     moag = add_gfx ("s_run");
//     moag->get_animation()->load("adontest/runsouth2.anim");

//     moag = add_gfx ("e_run");
//     moag->get_animation()->load("adontest/runeast2.anim");

//     moag = add_gfx ("w_run");
//     moag->get_animation()->load("adontest/runwest2.anim");
    
//     set_state("s_stand");
//     save("adontest/nakedguy.mdl");
}

void map_character_with_gfx::put(ogzstream & file) const
{
    map_character::put(file);
    map_placeable_model_gfx::put(file);
}

void map_character_with_gfx::get(igzstream & file)
{
    map_character::get(file);
    map_placeable_model_gfx::get(file);
}

s_int8 map_character_with_gfx::save(string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    return ret;
}

s_int8 map_character_with_gfx::load(string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    return ret;
}
