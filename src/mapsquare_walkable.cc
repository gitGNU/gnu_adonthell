/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   mapsquare_walkable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the mapsquare_walkable and mapsquare_walkable_area classes.
 * 
 * 
 */

#include "mapsquare_walkable.h"

mapsquare_walkable::mapsquare_walkable () 
{
    set_walkable (ALL_WALKABLE);
}

s_int8 mapsquare_walkable::get (igzstream& file) 
{
    walkable << file;
    return 0; 
}

s_int8 mapsquare_walkable::put (ogzstream& file) const
{
    walkable >> file;
    return 0; 
}

mapsquare_walkable_area::mapsquare_walkable_area ()
{
}

mapsquare_walkable_area::~mapsquare_walkable_area ()
{
}

void mapsquare_walkable_area::clear ()
{
    area.clear (); 
    
    basex = 0;
    basey = 0;  
} 

s_int8 mapsquare_walkable_area::get (igzstream& file)
{
    vector <vector<mapsquare_walkable> >::iterator it;
    vector <mapsquare_walkable>::iterator jt;
    u_int16 t_length, t_height;
    u_int16 basex_, basey_; 
    
    // Get the area size.
    t_length << file;
    t_height << file; 
    resize_area (t_length, t_height); 
    
    // Load the area.
    for (it = area.begin (); it != area.end (); it++)
        for (jt = it->begin (); jt < it->end (); jt++)
            jt->get (file);
    
    // Load the base square information.
    basex_ << file;
    basey_ << file;  
    set_base (basex_, basey_); 
    
    return 0; 
}

s_int8 mapsquare_walkable_area::put (ogzstream& file) const
{
    vector <vector<mapsquare_walkable> >::iterator it;
    vector <mapsquare_walkable>::iterator jt;

    // Put the area size.
    length () >> file;
    height () >> file; 
    
    // Save the area.
    for (it = area.begin (); it != area.end (); it++)
        for (jt = it->begin (); jt < it->end (); jt++)
            jt->put (file);
    
    // Save the base square information.
    base_x () >> file;
    base_y () >> file; 
    
    return 0; 
}
 
void mapsquare_walkable_area::resize_area (u_int16 nl, u_int16 nh)
{
    vector <vector<mapsquare_walkable> >::iterator i;

    area.resize (nl);
    for (i = area.begin (); i !=  area.end (); i++)
        i->resize (nh);
    
    set_length (nl * MAPSQUARE_SIZE);
    set_height (nh * MAPSQUARE_SIZE); 
}

void mapsquare_walkable_area::set_base (u_int16 nx, u_int16 ny)
{
    basex = nx;
    basey = ny; 
} 

mapsquare_walkable_area & mapsquare_walkable_area::operator = (const mapsquare_walkable_area & src)
{
    u_int16 i, j;
    
    // Clear everything.
    clear (); 
    
    (drawable&) (*this) = (drawable&) src; 

    // Copy the area.
    resize_area (src.area_length (), src.area_height ()); 
    for (i = 0; i < src.area_length (); i++)
        for (j = 0; j < src.area_height (); j++)
            area[i][j] = src.area[i][j]; 
    
    // Copy the base square information.
    set_base (src.base_x (), src.base_y ());
    
    return *this; 
}
