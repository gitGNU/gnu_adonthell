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

#ifndef MAP_OBJECT_WITH_GFX
#define MAP_OBJECT_WITH_GFX

#include "map_object.h"
#include "map_placeable_gfx.h"

class map_object_with_gfx : public map_object, public map_placeable_gfx
{
public:
    map_object_with_gfx (landmap & mymap); 

    void put(ogzstream & file) const;
    void get(igzstream & file);

    s_int8 save(string fname) const;
    s_int8 load(string fname);
}; 

#endif
