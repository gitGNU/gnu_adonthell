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

#ifndef MAP_PLACEABLE_GFX_H
#define MAP_PLACEABLE_GFX_H

#include "map_placeable_model_gfx.h"
#include "map_placeable.h"
#include <map>

class map_placeable_gfx : public map_placeable_model_gfx
{
private:
    map_placeable & Target;

public:
    map_placeable_gfx(map_placeable & target);
}; 

#endif
