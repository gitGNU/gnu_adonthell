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
 * @file   placeable_model_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_model_gfx class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_MODEL_GFX_H
#define MAP_PLACEABLE_MODEL_GFX_H

#include <map>

#include "lmap/placeable_area_gfx.h"
#include "lmap/placeable_model.h"
#include "fileops.h"

namespace lmap
{

    /**
     * The graphical representation of a placeable_model.
     *
     * Each object of this class has to be associated at constructor
     * time with a placeable_model that it will stick to all life long.
     * 
     */
    class placeable_model_gfx : public gfx::drawable
    {
    protected:
        mutable std::map <const std::string, placeable_area_gfx *> Gfxs;
        std::map <const std::string, placeable_area_gfx *>::iterator Current_gfx;

        placeable_model & Target;

    public:
        placeable_model_gfx (placeable_model & target);

        ~placeable_model_gfx();
    
        placeable_area_gfx * current_gfx();

        void set_gfx (const std::string & name);

        placeable_area_gfx * get_gfx(const std::string & name);

        placeable_area_gfx * add_gfx(const std::string & name);
        bool del_gfx(const std::string & name);
    
        bool update ();
    
        /** 
         * Draws the placeable representation.
         *
         * @todo: this function should call the Python drawing callback
         * function, if set. Currently it dumbly calls placeable_model::draw().
         * 
         * @param x X position where to draw
         * @param y Y position where to draw
         * @param da_opt Optionnal drawing area to use during the blit operation
         * @param target Optionnal surface to draw to (defaults to the screen)
         */
        void draw(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                  gfx::surface * target = NULL) const;
    
        void draw_walkable(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                           gfx::surface * target = NULL) const;

        void draw_border(s_int16 x, s_int16 y, const gfx::drawing_area * da_opt = NULL,
                         gfx::surface * target = NULL) const;

        void put(ogzstream & file) const;

        void get(igzstream & file);

    private:
        placeable_model_gfx(placeable_model_gfx &);
    };
}
    
#endif
