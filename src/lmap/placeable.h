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
 * @file   placeable.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable class.
 * 
 * 
 */

#ifndef MAP_PLACEABLE_H
#define MAP_PLACEABLE_H

#include <string>
#include <map>

#include "lmap/placeable_model.h"

namespace lmap
{

    class landmap;

    typedef enum 
        {
            UNKNOWN = 0, 
            OBJECT,
            CHARACTER, 
            ITEM
        } placeable_type; 

    /**
     * Class representing a placeable, i.e. something (character, object, ...)
     * that can be placed on a map and occupies some space on it.
     *
     * What makes this class different from placeable_model is that it has a type,
     * that higher-level class can overwrite to indicate what kind of placeable it is,
     * and a reference to the map the placeable belongs to, so update functions can
     * check the terrain around the placeable.
     * 
     */
    class placeable : public placeable_model
    {
    protected:

        placeable_type Type; 

        landmap & Mymap;
    
    public:
        //     coordinates base; 
    
        placeable(landmap & mymap);
    
        placeable_type type () const
        {
            return Type; 
        }
    };
}

#endif
