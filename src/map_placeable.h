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

class map_placeable : public map_placeable_model
{
protected:

    placeable_type Type; 

    landmap & Mymap;
    
public:
    map_coordinates base; 
    
    map_placeable(landmap & mymap);
    
    placeable_type type () 
    {
        return Type; 
    }
};

#endif
