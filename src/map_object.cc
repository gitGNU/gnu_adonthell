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

#include "map_object.h"


map_object::map_object(landmap & mymap) : map_placeable(mymap) 
{
    Type = OBJECT; 
}

void map_object::put(ogzstream & file) const
{
    map_placeable_model::put(file);
}

void map_object::get(igzstream & file)
{
    map_placeable_model::get(file);
}

s_int8 map_object::save(string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    return ret;
}

s_int8 map_object::load(string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    return ret;
}
