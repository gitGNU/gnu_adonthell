/*
   $Id$

   Copyright (C) 1999/2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream>
#include "win_manager.h"
#include "win_theme.h"
#include "input.h"
#include "screen.h"
#include "game.h"
#include "character.h"
#include "quest.h"
#include "mapengine.h"

#include "python_class.h"
 
#include "audio.h"


/**
 * @file   game.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the game class.
 * 
 * 
 */



initflags game::initiated = INIT_NONE;
PyObject * game::py_module = NULL;

/*
 * SWIG init prototypes. Should we use dynamic linking??? 
 */
extern "C"
{
    /** 
     * SWIG init prototype.
     * 
     */
    void initadonthellc (void);
}


// Initialize all parts of the game engine
bool game::init (config & configuration, initflags to_init = INIT_ALL)
{
    // init game loading/saving system
    if (to_init & INIT_SAVES) 
    { 
        gamedata::init (configuration.get_adonthellrc (), configuration.gamedir, configuration.game_name); 
    }
    
    // init video subsystem
    if (to_init & INIT_VIDEO) 
    { 
        screen::set_video_mode (320, 240);
        screen::set_fullscreen (configuration.screen_mode); 
    }
     
    // init audio subsystem
    if (to_init & INIT_AUDIO)
    {
        if (configuration.audio_volume > 0)
            audio::init (&configuration);
        else
            to_init = (enum initflags) (to_init ^ INIT_AUDIO);
    }
    
    // init input subsystem
    if (to_init & INIT_INPUT) 
    { 
        input::init ();
    }
    
    // init python interpreter
    if (to_init & INIT_PYTHON)
    { 
        python::init (); 
        
        // initialise python import paths, SWIG module and globals
        init_python (); 
        // init the game data
        init_data (); 
    }

    // init window manager
    if (to_init & INIT_WIN) 
    {
        win_manager::init (); 
    }
    
    initiated = to_init;
    
    // voila :)
    return true;
}

// Cleanup everything
void game::cleanup () 
{
    // close all windows
    // delete all themes and fonts
    if (initiated & INIT_WIN)
    { 
        win_manager::cleanup (); 
    }
    
    // shutdown input subsystem
    if (initiated & INIT_INPUT) 
    { 
        input::shutdown ();
    }

    // shutdown audio
    if (initiated & INIT_AUDIO)
    {
        audio::cleanup ();
    }

    // cleanup the saves
    if (initiated & INIT_SAVES) 
    {
        gamedata::cleanup (); 
    }
    
    // shutdown python
    if (initiated & INIT_PYTHON) 
    {
        cleanup_data ();
        cleanup_python (); 
        python::cleanup ();     
    }

    // shutdown video and SDL
    if (initiated & INIT_VIDEO)
    {
        SDL_Quit ();
    }     
}

void game::init_data ()
{
    data::map_engine = new mapengine;
    data::the_player = NULL;
}

void game::cleanup_data () 
{
    delete data::map_engine;
    data::map_engine = NULL;

    if (data::the_player)
    { 
        delete data::the_player;
        data::the_player = NULL;
    }
}

bool game::init_python () 
{
    // Initialise the import path.
    // Shared modules path 
    python::insert_path (DATA_DIR"/modules"); 

    // Game specific path
    string t = gamedata::game_data_dir () + "/scripts/modules"; 
    python::insert_path((char *) t.c_str ());
    t = gamedata::game_data_dir () + "/scripts"; 
    python::insert_path((char *) t.c_str ());

    /* Initialise SWIG module. This should go if we ever switch to dynamic 
       link */
    initadonthellc();
        
    py_module = python::import_module ("adonthell"); 
    if (!py_module)
        return false;     
    
    data::globals = PyModule_GetDict (py_module);

    return true; 
}

void game::cleanup_python () 
{
    // Cleanup the global namespace of python interpreter
    // Note that we don't have to DECREF data::globals, because they're a
    // borrowed reference of py_module.
    Py_DECREF (py_module); 
}
