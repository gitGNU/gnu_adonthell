/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#include "fileops.h"
#include "mapengine.h"
#include "win_manager.h"
#include "gametime.h"
#include "character.h"

#include "gamedata.h"

#include <sys/time.h>
#include <unistd.h>

mapengine * data::map_engine; 

mapengine::mapengine ()
{
    mv.mapview::resize (screen::length (), screen::height ());
    letsexit = false;
    should_update_map_ = true; 
}

mapengine::~mapengine ()
{
}

void mapengine::set_mapview_schedule (string s, PyObject * args = NULL)
{
    mv.mapview::set_schedule (s, args);
}

void mapengine::load_map (string fname)
{
    lmap.load (fname);
    mv.mapview::attach_map (&lmap);
}

void mapengine::run ()
{
    letsexit = false;
    mv.set_visible (true);
    mv.pack ();
    win_manager::add (&mv);
    gametime::start_action (); 

    while (1) 
    {
        mainloop ();
        if (letsexit) break; 
        screen::show ();
        gametime::update (); 
    }
    
    win_manager::remove (&mv);
}

void mapengine::mainloop ()
{
    input::update ();
    for (int i = 0; i < gametime::frames_to_do (); i++)
    {
        win_manager::input_update ();
        if (should_update_map ()) lmap.update ();
        win_manager::update ();
    }
    screen::clear (); 
    win_manager::draw ();
}

// fade the screen out
void mapengine::fade_out ()
{
    s_int16 i = 0;

    while (i <= 60)
    {
        if (i > 60) i = 60;

        mainloop ();

        screen::transition (i * 2);
        screen::show ();

        gametime::update ();
        i = i + (gametime::frames_to_do () * 2);
    }
}

// fade the screen in
void mapengine::fade_in ()
{
    s_int16 i = 60;

    while (i >= 0)
    {
        if (i < 0) i = 0;

        mainloop ();

        screen::transition (i * 2);
        screen::show ();

        gametime::update ();
        i = i - (gametime::frames_to_do () * 2);
    }
}

s_int8 mapengine::get_state (igzstream& file)
{
    u_int16 nbr_of, i;
    string t;

    // Get the map filename 
    t << file; 

    // Load the map from the file
    lmap.get (file); 
    // Load the map state (events)
    if (!lmap.get_state (file))
        return false; 

    mv.mapview::attach_map (&lmap);

    // Load the mapcharacters
    nbr_of << file; 

    for (i = 0; i < nbr_of; i++)
    {
        t << file;
        mapcharacter *mc = (mapcharacter *) data::characters[t.c_str ()];
        
        mc->set_map (&lmap);
        mc->get_state (file);
    }

    // Load the mapview state
    mv.mapview::get_state (file);
    mv.pack (); 
    return true;
}

s_int8 mapengine::put_state (ogzstream& file)
{
    u_int16 nbr_of, i;
    
    // Save the map filename
    string t = lmap.filename ();
    t >> file; 

    // Save the map itself
    lmap.put (file); 
    // Save the map state (events)
    lmap.put_state (file); 

    // Save the mapcharacters and their status
    nbr_of = lmap.nbr_of_mapcharacters ();
    nbr_of >> file; 

    for (i = 0; i < nbr_of; i++)
    {
        mapcharacter * mc = lmap.get_mapcharacter (i); 
        t = mc->get_name ();
        t >> file; 
        mc->put_state (file);
    }

    // Save the mapview state
    mv.mapview::put_state (file);
    return 0;
}
