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

static animation * make_anim (const string & s) 
{
    animation * an = new animation ();
    image * im = new image ();
    animationframe af; 
    im->load_pnm (s);
    
    an->insert_image (im, 0);
    af.set_image_nbr (0);
    af.set_mask (true);
    an->insert_frame (af, 0); 
    return an; 
}


map_object_with_gfx::map_object_with_gfx () : map_object (), map_placeable_gfx ((map_placeable &) *this) 
{
    mapsquare_obj_area moa;
    moa.resize (4, 5);
    moa.base.set_position (3, 4);
    add_state ("default", moa);

    mapsquare_obj_area_gfx moag;

    moag.set_animation (make_anim ("adontest/house.pnm"));
    add_gfx ("default", moag); 
    
    set_state ("default"); 
}
