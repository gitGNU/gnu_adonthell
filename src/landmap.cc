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


#include "landmap.h"
#include <algorithm>

mapsquare_info::mapsquare_info (map_coordinates & pos)
 : map_coordinates (pos) 
{
}

bool mapsquare::add (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    objects.push_back (mi);
    return true; 
}

bool mapsquare::remove (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    vector <mapsquare_info>::iterator er = find (objects.begin (), objects.end (), mi); 
    if (er == objects.end ()) return false;
    objects.erase (er);
    return true; 
}

bool mapsquare::exist (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    vector <mapsquare_info>::iterator er = find (objects.begin (), objects.end (), mi); 
    if (er == objects.end ()) return false;
    return true; 
}

void landmap::resize (const u_int16 nx, const u_int16 ny) 
{
    area.resize (nx);
    for (vector <vector <mapsquare> >::iterator i = area.begin ();
         i != area.end (); i++)
        i->resize (ny); 
}

mapsquare * landmap::get (const u_int16 x, const u_int16 y) 
{
    return (&area[x][y]); 
}

bool landmap::put (map_placeable * obj, map_coordinates & pos) 
{
    u_int16 i, j;
    mapsquare_obj_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = pos.x () < state->base.x () ? 0 :
        pos.x () - state->base.x (); 
    u_int16 sy = pos.y () < state->base.y () ? 0 :
        pos.y () - state->base.y (); 
    
    u_int16 fx = pos.x () + state->area_length () - state->base.x (); 
    u_int16 fy = pos.y () + state->area_height () - state->base.y () ; 

    if (pos.ox ()) fx++;
    if (pos.oy ()) fy++; 
    
    mapsquare * msqr; 
    
    for (j = sy; j < fy; j++) 
        for (i = sx; i < fx; i++) 
        {
            msqr = get (i, j);
            msqr->add (obj, pos); 
        }
    return true; 
}

bool landmap::put (map_moving * obj) 
{
    return put (obj, *obj); 
}

bool landmap::remove (map_placeable * obj, map_coordinates & pos) 
{
    u_int16 i, j;
    mapsquare_obj_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = pos.x () < state->base.x () ? 0 :
        pos.x () - state->base.x (); 
    u_int16 sy = pos.y () < state->base.y () ? 0 :
        pos.y () - state->base.y (); 
    
    u_int16 fx = pos.x () + state->area_length () - state->base.x (); 
    u_int16 fy = pos.y () + state->area_height () - state->base.y () ; 

    if (pos.ox ()) fx++;
    if (pos.oy ()) fy++; 
    
    mapsquare * msqr; 
    
    for (j = sy; j < fy; j++) 
        for (i = sx; i < fx; i++) 
        {
            msqr = get (i, j);
            msqr->remove (obj, pos); 
        }
    return true; 
}

bool landmap::remove (map_moving * obj) 
{
    return remove (obj, *obj); 
}
