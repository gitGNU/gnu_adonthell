/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _MAPENGINE_H
#define _MAPENGINE_H

#include "screen.h"
#include "input.h"
#include "fileops.h"
#include "landmap.h" 
#include "win_mapview.h"

class mapengine
{
public:
    mapengine ();
    ~mapengine ();
    
    void set_mapview_schedule (string s);
    void load_map (string fname);

    landmap *get_landmap ()
    {
        return &lmap;
    }

    mapview *get_mapview ()
    {
        return (mapview*) &mv;
    }

    void run (); 
    void quit ()
    {
        letsexit = true;
    }

    s_int8 get_state (igzstream& file);
    s_int8 put_state (ogzstream& file);

    void mainloop ();

    void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL,
               surface * target = NULL)
    {
        mv.mapview::draw (x, y, da_opt, target); 
    }

protected:
    landmap lmap;
    win_mapview mv;
    bool letsexit;
};

#ifndef SWIG
namespace data
{
    /**
     * Map engine used during the game.
     * 
     */ 
    extern mapengine * map_engine;
}
#endif

#endif
