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
 * @file   map_placeable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the map_placeable class.
 * 
 * 
 */

#include "map_placeable.h"
#include "landmap.h"

map_placeable::map_placeable(landmap & mymap) : Mymap(mymap)
{
    Type = UNKNOWN;
    State_changed = false;
}
