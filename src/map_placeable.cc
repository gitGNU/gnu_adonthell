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

#include "map_placeable.h"
#include "landmap.h"

map_placeable::map_placeable(landmap & mymap) : Mymap(mymap)
{
    Type = UNKNOWN;
    Current_state = States.begin (); 
    State_changed = false;
}

map_placeable_area * map_placeable::current_state()
{
    if (Current_state != States.end ())
        return &(Current_state->second);
    else return NULL; 
}

map_placeable_area * map_placeable::get_state (const string & name) 
{
    map <string, map_placeable_area>::iterator State;
    State = States.find (name); 
    if (State == States.end())
        return NULL;
    else return &(State->second);
}

const string map_placeable::current_state_name ()
{
    if (Current_state != States.end ())
        return Current_state->first;
    else return string (); 
}

map_placeable_area * map_placeable::add_state (const string & name) 
{
    return &((States.insert(pair<const string, const map_placeable_area> (name, map_placeable_area()))).first->second);
}


void map_placeable::set_state (const string & name) 
{    
    if (Current_state != States.end() && Current_state->first == name)
        return;
    
    map <string, map_placeable_area>::iterator Previous_state;
    Previous_state = Current_state;
    Current_state = States.find (name); 
    if (Current_state == States.end())
        Current_state = Previous_state;
    else State_changed = true;
}
