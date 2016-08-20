/*
   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
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
    can_use_for_pathfinding = true; 
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
    can_use_for_pathfinding = src.can_use_for_pathfinding; 
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
        area_length () : px + (mobj->area_length ()) - mobj->base_x ();  
    u_int16 je = py + (mobj->area_height ()) - mobj->base_y () > area_height () ?
        area_height () : py + (mobj->area_height ()) - mobj->base_y (); 
    
    // Find the base tile, get it's reference (to remove others).
    for (it = area[px][py].tiles.begin (); it != area[px][py].tiles.end () &&
           !(it->is_base == true && it->mapobj == mobj); it++); 
    
    // Base tile not found - better to return now...
    if (it == area[px][py].tiles.end ()) return;
    
    // Keep the iterator available for further comparison
    list <mapsquare_tile>::iterator the_base = it;
     
    // And now erase all the others tiles of this object. 
    for (j = j0; j < je; j++)
        for (i = i0; i < ie; i++)
        {
            if (i != px || j != py)
            {
                mapsquare & s = area[i][j]; 
                
                for (it = s.tiles.begin (); it != s.tiles.end () && it->base_tile != the_base; it++); 

                // Not found?? Weird - let's not mess with it then!
                if (it == s.tiles.end ()) continue; 

                s.tiles.erase (it);
                
                // Recalculate the walkability of this square.
                s.set_walkable (ALL_WALKABLE);
                for (it = s.tiles.begin (); it != s.tiles.end (); it++)
                {
                    u_int16 wx = it->x - (it->base_tile->x - it->mapobj->base_x ());
                    u_int16 wy = it->y - (it->base_tile->y - it->mapobj->base_y ());
                    s.set_walkable (s.get_walkable () &
                                    it->mapobj->get_square (wx, wy)->get_walkable ());                     
                }
            }
        } 
    mapsquare & s = area[px][py]; 
    // Erase the base tile
    s.tiles.erase (the_base);
    // Recalculate the walkability of this square.
    s.set_walkable (ALL_WALKABLE);
    for (it = s.tiles.begin (); it != s.tiles.end (); it++)
    {
	u_int16 wx = it->x - (it->base_tile->x - it->mapobj->base_x ());
	u_int16 wy = it->y - (it->base_tile->y - it->mapobj->base_y ());
	s.set_walkable (s.get_walkable () &
			it->mapobj->get_square (wx, wy)->get_walkable ());                     
    }
}

void mapsquare_area::resize_area (u_int16 nl, u_int16 nh)
{
    vector <vector<mapsquare> >::iterator i;

    area.resize (nl);
    for (i = area.begin (); i !=  area.end (); i++)
        i->resize (nh);

    u_int16 j, k;
    for (j = 0; j < nl; j++)
        for (k = 0; k < nh; k++)
        {
            area[j][k].x_ = j; 
            area[j][k].y_ = k;
        }
}
