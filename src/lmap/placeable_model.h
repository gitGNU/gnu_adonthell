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
 * @file   placeable_model.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_model class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_MODEL_H
#define MAP_PLACEABLE_MODEL_H

#include <map>
#include <string>

#include "lmap/placeable_area.h"
#include "fileops.h"

namespace lmap
{

    /**
     * Represents a placeable, i.e. something (character, object, ...)
     * that can be placed on a map and occupies some space on it.
     *
     * This class is separated from placeable to allow easy edition
     * without depending on the whole map stuff. While it is suitable
     * for edition, it can't be actually placed on a map. Use placeable
     * for that instead.
     * 
     */
    class placeable_model
    {
    protected:
        mutable std::string Filename;

        mutable std::map <std::string, placeable_area> States;
        std::map <std::string, placeable_area>::iterator Current_state;

        bool State_changed;
    
    public:
        typedef std::map <std::string, placeable_area>::iterator iterator;

        iterator begin () 
        {
            return States.begin (); 
        }

        iterator end () 
        {
            return States.end (); 
        }

        placeable_model();

        placeable_area * current_state();

        placeable_area * get_state(const std::string & name);
    
        const std::string current_state_name();
    
        placeable_area * add_state (const std::string & name);

        bool del_state (const std::string & name);
    
        void set_state (const std::string & name);

        void put(ogzstream & file) const;

        void get(igzstream & file);

        const std::string & filename()
        {
            return Filename;
        }

        /**
         * This friendship is needed so placeable_model_gfx
         * can modify the Has_changed member.
         * 
         */
        friend class placeable_model_gfx;
    }; 
}

#endif
