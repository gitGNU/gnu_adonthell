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
    zground = pos.z();
}

bool mapsquare_info::operator < (const mapsquare_info & mi) const
{
//     if (y() == mi.y() || (y() == mi.y() + 1 && mi.oy()) &&
//         z() + obj->current_state()->zsize <= mi.z()) return true;

//     if (mi.y() == y() || (mi.y() == y() + 1 && oy()) &&
//         z() >= mi.z() + mi.obj->current_state()->zsize) return false;

    if (z() + obj->current_state()->zsize <= mi.zground) return true;
    if (zground >= mi.z() + mi.obj->current_state()->zsize) return false;
    if (y() < mi.y()) return true;
    if (y() > mi.y()) return false;
    if (oy() < mi.oy()) return true;
    if (oy() > mi.oy()) return false;

//     if (obj->type() == OBJECT) return true;
//     if (obj->type() == CHARACTER) return false;
//     if (z() + obj->current_state()->zsize < mi.z() + mi.obj->current_state()->zsize) return true;
//     if (z() + obj->current_state()->zsize > mi.z() + mi.obj->current_state()->zsize) return false;

//     if ((y() * mapsquare_size) + oy() + mapsquare_size <=
//         (mi.y() - (mi.obj->current_state()->area_height() - 1)) * mapsquare_size + mi.oy()) return true;

//     if ((y() - (obj->current_state()->area_height() - 1)) * mapsquare_size + oy() >=
//         mapsquare_size + mi.y() * mapsquare_size + mi.oy()) return false;

    // If the objects are at the same y position, we better
    // make an arbitrary test to make sure a moving object
    // won't go from behind to before another object when
    // their y coordinates are the same and the x coordinate
    // of the moving object become greater than the other object.
    return false;
}

bool mapsquare::add (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    vector<mapsquare_info>::iterator it = objects.begin();
    while(it != objects.end() && mi.z() + mi.obj->current_state()->zsize < 
          it->z() + it->obj->current_state()->zsize) ++it;
    objects.insert(it, mi);
    return true; 
}

bool mapsquare::add (map_moving * obj)
{
    mapsquare_info mi (*obj);
    mi.obj = obj; 
    mi.zground = obj->zground;
    vector<mapsquare_info>::iterator it = objects.begin();
    while(it != objects.end() && mi.z() + mi.obj->current_state()->zsize < 
          it->z() + it->obj->current_state()->zsize) ++it;
    objects.insert(it, mi);
    return true; 
}

bool mapsquare::remove (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    vector <mapsquare_info>::iterator er;
    er = find (objects.begin (), objects.end (), mi);
    if (er == objects.end ()) return false;
    objects.erase (er);
    return true; 
}

bool mapsquare::exist (map_placeable * obj, map_coordinates & pos)
{
    mapsquare_info mi (pos);
    mi.obj = obj; 
    vector <mapsquare_info>::iterator er;
    er = find (objects.begin (), objects.end (), mi); 
    if (er == objects.end ()) return false;
    return true; 
}

landmap::~landmap()
{
    clear();
}

void landmap::clear()
{
    objects.clear();
    characters.clear();
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
    map_placeable_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = pos.x () < state->base.x () ? 0 :
        pos.x () - state->base.x (); 
    u_int16 sy = pos.y () < state->base.y () ? 0 :
        pos.y () - state->base.y (); 
    
    u_int16 fx = pos.x () + state->area_length () - state->base.x ();
    u_int16 fy = pos.y () + state->area_height () - state->base.y (); 

    if (pos.ox ()) fx++;
    if (pos.oy ()) fy++; 
    
    if (fx > length()) fx = length() - 1;
    if (fy > height()) fy = height() - 1;

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
    u_int16 i, j;
    map_placeable_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = obj->x () < state->base.x () ? 0 :
        obj->x () - state->base.x (); 
    u_int16 sy = obj->y () < state->base.y () ? 0 :
        obj->y () - state->base.y (); 
    
    u_int16 fx = obj->x () + state->area_length () - state->base.x ();
    u_int16 fy = obj->y () + state->area_height () - state->base.y (); 

    if (obj->ox ()) fx++;
    if (obj->oy ()) fy++; 
    
    if (fx > length()) fx = length() - 1;
    if (fy > height()) fy = height() - 1;

    mapsquare * msqr; 
    
    for (j = sy; j < fy; j++) 
        for (i = sx; i < fx; i++) 
        {
            msqr = get (i, j);
            msqr->add (obj); 
        }

    return true; 

    //    return put (obj, *obj); 
}

bool landmap::remove (map_placeable * obj, map_coordinates & pos) 
{
    u_int16 i, j;
    map_placeable_area * state = obj->current_state ();

    if (!state) return false;

    u_int16 sx = pos.x () < state->base.x () ? 0 :
        pos.x () - state->base.x (); 
    u_int16 sy = pos.y () < state->base.y () ? 0 :
        pos.y () - state->base.y (); 
    
    u_int16 fx = pos.x () + state->area_length () - state->base.x ();
    u_int16 fy = pos.y () + state->area_height () - state->base.y (); 

    if (pos.ox ()) fx++;
    if (pos.oy ()) fy++; 
    
    if (fx > length()) fx = length() - 1;
    if (fy > height()) fy = height() - 1;

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

void landmap::update()
{
    objects.update();
    characters.update();
}

map_object * landmap::add_map_object()
{    
    return objects.add(*this);
}

map_character * landmap::add_map_character()
{
    return characters.add(*this);
}

bool landmap::put_map_object(u_int32 index, map_coordinates & pos)
{
    put(objects[index], pos);
    return true;
}

void landmap::output_occupation()
{
    for (u_int16 j = 0; j < height(); j++)
    {
        for (u_int16 i = 0; i < length(); i++)
        {
            mapsquare * msqr = get(i, j);
            u_int16 nb = 0;
            for (mapsquare::iterator it = msqr->begin(); it != msqr->end(); it++)
                nb++;
            cout << nb << " ";
        }
        cout << endl;
    }
}
