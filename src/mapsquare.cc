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
 * @file   mapsquare.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the mapsquare and mapsquare_area classes.
 * 
 * 
 */


#include "mapsquare.h"
#include "mapobject.h"
#include "mapcharacter.h"

mapsquare_tile::mapsquare_tile ()
{
    mapobj = NULL; 
    is_base = false;
}

mapsquare_tile::~mapsquare_tile ()
{
}

mapsquare_char::mapsquare_char ()
{
    mchar = NULL;
    is_base = false;
}

mapsquare_char::~mapsquare_char ()
{
}

mapsquare::mapsquare () : mapsquare_walkable () 
{
    base_begin = tiles.end ();
}

mapsquare::mapsquare (const mapsquare& src) : mapsquare_walkable () 
{
    tiles = src.tiles;
    mapchars = src.mapchars; 

    // Correctly place the base tile square's base tile pointer.
    list <mapsquare_tile>::iterator it; 
    for (it = tiles.begin ();
         it != tiles.end () && *(it->base_tile) < *it; it++);
    base_begin = it;
}

mapsquare::~mapsquare ()
{
}

bool mapsquare::is_free () 
{
    list <mapsquare_char>::iterator i;
    for (i = mapchars.begin (); i != mapchars.end (); i++)
        if (i->is_base)
            return false;
    return true;
}

mapcharacter * mapsquare::whoshere () 
{
    list <mapsquare_char>::iterator i;
    for (i = mapchars.begin (); i != mapchars.end (); i++)
        if (i->is_base)
            return i->mchar;
    return NULL;
}

mapsquare_area::mapsquare_area ()
{
}

mapsquare_area::~mapsquare_area ()
{
}

void mapsquare_area::clear ()
{
    area.clear ();      
} 
 
s_int8 mapsquare_area::put_mapobject (u_int16 px, u_int16 py, mapobject * mobj)
{ 
    u_int16 i, j;
    mapsquare_tile t;
    list <mapsquare_tile>::iterator it;
    
    // Calculating where the object will start and end on the map.
    u_int16 i0 = px - mobj->base_x () < 0 ? 0 : px - mobj->base_x (); 
    u_int16 j0 = py - mobj->base_y () < 0 ? 0 : py - mobj->base_y (); 
    
    u_int16 ie = px + (mobj->area_length ()) - mobj->base_x () > area_length () ?
        area_length () : px + (mobj->area_length ()) - mobj->base_x ();  
    u_int16 je = py + (mobj->area_height ()) - mobj->base_y () > area_height () ?
        area_height () : py + (mobj->area_height ()) - mobj->base_y (); 
    
    // Offset between square's position on the map and on the object.
    s_int16 xoff = mobj->base_x () - px; 
    s_int16 yoff = mobj->base_y () - py; 

    
    // First place the base tile, as others refers to it...
    t.mapobj = mobj; 
    t.is_base = true;
    t.x = px;
    t.y = py;
    
    // The iterator will be inserted AFTER all the others base tiles.
    // Doing so, this object will be drawn last on this square.
    for (it = area[px][py].tiles.begin ();
         it != area[px][py].tiles.end () && *(it->base_tile) <= t; it++);
    area[px][py].tiles.insert (it, t);
    it--;
    it->base_tile = it;

    // Update t so it refers to the base tile
    t.base_tile = it;
    t.is_base = false;

    // Now place the others tiles.
    for (j = j0; j < je; j++)
        for (i = i0; i < ie; i++)
        {
            mapsquare & s = area[i][j];
            t.x = i;
            t.y = j;
            s.set_walkable (s.get_walkable () &
                            mobj->get_square (i + xoff, j + yoff)->get_walkable ());
            
            if (i != px || j != py)
            { 
                for (it = s.tiles.begin ();
                     it != s.tiles.end () &&
                         *(it->base_tile) <= *(t.base_tile); it++);
                s.tiles.insert (it, t);
            }
        }
    
    // Correctly place the base tile square's base tile pointer.
    for (it = area[px][py].tiles.begin ();
         it != area[px][py].tiles.end () && *(it->base_tile) < *it; it++);
    area[px][py].base_begin = it;
    
    return 0;
}

void mapsquare_area::remove_mapobject (u_int16 px, u_int16 py, mapobject * mobj)
{
    u_int16 i, j;      
    list <mapsquare_tile>::iterator it;

    // Calculating where the object will start and end on the map.
    u_int16 i0 = px - mobj->base_x () < 0 ? 0 : px - mobj->base_x (); 
    u_int16 j0 = py - mobj->base_y () < 0 ? 0 : py - mobj->base_y (); 
    
    u_int16 ie = px + (mobj->area_length ()) - mobj->base_x () > area_length () ?
        area_length () - 1 : px + (mobj->area_length ()) - mobj->base_x () - 1;  
    u_int16 je = py + (mobj->area_height ()) - mobj->base_y () > area_height () ?
        area_height () - 1 : py + (mobj->area_height ()) - mobj->base_y () - 1; 
    
    // Find the base tile, get it's reference (to remove others) and erase it.
    for (it = area[px][py].tiles.begin (); it != area[px][py].tiles.end () &&
             (it->is_base ==  true && it->mapobj == mobj); it++);

    // Base tile not found - better to return now...
    if (it == area[px][py].tiles.end ()) return;

    // Keep the iterator available for further comparison
    list <mapsquare_tile>::iterator the_base = it;

    // Erase the base tile
    area[px][py].tiles.erase (it);
    
    // And now erase all the others tiles of this object.
    for (j = j0; j < je; j++)
        for (i = i0; i < ie; i++)
        {
            if (i != px || j != py)
            {
                mapsquare & s = area[i][j]; 
                
                for (it = s.tiles.begin (); it->base_tile != the_base; it++); 
                s.tiles.erase (it);
                
                // Recalculate the walkability of this square.
                s.set_walkable (NONE_WALKABLE);
                for (it = s.tiles.begin (); it != s.tiles.end (); it++)
                {
                    u_int16 wx = it->x - (it->base_tile->x - it->mapobj->base_x ());
                    u_int16 wy = it->y - (it->base_tile->y - it->mapobj->base_y ());
                    s.set_walkable (s.get_walkable () &
                                    it->mapobj->get_square (wx, wy)->get_walkable ());                     
                }
            }
        } 
}

void mapsquare_area::resize_area (u_int16 nl, u_int16 nh)
{
    vector <vector<mapsquare> >::iterator i;

    area.resize (nl);
    for (i = area.begin (); i !=  area.end (); i++)
        i->resize (nh);
}