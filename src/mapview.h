/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   mapview.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapview class.
 * 
 * 
 */

 

#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include "mapobject.h"
#include "landmap.h"
#include "py_inc.h"
#include <list>

class mapsquare_tile;
class mapsquare_char;
class landmap;

class mapview : public drawable
{
    u_int16 d_length, d_height;	// size of the view in map squares

    u_int16 currentsubmap;
    u_int16 posx, posy;
    u_int16 offx, offy;

    u_int16 draw_offx, draw_offy;
 
    landmap *m_map;

    drawing_area *da;

    mutable list <mapsquare_tile> critical_draw;
    mutable list <mapsquare_char> characters_draw;

    PyObject *locals;			// Locals that belong to that mapview
    py_script schedule;
    string schedule_file_; 

public:
    void init ();
    mapview ();
    ~mapview ();
 
    void attach_map (landmap * m);
    void detach_map ();

    s_int8 set_current_submap (u_int16 sm);
    s_int8 set_pos (u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);
    s_int8 center_on (u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);

    bool can_scroll_right ()
    {
        return (posx != m_map->submap[currentsubmap]->area_length () - d_length);
    }
    
    bool can_scroll_left ()
    {
        return (posx || offx);
    }
    bool can_scroll_up ()
    {
        return (posy || offy);
    }
    bool can_scroll_down ()
    {
        return (posy != m_map->submap[currentsubmap]->area_height () - d_height);
    }

    void scroll_right ();
    void scroll_left ();
    void scroll_down ();
    void scroll_up ();
    u_int16 get_current_submap ()
    {
        return currentsubmap;
    }
    u_int16 get_posx ()
    {
        return posx;
    }
    u_int16 get_posy ()
    {
        return posy;
    }
    void resize (u_int16 l, u_int16 h);

    s_int8 get_state (igzstream& file);
    s_int8 put_state (ogzstream& file);

    void set_schedule (string file);

    void update ();
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface *target = NULL) const;
 

private:
#ifndef SWIG
    void draw_tile (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target,
                    list<mapsquare_tile>::iterator it) const;
    
    void draw_mapchar (s_int16 x, s_int16 y, const drawing_area * da_opt,
                       surface * target, list<mapsquare_char>::iterator itc) const; 

#endif
    
};

#endif
