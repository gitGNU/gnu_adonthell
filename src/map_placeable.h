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

#ifndef MAP_PLACEABLE_H
#define MAP_PLACEABLE_H


#include "map_placeable_area.h"
#include <string>
#include <map>

class landmap;

typedef enum 
{
    UNKNOWN = 0, 
    OBJECT,
    CHARACTER, 
    ITEM
} placeable_type; 

class map_placeable
{
protected:
    map <string, map_placeable_area> States;
    map <string, map_placeable_area>::iterator Current_state;

    placeable_type Type; 

    bool State_changed;

    landmap & Mymap;
    
public:
    map_coordinates base; 
    
    map_placeable(landmap & mymap);
    
    placeable_type type () 
    {
        return Type; 
    }

    map_placeable_area * current_state ();

    map_placeable_area * get_state (const string & name);

    const string current_state_name ();

    map_placeable_area * add_state (const string & name);

    void set_state (const string & name);

    /**
     * This friendship is needed so map_placeable_gfx
     * can modify the Has_changed member.
     * 
     */
    friend class map_placeable_gfx;
};

#endif
