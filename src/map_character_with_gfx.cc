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

#include "map_character_with_gfx.h"

map_character_with_gfx::map_character_with_gfx (landmap & mymap) : map_character (mymap),
                                                    map_placeable_gfx((map_placeable &)*this) 
{
//     map_placeable_area * moa;
//     moa = add_state ("n_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("s_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("e_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("w_stand");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("n_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("s_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("e_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("w_walk");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("n_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("s_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("e_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state ("w_run");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

//     moa = add_state("say_hi");
//     moa->set_area_size (1, 2);
//     moa->base.set_position (0, 1);

    
//     map_placeable_area_gfx * moag;
//     animation * an;

//     moag = add_gfx ("n_stand");
//     an = new animation();
//     an->load("adontest/standnorth2.anim");
//     moag->set_animation (an);
    
//     moag = add_gfx ("s_stand");
//     an = new animation();
//     an->load("adontest/standsouth2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("e_stand");
//     an = new animation();
//     an->load("adontest/standeast2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("w_stand");
//     an = new animation();
//     an->load("adontest/standwest2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("n_walk");
//     an = new animation();
//     an->load("adontest/walknorth2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("s_walk");
//     an = new animation();
//     an->load("adontest/walksouth2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("e_walk");
//     an = new animation();
//     an->load("adontest/walkeast2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("w_walk");
//     an = new animation();
//     an->load("adontest/walkwest2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("n_run");
//     an = new animation();
//     an->load("adontest/runnorth2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("s_run");
//     an = new animation();
//     an->load("adontest/runsouth2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("e_run");
//     an = new animation();
//     an->load("adontest/runeast2.anim");
//     moag->set_animation (an);

//     moag = add_gfx ("w_run");
//     an = new animation();
//     an->load("adontest/runwest2.anim");
//     moag->set_animation (an);
    
//     moag = add_gfx ("say_hi");
//     an = new animation();
//     an->load("adontest/say_hi.anim2");
//     moag->set_animation(an);


    load("adontest/chrono.mdl");
    set_state ("s_stand"); 
}
