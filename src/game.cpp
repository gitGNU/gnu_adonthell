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
#include "input.h"
#include "screen.h"
#include "game.h"

#ifndef _EDIT_
#include "Python.h"
#include "py_modules.h"
#include "py_inc.h"
#include "data.h"
#endif

#ifdef SDL_MIXER
#include "audio.h"
#endif

// The game engine
game::game (int argc, char **argv)
{
    configuration = new config (argc > 1 ? argv[1] : "");
#ifdef SDL_MIXER
    audio_thread = NULL; 
#endif
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
    screen::set_video_mode (320, 240, configuration);

    // init audio subsystem
#ifdef SDL_MIXER
    audio::init (configuration);

    /*    if (audio::is_initialized())
        audio_thread = SDL_CreateThread (audio::update, NULL);

    if (audio_thread == NULL) 
    {
        printf ("Couldn't create audio thread: %s\n", SDL_GetError ());
        printf ("Audio will not be used\n");
	}*/
#endif  

    // init input subsystem
    input::init();

    // init python interpreter
#ifndef _EDIT_
	init_python ();

    // init the data subsystem
    data::init (configuration->get_adonthellrc ());

#ifdef SDL_MIXER
    // Make audio_in available to python
    //  pass_instance(&audio_in,"audio");
#endif
#endif

    // voila :)
    return true;
}

// Cleanup everything
game::~game ()
{
    // save the config file
    configuration->write_adonthellrc ();
    delete configuration;
    
    // shutdown audio
#ifdef SDL_MIXER
    /*    if (audio_thread != NULL) 
    {
    SDL_KillThread (audio_thread);*/
        audio::cleanup ();
	//    }
#endif

    // shutdown Python
#ifndef _EDIT_
	Py_Finalize ();

    // delete all data
	data::cleanup ();
#endif

    // shutdown video
    SDL_Quit ();
}
