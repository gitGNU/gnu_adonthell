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
 * @file   map_placeable_model_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_placeable_model_gfx class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_MODEL_GFX_H
#define MAP_PLACEABLE_MODEL_GFX_H

#include "map_placeable_area_gfx.h"
#include "map_placeable_model.h"
#include <map>
#include "fileops.h"

/**
 * The graphical representation of a map_placeable_model.
 *
 * Each object of this class has to be associated at constructor
 * time with a map_placeable_model that it will stick to all life long.
 * 
 */
class map_placeable_model_gfx : public drawable
{
protected:
    mutable map <const string, map_placeable_area_gfx *> Gfxs;
    map <const string, map_placeable_area_gfx *>::iterator Current_gfx;

    map_placeable_model & Target;

public:
    map_placeable_model_gfx (map_placeable_model & target);

    ~map_placeable_model_gfx();
    
    map_placeable_area_gfx * current_gfx();

    void set_gfx (const string & name);

    map_placeable_area_gfx * get_gfx(const string & name);

    map_placeable_area_gfx * add_gfx(const string & name);
    bool del_gfx(const string & name);
    
    bool update ();
    
    /** 
     * Draws the map_placeable representation.
     *
     * @todo: this function should call the Python drawing callback
     * function, if set. Currently it dumbly calls map_placeable_model::draw().
     * 
     * @param x X position where to draw
     * @param y Y position where to draw
     * @param da_opt Optionnal drawing area to use during the blit operation
     * @param target Optionnal surface to draw to (defaults to the screen)
     */
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const;
    
    void put(ogzstream & file) const;

    void get(igzstream & file);

private:
    map_placeable_model_gfx(map_placeable_model_gfx &);
};
    
#endif
