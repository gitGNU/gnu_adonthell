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
 * @file   map_object.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_object class.
 * 
 * 
 */

#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "map_placeable.h"
#include "fileops.h"

/**
 * Implements a "landscape" map object.
 *
 * A map_object is a map_placeable that has some update abilities and can be
 * placed several times on the same map. Note, however, that all instances of
 * the same map object will have the same state and will always look the same.
 * While this is most often ok, some special object (like doors) will require
 * you to load one object per instance.
 * 
 */
class map_object : public map_placeable
{
public:
    map_object(landmap & mymap); 

    bool update()
    {
        return true;
    }

    void put(ogzstream & file) const;
    void get(igzstream & file);

    s_int8 save(string fname) const;
    s_int8 load(string fname);
};

#endif
