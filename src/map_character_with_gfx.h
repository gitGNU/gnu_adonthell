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

#ifndef MAP_CHARACTER_WITH_GFX_H
#define MAP_CHARACTER_WITH_GFX_H

#include "map_character.h"
#include "map_placeable_gfx.h"

class map_character_with_gfx : public map_character, public map_placeable_gfx
{
public:
    map_character_with_gfx (landmap & mymap); 
}; 

#endif
