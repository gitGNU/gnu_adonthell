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
 * @file   map_placeable_model_gfx.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the map_placeable_model_gfx class.
 * 
 * 
 */

#include "map_placeable_model_gfx.h"

map_placeable_model_gfx::map_placeable_model_gfx (map_placeable_model & target) : Target(target)
{
    Current_gfx = Gfxs.begin (); 
}

map_placeable_model_gfx::~map_placeable_model_gfx()
{
    for (Current_gfx = Gfxs.begin(); Current_gfx != Gfxs.end(); Current_gfx++)
        delete Current_gfx->second;
}

void map_placeable_model_gfx::set_gfx (const string & name) 
{
    if (Current_gfx != Gfxs.end() && Current_gfx->first == name)
        return;
    
    if (Current_gfx != Gfxs.end())
    {
        Current_gfx->second->stop();
        Current_gfx->second->rewind();
    }
    
    map <const string, map_placeable_area_gfx *>::iterator Previous_gfx;
    Previous_gfx = Current_gfx;
    Current_gfx = Gfxs.find (name); 
    
    if (Current_gfx != Gfxs.end())
        Current_gfx->second->play();
    else Current_gfx = Previous_gfx;
}

map_placeable_area_gfx * map_placeable_model_gfx::get_gfx(const string & name)
{
    map <const string, map_placeable_area_gfx *>::iterator Gfx;
    Gfx = Gfxs.find (name); 
    
    if (Gfx != Gfxs.end())
        return Gfx->second;
    else return NULL;
}

map_placeable_area_gfx * map_placeable_model_gfx::add_gfx(const string & name)
{
    map_placeable_area_gfx * newmpa = new map_placeable_area_gfx();
    
    return (Gfxs.insert(pair<const string, map_placeable_area_gfx *>
                        (name, newmpa))).first->second;
}

bool map_placeable_model_gfx::del_gfx(const string & name)
{
    map <const string, map_placeable_area_gfx *>::iterator it = Gfxs.find(name);
    if (it == Gfxs.end()) return false;
    delete it->second;
    return true;
}

bool map_placeable_model_gfx::update() 
{
    if (Target.State_changed)
    {
        Target.State_changed = false;
        set_gfx (Target.current_state_name()); 
    }
    
    if (Current_gfx != Gfxs.end ())
        Current_gfx->second->update ();
    
    return true;
}

void map_placeable_model_gfx::draw (s_int16 x, s_int16 y, const drawing_area * da_opt,
                                    surface * target) const
{
    if (Current_gfx != Gfxs.end ())
    {
        if (Target.Current_state != Target.States.end())
        {
            const map_placeable_area & t = Target.Current_state->second;
            Current_gfx->second->draw (x - (t.base.x()) * 
                                       mapsquare_size - t.base.ox(), 
                                       y - (t.base.y()) * 
                                       mapsquare_size - t.base.oy(), 
                                       da_opt, target); 
        }
        else
            Current_gfx->second->draw (x, y, da_opt, target); 
    }
}

void map_placeable_model_gfx::draw_walkable(s_int16 x, s_int16 y, 
                                            const drawing_area * da_opt,
                                            surface * target) const
{
    image im(mapsquare_size, mapsquare_size);
    im.fillrect(0, 0, im.length(), im.height(), 0xFF, 0, 0);
    im.set_alpha(128);
    
    map_placeable_area * st = Target.current_state();
    

    for (int j = 0; j < st->area_height(); j++)
        for (int i = 0; i < st->area_length(); i++)
        {
            if (!st->get(i, j).is_walkable())
            {
            im.draw(x - (st->base.x() - i) * 
                    mapsquare_size - st->base.ox(), 
                    y - (st->base.y() - j) * 
                    mapsquare_size - st->base.oy(), 
                    da_opt, target); 
            }
        }
}

void map_placeable_model_gfx::draw_border(s_int16 x, s_int16 y, 
                                          const drawing_area * da_opt,
                                          surface * target) const
{
    map_placeable_area * st = Target.current_state();

    x -= st->base.x() * mapsquare_size + st->base.ox();
    y -= st->base.y() * mapsquare_size + st->base.oy();

    screen::display.fillrect(x, y, st->area_length() * mapsquare_size, 1, 0xFFFFFF);
    screen::display.fillrect(x, y, 1, st->area_height() * mapsquare_size, 0xFFFFFF);
    screen::display.fillrect(x + st->area_length() * mapsquare_size - 1, y, 1, st->area_height() * mapsquare_size, 0xFFFFFF);
    screen::display.fillrect(x, y + st->area_height() * mapsquare_size - 1, st->area_length() * mapsquare_size, 1, 0xFFFFFF);
}

void map_placeable_model_gfx::put(ogzstream & file) const
{
    u_int32 s = Gfxs.size();
    s >> file;
    
    for (map <const string, map_placeable_area_gfx *>::iterator i = Gfxs.begin();
         i != Gfxs.end(); i++)
    {
        i->first >> file;
        i->second->put(file);
    }   
}

void map_placeable_model_gfx::get(igzstream & file)
{
    u_int32 size;
    
    size << file;
    
    for (u_int32 i = 0; i < size; i++)
    {
        string s;
        s << file;
        
        map_placeable_area_gfx * mpa = add_gfx(s);
        mpa->get(file);
        
    }
}

map_placeable_area_gfx * map_placeable_model_gfx::current_gfx()
{
    if (Current_gfx != Gfxs.end ())
        return Current_gfx->second;
    else return NULL; 
}
