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

#ifndef MAP_PLACEABLE_GFX_H
#define MAP_PLACEABLE_GFX_H

#include "mapsquare_obj_area_gfx.h"
#include "map_placeable.h"
#include <map>

class map_placeable_gfx : public drawable
{
private:
    map <string, mapsquare_obj_area_gfx> Gfxs;
    map <string, mapsquare_obj_area_gfx>::iterator Current_gfx;

public:
    map_placeable_gfx () 
    {
        Current_gfx = Gfxs.begin (); 
    }
    
    void add_gfx (const string & name, mapsquare_obj_area_gfx gfx)
    {
        Gfxs.insert (pair<const string, mapsquare_obj_area_gfx> (name, gfx)); 
    }

    void set_state (const string & name) 
    {
        Current_gfx = Gfxs.find (name); 
    }

    bool update () 
    {
        if (Current_gfx != Gfxs.end ())
            Current_gfx->second.update ();
        return true; 
    }

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        if (Current_gfx != Gfxs.end ())
            Current_gfx->second.draw (x, y, da_opt, target); 
    }

    void draw (s_int16 x, s_int16 y, map_placeable * mpl,
               const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        if (Current_gfx != Gfxs.end ())
            Current_gfx->second.draw (x, y, mpl->current_state (), da_opt, target); 
    }
}; 

#endif
