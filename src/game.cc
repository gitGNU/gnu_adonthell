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

#include "python.h"
 
#ifdef SDL_MIXER
#include "audio.h"
#endif


/**
 * @file   game.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the game class.
 * 
 * 
 */



config * game::configuration = NULL; 
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
bool game::init (int argc, char **argv)
{
    if (configuration) delete configuration;
    
    configuration = new config (argc > 1 ? argv[1] : "");
    
    // try to read adonthellrc
    if (!configuration->read_adonthellrc ())
        return false;

    // init game loading/saving system
    gamedata::init (configuration->get_adonthellrc (), configuration->datadir); 
    
    // init video subsystem
    screen::set_video_mode (320, 240);
    screen::set_fullscreen (configuration->screen_mode);
    
    // set the theme
    win_theme::theme = new char[strlen(configuration->window_theme.c_str ())];
    strcpy (win_theme::theme, configuration->window_theme.c_str ());
    strcat (win_theme::theme,"/");

#if defined SDL_MIXER
    // init audio subsystem
    audio::init (configuration);
#endif 

    // init input subsystem
    input::init ();
    
    // init python interpreter
    python::init (); 

    // initialise python import paths, SWIG module and globals
    init_python (); 
         
    // init the game data
    init_data (); 
    
    // voila :)
    return true;
}

// Cleanup everything
void game::cleanup () 
{
    // close all windows
    win_manager::destroy(); 
    
    // cleanup the data
    cleanup_data ();
    
    // cleanup the globals
    cleanup_python (); 
    
    // shutdown input subsystem
    input::shutdown ();

#if defined SDL_MIXER
    // shutdown audio
    audio::cleanup ();
#endif

    // Delete the theme
    delete[] win_theme::theme; 

    // cleanup the saves
    gamedata::cleanup (); 
          
    // shutdown python
    python::cleanup ();     

    // shutdown video and SDL
    SDL_Quit ();

    // save the config file
    configuration->write_adonthellrc ();
    delete configuration;
    configuration = NULL;  
}

void game::init_data ()
{
    data::the_player = NULL;     
    
    data::map_engine = new mapengine;
    PyObject *map_engine = python::pass_instance (data::map_engine, "mapengine"); 
    PyDict_SetItemString (data::globals, "map_engine", map_engine);
    Py_DECREF (map_engine); 

    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (data::globals, "characters", chars);
    Py_DECREF (chars); 

    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (data::globals, "quests", quests);
    Py_DECREF (quests); 
}

void game::cleanup_data () 
{
    delete data::map_engine; 
}

bool game::init_python () 
{
    // Initialise the import path.
    python::insert_path("scripts");
    python::insert_path("scripts/modules");

    /* Initialise SWIG module. This should go if we ever switch to dynamic 
       link */
    initadonthellc();
    
    // Init the global namespace of the python interpreter 
    py_module = python::import_module ("ins_modules");
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
