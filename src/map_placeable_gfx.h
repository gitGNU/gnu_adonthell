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
 * @file   map_placeable_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_placeable_gfx class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_GFX_H
#define MAP_PLACEABLE_GFX_H

#include "map_placeable_model_gfx.h"
#include "map_placeable.h"
#include <map>

/**
 * The graphical representation of a map_placeable.
 *
 * Each object of this class has to be associated at constructor
 * time with a map_placeable_model that it will stick to all life long.
 * 
 */
class map_placeable_gfx : public map_placeable_model_gfx
{
private:
    map_placeable & Target;

public:
    map_placeable_gfx(map_placeable & target);
}; 

#endif
