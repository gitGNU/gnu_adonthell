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

#ifndef MAPSQUARE_OBJ_AREA_GFX
#define MAPSQUARE_OBJ_AREA_GFX

#include "animation.h"
#include "mapsquare_obj_area.h"

class mapsquare_obj_area_gfx : public drawable
{
private:
    animation * anim;

public:
    mapsquare_obj_area_gfx () 
    {
        anim = NULL; 
    }

    bool update () 
    {
        if (anim) anim->update ();
        return true; 
    }

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        if (anim)
            anim->draw (x, y, da_opt, target); 
    }

    void draw (s_int16 x, s_int16 y, mapsquare_obj_area * area,
               const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
            draw (x - (area->base.x () * mapsquare_size) - area->base.ox (),
                  y - (area->base.y () * mapsquare_size) - area->base.oy (),
                  da_opt, target); 
    }

    void set_animation (animation * an) 
    {
        anim = an; 
    }

    void resize (u_int16 nx, u_int16 ny)
    {
        drawable::set_length (nx * mapsquare_size); 
        drawable::set_height (ny * mapsquare_size); 
    }
}; 


#endif
