/*
   $Id$

   Copyright (C) 1999 The Adonthell Team
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <stdio.h>
#include <unistd.h>
#include "win_manager.h"
#include "input.h"
#include "screen.h"
#include "game.h"

#if defined(USE_PYTHON)
#include "Python.h"
#include "py_modules.h"
#include "py_inc.h"
#endif
#include "data.h"

#ifdef SDL_MIXER
#include "audio.h"
#endif

// The game engine
game::game (int argc, char **argv)
{
    configuration = new config (argc > 1 ? argv[1] : "");
}

// Initialize all parts of the game engine
bool game::init ()
{
    // try to read adonthellrc
    if (!configuration->read_adonthellrc ())
        return false;
    
    // try to change into data directory
    if (chdir (configuration->datadir.c_str ()))
    {
        printf ("\nSeems like %s is no valid data directory.", configuration->datadir.c_str ());
        printf ("\nIf you have installed the Adonthell data files into a different location,");
        printf ("\nplease make sure to update the $HOME/.adonthell/adonthellrc file\n");
        return false;
    }

    // init video subsystem
    screen::set_video_mode (320, 240);

    // init audio subsystem
#if defined SDL_MIXER && !defined _EDIT_
    //    audio::init (configuration);
#endif 

    // init input subsystem
    input::init ();

    // init python interpreter
#if defined(USE_PYTHON)
    init_python ();
#endif
#if !defined(_EDIT_)
    // init the data subsystem
    data::init (configuration->get_adonthellrc ());
#endif
    // voila :)
    return true;
}

// Cleanup everything
game::~game ()
{
    // close all windows
    win_manager::destroy();

    // save the config file
    configuration->write_adonthellrc ();
    delete configuration;
    
#if !defined(_EDIT_)
    // delete all data
    data::cleanup ();
#endif
    // shutdown audio
#if defined SDL_MIXER && !defined _EDIT_
    //    audio::cleanup ();
#endif
    // shutdown Python
#if defined(USE_PYTHON)
    Py_Finalize ();
#endif
    // shutdown video
    SDL_Quit ();
}
