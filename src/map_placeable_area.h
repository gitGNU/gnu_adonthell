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


#ifndef MAP_PLACEABLE_AREA_H_
#define MAP_PLACEABLE_AREA_H_

#include "map_coordinates.h"
#include <vector>
#include "fileops.h"

const int mapsquare_size = 40;

class mapsquare_walkable_info
{
private:
    bool Walkable;

public:
    mapsquare_walkable_info () 
    {
        Walkable = true;  
    }

    bool is_walkable () 
    {
        return Walkable; 
    }

    void set_walkable (bool b) 
    {
        Walkable = b; 
    }

    void put(ogzstream & file)
    {
        Walkable >> file;
    }

    void get(igzstream & file)
    {
        Walkable << file;
    }
}; 

class map_placeable_area
{
private:
    vector <vector <mapsquare_walkable_info> > area;
    
public:
    map_coordinates base; 

    u_int16 area_length() const
    {
        return area.size ();
    }
    
    u_int16 area_height() const;

    void set_area_size(u_int16 nx, u_int16 ny);

    mapsquare_walkable_info & get(u_int16 x, u_int16 y) 
    {
        return area[x][y]; 
    }

    bool update() 
    {
        return true; 
    }

    void put(ogzstream & file);

    void get(igzstream & file);
}; 

#endif

