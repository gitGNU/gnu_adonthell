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
 * @file   map_placeable_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the map_placeable_gfx class.
 * 
 * 
 */

#include "map_placeable_gfx.h"

map_placeable_gfx::map_placeable_gfx(map_placeable & target) : map_placeable_model_gfx(target), Target(target)
{
}    
