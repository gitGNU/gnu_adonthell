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

#include "map_character_with_gfx.h"

map_character_with_gfx::map_character_with_gfx () : map_character (),
                                                    map_character_gfx () 
{
    mapsquare_obj_area moa;
    moa.resize (1, 2);
    moa.base.set_position (0, 1);
    add_state ("n_stand", moa);
    add_state ("s_stand", moa);
    add_state ("e_stand", moa);
    add_state ("w_stand", moa);

    add_state ("n_walk", moa);
    add_state ("s_walk", moa);
    add_state ("e_walk", moa);
    add_state ("w_walk", moa);

    add_state ("n_run", moa);
    add_state ("s_run", moa);
    add_state ("e_run", moa);
    add_state ("w_run", moa);

    add_state("say_hi", moa);
    
    mapsquare_obj_area_gfx moag;

    animation * an;
    an = new animation();
    an->load("adontest/standnorth.anim");
    moag.set_animation (an);
    add_gfx ("n_stand", moag);
    an = new animation();
    an->load("adontest/standsouth.anim");
    moag.set_animation (an);
    add_gfx ("s_stand", moag);
    an = new animation();
    an->load("adontest/standeast.anim");
    moag.set_animation (an);
    add_gfx ("e_stand", moag);
    an = new animation();
    an->load("adontest/standwest.anim");
    moag.set_animation (an);
    add_gfx ("w_stand", moag);

    an = new animation();
    an->load("adontest/walknorth.anim");
    moag.set_animation (an);
    add_gfx ("n_walk", moag);
    an = new animation();
    an->load("adontest/walksouth.anim");
    moag.set_animation (an);
    add_gfx ("s_walk", moag);
    an = new animation();
    an->load("adontest/walkeast.anim");
    moag.set_animation (an);
    add_gfx ("e_walk", moag);
    an = new animation();
    an->load("adontest/walkwest.anim");
    moag.set_animation (an);
    add_gfx ("w_walk", moag);

    an = new animation();
    an->load("adontest/runnorth.anim");
    moag.set_animation (an);
    add_gfx ("n_run", moag);
    an = new animation();
    an->load("adontest/runsouth.anim");
    moag.set_animation (an);
    add_gfx ("s_run", moag);
    an = new animation();
    an->load("adontest/runeast.anim");
    moag.set_animation (an);
    add_gfx ("e_run", moag);
    an = new animation();
    an->load("adontest/runwest.anim");
    moag.set_animation (an);
    add_gfx ("w_run", moag);
    
    an = new animation();
    an->load("adontest/say_hi.anim");
    moag.set_animation(an);
    add_gfx("say_hi", moag);
    
    set_state ("w_stand"); 
}
