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
 * @file   map_placeable_model.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_placeable_model class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_MODEL_H
#define MAP_PLACEABLE_MODEL_H

#include "map_placeable_area.h"
#include "fileops.h"
#include <map>
#include <string>

using std::map;
using std::string;
using std::pair;

/**
 * Represents a placeable, i.e. something (character, object, ...)
 * that can be placed on a map and occupies some space on it.
 *
 * This class is separated from map_placeable to allow easy edition
 * without depending on the whole map stuff. While it is suitable
 * for edition, it can't be actually placed on a map. Use map_placeable
 * for that instead.
 * 
 */
class map_placeable_model
{
protected:
    mutable string Filename;

    mutable map <string, map_placeable_area> States;
    map <string, map_placeable_area>::iterator Current_state;

    bool State_changed;
    
public:
    typedef map <string, map_placeable_area>::iterator iterator;

    iterator begin () 
    {
        return States.begin (); 
    }

    iterator end () 
    {
        return States.end (); 
    }

    map_placeable_model();

    map_placeable_area * current_state();

    map_placeable_area * get_state(const string & name);
    
    const string current_state_name();
    
    map_placeable_area * add_state (const string & name);

    bool del_state (const string & name);
    
    void set_state (const string & name);

    void put(ogzstream & file) const;

    void get(igzstream & file);

    const string & filename()
    {
        return Filename;
    }

    /**
     * This friendship is needed so map_placeable_model_gfx
     * can modify the Has_changed member.
     * 
     */
    friend class map_placeable_model_gfx;
}; 

#endif
