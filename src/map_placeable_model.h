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

#ifndef MAP_PLACEABLE_MODEL_H
#define MAP_PLACEABLE_MODEL_H

#include "map_placeable_area_gfx.h"
#include <map>
#include "fileops.h"

class map_placeable_model : public drawable
{
protected:
    mutable map <const string, map_placeable_area_gfx> Gfxs;
    map <const string, map_placeable_area_gfx>::iterator Current_gfx;

public:
    map_placeable_model ()
    {
        Current_gfx = Gfxs.begin (); 
    }
    
    void set_gfx (const string & name) 
    {
        if (Current_gfx != Gfxs.end() && Current_gfx->first == name)
            return;

        if (Current_gfx != Gfxs.end())
        {
            Current_gfx->second.stop();
            Current_gfx->second.rewind();
        }

        map <const string, map_placeable_area_gfx>::iterator Previous_gfx;
        Previous_gfx = Current_gfx;
        Current_gfx = Gfxs.find (name); 

        if (Current_gfx != Gfxs.end())
            Current_gfx->second.play();
        else Current_gfx = Previous_gfx;
    }

    map_placeable_area_gfx * get_gfx(const string & name)
    {
        map <const string, map_placeable_area_gfx>::iterator Gfx;
        Gfx = Gfxs.find (name); 

        if (Gfx != Gfxs.end())
            return &(Gfx->second);
        else return NULL;
    }

    map_placeable_area_gfx * add_gfx(const string & name, map_placeable_area & target)
    {
        return &((Gfxs.insert(pair<const string, const map_placeable_area_gfx> (name, map_placeable_area_gfx(target)))).first->second);
    }

    bool update () 
    {
        if (Current_gfx != Gfxs.end ())
            Current_gfx->second.update ();
        
        return true;
        
    }
    
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface * target = NULL) const
    {
        if (Current_gfx != Gfxs.end ())
            Current_gfx->second.draw (x, y, da_opt, target); 
    }

    void put(ogzstream & file) const
    {
        Gfxs.size() >> file;

        for (map <const string, map_placeable_area_gfx>::iterator i = Gfxs.begin();
             i != Gfxs.end(); i++)
        {
            i->first >> file;
            i->second.put(file);
        }

    }

    void get(igzstream & file, map_placeable_area & target)
    {
        int size;

        size << file;

        for (int i = 0; i < size; i++)
        {
            string s;
            s << file;

            map_placeable_area_gfx * mpa = add_gfx(s, target);
            mpa->get(file);
            
        }
    }
}; 

#endif
