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

#ifndef MAP_PLACEABLE_MODEL_H
#define MAP_PLACEABLE_MODEL_H

#include "map_placeable_area.h"
#include <map>
#include "fileops.h"

class map_placeable_model
{
protected:
    mutable map <string, map_placeable_area> States;
    map <string, map_placeable_area>::iterator Current_state;

    bool State_changed;
    
public:
    map_placeable_model();

    map_placeable_area * current_state();

    map_placeable_area * get_state(const string & name);
    
    const string current_state_name();
    
    map_placeable_area * add_state (const string & name);
    
    void set_state (const string & name);

    void put(ogzstream & file) const;

    void get(igzstream & file);

    /**
     * This friendship is needed so map_placeable_model_gfx
     * can modify the Has_changed member.
     * 
     */
    friend class map_placeable_model_gfx;
}; 

#endif
