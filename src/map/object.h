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
 * @file   object.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the object class.
 * 
 * 
 */

#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "placeable.h"
#include "fileops.h"

namespace map
{

    /**
     * Implements a "landscape" map object.
     *
     * A object is a placeable that has some update abilities and can be
     * placed several times on the same map. Note, however, that all instances of
     * the same map object will have the same state and will always look the same.
     * While this is most often ok, some special object (like doors) will require
     * you to load one object per instance.
     * 
     */
    class object : public placeable
    {
    public:
        object(landmap & mymap); 

        bool update()
        {
            return true;
        }

        void put(ogzstream & file) const;
        void get(igzstream & file);

        s_int8 save(const std::string fname) const;
        s_int8 load(const std::string fname);
    };
}

#endif
