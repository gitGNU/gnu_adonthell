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
 * @file   map_character_with_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_character_with_gfx class.
 * 
 * 
 */


#ifndef MAP_CHARACTER_WITH_GFX_H
#define MAP_CHARACTER_WITH_GFX_H

#include "map_character.h"
#include "map_placeable_gfx.h"

/**
 * A map_character along with it's graphical representation.
 * 
 */
class map_character_with_gfx : public map_character, public map_placeable_gfx
{
public:
    map_character_with_gfx (landmap & mymap); 

    void put(ogzstream & file) const;
    void get(igzstream & file);

    s_int8 save(string fname) const;
    s_int8 load(string fname);

    void draw(s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL);
    void draw_shadow(s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                     surface * target = NULL);
private:
    image shadow;

}; 

#endif
