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


#include "map_placeable_model.h"

map_placeable_model::map_placeable_model()
{
    Current_state = States.begin (); 
}

map_placeable_area * map_placeable_model::current_state()
{
    if (Current_state != States.end ())
        return &(Current_state->second);
    else return NULL; 
}

map_placeable_area * map_placeable_model::get_state (const string & name) 
{
    map <string, map_placeable_area>::iterator State;
    State = States.find (name); 
    if (State == States.end())
        return NULL;
    else return &(State->second);
}

const string map_placeable_model::current_state_name()
{
    if (Current_state != States.end ())
        return Current_state->first;
    else return string (); 
}

map_placeable_area * map_placeable_model::add_state (const string & name) 
{
    return &((States.insert(pair<const string, const map_placeable_area> (name, map_placeable_area()))).first->second);
}

void map_placeable_model::set_state (const string & name) 
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

void map_placeable_model::put(ogzstream & file) const
{
    u_int32 s = States.size();

    s >> file;
        
    for (map <string, map_placeable_area>::iterator i = States.begin();
         i != States.end(); i++)
    {
        i->first >> file;
        i->second.put(file);
    }        
}

void map_placeable_model::get(igzstream & file)
{
    u_int32 size;
        
    size << file;
        
    for (u_int32 i = 0; i < size; i++)
    {
        string s;
        s << file;

        map_placeable_area * mpa = add_state(s);
        mpa->get(file);
            
    }
}
