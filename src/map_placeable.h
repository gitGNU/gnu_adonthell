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
 * @file   map_placeable.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_placeable class.
 * 
 * 
 */

#ifndef MAP_PLACEABLE_H
#define MAP_PLACEABLE_H

#include "map_placeable_model.h"
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

/**
 * Class representing a placeable, i.e. something (character, object, ...)
 * that can be placed on a map and occupies some space on it.
 *
 * What makes this class different from map_placeable_model is that it has a type,
 * that higher-level class can overwrite to indicate what kind of placeable it is,
 * and a reference to the map the placeable belongs to, so update functions can
 * check the terrain around the placeable.
 * 
 */
class map_placeable : public map_placeable_model
{
protected:

    placeable_type Type; 

    landmap & Mymap;
    
public:
//     map_coordinates base; 
    
    map_placeable(landmap & mymap);
    
    placeable_type type () 
    {
        return Type; 
    }
};

#endif
