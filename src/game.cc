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



initflags game::initiated = INIT_NONE;
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
bool game::init (int argc, char **argv, initflags to_init = INIT_ALL)
{
    if (configuration) delete configuration;
    
    configuration = new config (argc > 1 ? argv[1] : "");
    
    // try to read adonthellrc
    if (!configuration->read_adonthellrc ())
        return false;

    // init game loading/saving system
    if (to_init & INIT_SAVES) 
    { 
        gamedata::init (configuration->get_adonthellrc (), configuration->datadir); 
    }
    
    // init video subsystem
    if (to_init & INIT_VIDEO) 
    { 
        screen::set_video_mode (320, 240);
        screen::set_fullscreen (configuration->screen_mode);

        // set the theme
        win_theme::theme = new char[strlen(configuration->window_theme.c_str ())];
        strcpy (win_theme::theme, configuration->window_theme.c_str ());
        strcat (win_theme::theme,"/");
    }
    

#if defined SDL_MIXER
    // init audio subsystem
    if (to_init & INIT_AUDIO) 
    { 
        audio::init (configuration);
    }
#endif 
    
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
    
    initiated = to_init; 
    
    // voila :)
    return true;
}

// Cleanup everything
void game::cleanup () 
{
    // close all windows
    win_manager::destroy(); 
    
    // shutdown input subsystem
    if (initiated & INIT_INPUT) 
    { 
        input::shutdown ();
    }
#if defined SDL_MIXER
    // shutdown audio
    if (initiated & INIT_AUDIO) 
    {
        audio::cleanup ();
    }
#endif


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
    if (initiated & INIT_PYTHON) 
    {
        SDL_Quit ();
        // Delete the theme
        delete[] win_theme::theme; 
    }
    
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

    data::the_player = new character;
    data::the_player->set_name ("Player");
    
    // Add the player to the game objects
    data::characters[data::the_player->get_name().c_str ()] = data::the_player; 
    
    // retrieve character array
    chars = PyDict_GetItemString (data::globals, "characters");

    // Make the player available to Python
    PyObject *the_player = python::pass_instance (data::the_player, "character"); 
    PyDict_SetItemString (data::globals, "the_player", the_player);
    
    PyDict_SetItemString (chars, (char *) data::the_player->get_name().c_str (),
                          the_player);
    Py_DECREF (the_player); 
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
