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
 * @file   placeable_gfx.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the placeable_gfx class.
 * 
 * 
 */


#ifndef MAP_PLACEABLE_GFX_H
#define MAP_PLACEABLE_GFX_H

#include <map>

#include "lmap/placeable_model_gfx.h"
#include "lmap/placeable.h"

namespace lmap
{

    /**
     * The graphical representation of a placeable.
     *
     * Each object of this class has to be associated at constructor
     * time with a placeable_model that it will stick to all life long.
     * 
     */
    class placeable_gfx : public placeable_model_gfx
    {
    private:
        placeable & Target;

    public:
        placeable_gfx(placeable & target);
    }; 
}

#endif
