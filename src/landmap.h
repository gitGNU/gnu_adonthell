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


#ifndef LANDMAP_H
#define LANDMAP_H

#include "map_character_with_gfx.h"
#include "map_object_with_gfx.h"
#include <vector>

class mapsquare_info : public map_coordinates
{
public:
    map_placeable * obj;
    
    mapsquare_info (map_coordinates & pos);

    bool operator < (const mapsquare_info & mi) const
    {
        if (y() < mi.y()) return true;
        if (y() != mi.y()) return false;
        if (oy() < mi.oy()) return true;
        if (oy() != mi.oy()) return false;
        // If the objects are at the same y position, we better
        // make an arbitrary test to make sure a moving object
        // won't go from behind to before another object when
        // their y coordinates are the same and the x coordinate
        // of the moving object become greater than the other object.
        if (obj < mi.obj) return true;
        return false;
    }

    bool operator == (const mapsquare_info & mi) const
    {
        // We call map_coordinates::operator == here
        return ((map_coordinates)*this == (map_coordinates)mi && 
                obj == mi.obj);
    }
}; 

class mapsquare
{
private:
    vector <mapsquare_info> objects; 

public:
    vector <mapsquare_info>::iterator begin () 
    {
        return objects.begin (); 
    }

    vector <mapsquare_info>::iterator end () 
    {
        return objects.end (); 
    }

    bool add (map_placeable * obj, map_coordinates & pos); 
    bool remove (map_placeable * obj, map_coordinates & pos); 
    bool exist (map_placeable * obj, map_coordinates & pos); 
}; 

class landmap
{
private:
    vector <vector <mapsquare> > area;

public:
    u_int16 length () const
    {
        return area.size ();
    }
    
    u_int16 height () const
    {
        if (area.size ()) return area[0].size ();
        else return 0; 
    }

    void resize (const u_int16 nx, const u_int16 ny);

    mapsquare * get (const u_int16 x, const u_int16 y); 

    bool put (map_placeable * obj, map_coordinates & pos); 
    bool put (map_moving * obj); 
    bool remove (map_placeable * obj, map_coordinates & pos); 
    bool remove (map_moving * obj); 
}; 


#endif

