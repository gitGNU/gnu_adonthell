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


#include <iostream>
#include "fileops.h"
#include "mapengine.h"
#include "win_manager.h"

mapengine::mapengine ()
{
    mv.resize (screen::length (), screen::height ());
    letsexit = false;
}

mapengine::~mapengine ()
{
    win_manager::destroy ();
}

void mapengine::set_mapview_schedule (string s)
{
    mv.set_schedule (s);
}

void mapengine::load_map (string fname)
{
    lmap.load (fname);
    mv.attach_map (&lmap);
}

void mapengine::run ()
{
    gametime::start_action (); 
    while (!letsexit)
    {
        mainloop ();
        screen::show ();
        gametime::update (); 
        if (input::has_been_pushed (SDLK_ESCAPE))
        { 
            cout << "exit\n"; 
            letsexit = true;
        }
    }
}

void mapengine::mainloop ()
{
    input::update ();
    for (int i = 0; i < gametime::frames_to_do (); i++)
    {
        win_manager::update ();
        lmap.update ();
        mv.update ();
    }
    mv.draw (0, 0);
    win_manager::draw ();
}

s_int8 mapengine::get_state (igzstream& file)
{
    u_int16 nbr_of, i;
    string t;

    // Load the map from the filename 
    t << file; 
    load_map (t);

    // Load the map state (events)
    if (!lmap.get_state (file))
        return false; 

    // Load the mapcharacters
    nbr_of << file; 

    for (i = 0; i < nbr_of; i++)
    {
        t << file; 
        mapcharacter *mc = (mapcharacter *) (data::characters.get (t.c_str ()));
        
        mc->set_map (&lmap);
        mc->get_state (file);
    }
    // Load the mapview state
    mv.get_state (file);
    
    return true;
}

s_int8 mapengine::put_state (ogzstream& file)
{
    u_int16 nbr_of, i;
    
    // Save the map filename
    string t = lmap.filename ();
    t >> file; 

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
    mv.put_state (file);
    return 0;
}
