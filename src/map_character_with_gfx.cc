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
    add_state ("standnorth", moa);
    add_state ("standsouth", moa);
    add_state ("standeast", moa);
    add_state ("standwest", moa);
    
    mapsquare_obj_area_gfx moag;
    
    moag.set_animation (make_anim ("adontest/servant1_north_1.pnm"));
    add_gfx ("standnorth", moag);
    moag.set_animation (make_anim ("adontest/servant1_south_1.pnm"));
    add_gfx ("standsouth", moag);
    moag.set_animation (make_anim ("adontest/servant1_east_1.pnm"));
    add_gfx ("standeast", moag);
    moag.set_animation (make_anim ("adontest/servant1_west_1.pnm"));
    add_gfx ("standwest", moag);
    
    set_state ("standwest"); 
}
