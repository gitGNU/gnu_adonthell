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

#ifndef MAP_PLACEABLE_GFX_H
#define MAP_PLACEABLE_GFX_H

#include "map_placeable_model.h"
#include "map_placeable.h"
#include <map>

class map_placeable_gfx : public map_placeable_model
{
private:
    map_placeable & Target;

public:
    map_placeable_gfx (map_placeable & target) : map_placeable_model(), Target(target)
    {
    }
    
    bool update () 
    {
        if (Target.State_changed)
        {
            Target.State_changed = false;
            set_gfx (Target.current_state_name()); 
        }
        
        return map_placeable_model::update();
    }

    /** 
     * Draws the map_placeable representation.
     *
     * @todo: this function should call the Python drawing callback
     * function, if set. Currently it dumbly calls map_placeable_model::draw().
     * 
     * @param x X position where to draw
     * @param y Y position where to draw
     * @param da_opt Optionnal drawing area to use during the blit operation
     * @param target Optionnal surface to draw to (defaults to the screen)
     */
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        map_placeable_model::draw(x, y, da_opt, target);
    }
}; 

#endif
