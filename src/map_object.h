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

#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "map_placeable.h"
#include "fileops.h"

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
