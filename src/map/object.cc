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
 * @file   object.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the object class.
 * 
 * 
 */

#include "object.h"

using namespace map;

object::object(landmap & mymap) : placeable(mymap) 
{
    Type = OBJECT; 
}

void object::put(ogzstream & file) const
{
    placeable_model::put(file);
}

void object::get(igzstream & file)
{
    placeable_model::get(file);
    set_state("default");
}

s_int8 object::save(const std::string fname) const
{
    ogzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    put (file);
    file.close (); 
    return ret;
}

s_int8 object::load(const std::string fname)
{
    igzstream file (fname);
    s_int8 ret = 0; 
    
    if (!file.is_open ())
        return 1;
    get (file);
    file.close (); 
    return ret;
}
