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

/**
 * @file   placeable_model.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the placeable_model class.
 * 
 * 
 */


#include "placeable_model.h"

using namespace map;

placeable_model::placeable_model()
{
    Current_state = States.begin (); 
}

placeable_area * placeable_model::current_state()
{
    if (Current_state != States.end ())
        return &(Current_state->second);
    else return NULL; 
}

placeable_area * placeable_model::get_state (const std::string & name) 
{
    std::map <std::string, placeable_area>::iterator State;
    State = States.find (name); 
    if (State == States.end())
        return NULL;
    else return &(State->second);
}

const std::string placeable_model::current_state_name()
{
    if (Current_state != States.end ())
        return Current_state->first;
    else return std::string (); 
}

placeable_area * placeable_model::add_state (const std::string & name) 
{
    return &((States.insert(std::pair<const std::string, const placeable_area> (name, placeable_area()))).first->second);
}

bool placeable_model::del_state (const std::string & name)
{
    return States.erase(name);
}

void placeable_model::set_state (const std::string & name) 
{    
    if (Current_state != States.end() && Current_state->first == name)
        return;
        
    std::map <std::string, placeable_area>::iterator Previous_state;
    Previous_state = Current_state;
    Current_state = States.find (name); 
    if (Current_state == States.end())
        Current_state = Previous_state;
    else State_changed = true;
}

void placeable_model::put(ogzstream & file) const
{
    u_int32 s = States.size();

    s >> file;
        
    for (std::map <std::string, placeable_area>::iterator i = States.begin();
         i != States.end(); i++)
    {
        i->first >> file;
        i->second.put(file);
    }        
}

void placeable_model::get(igzstream & file)
{
    u_int32 size;
        
    size << file;
        
    for (u_int32 i = 0; i < size; i++)
    {
        std::string s;
        s << file;

        placeable_area * mpa = add_state(s);
        mpa->get(file);
            
    }
}
