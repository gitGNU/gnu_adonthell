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


#include "mapsquare_obj_area.h"
#include <string>
#include <map>

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
    map <string, mapsquare_obj_area> States;
    map <string, mapsquare_obj_area>::iterator Current_state;

protected:
    placeable_type Type; 

    bool State_changed;
    
public:
    map_coordinates base; 
    
    map_placeable () 
    {
        Type = UNKNOWN;
        Current_state = States.begin (); 
        State_changed = false;
    }
    
    map_placeable (u_int16 l, u_int16 h) 
    {
        Type = UNKNOWN;
        Current_state = States.begin (); 
    }

    placeable_type type () 
    {
        return Type; 
    }

    mapsquare_obj_area * current_state () 
    {
        if (Current_state != States.end ())
            return &(Current_state->second);
        else return NULL; 
    }

    const string current_state_name () 
    {
        if (Current_state != States.end ())
            return Current_state->first;
        else return string (); 
    }

    void add_state (const string & name, const mapsquare_obj_area & area) 
    {
        States.insert (pair<const string, const mapsquare_obj_area> (name, area)); 
    }

    void set_state (const string & name) 
    {    
        if (Current_state != States.end() && Current_state->first == name)
            return;

        map <string, mapsquare_obj_area>::iterator Previous_state;
        Previous_state = Current_state;
        Current_state = States.find (name); 
        if (Current_state == States.end())
            Current_state = Previous_state;
        else State_changed = true;
    }

    friend class map_placeable_gfx;
};

#endif