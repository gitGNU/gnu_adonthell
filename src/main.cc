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


/**
 * @file   main.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Contains the main() function.
 * 
 * 
 */
 
#include "audio.h"
#include "character.h"
#include "game.h"
#include "gamedata.h"
#include "input.h"
#include "nls.h"
#include "python_class.h"
#include "screen.h"
#include "yarg.h"
#include "win_manager.h"
#include "win_theme.h"

#ifdef MEMORY_LEAKS
#include <mcheck.h>
#endif

using namespace std; 

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

bool init_python()
{
    // Initialise the import path.
    // Shared modules path 
    string t;
    t = game::global_data_dir() + "/modules";
    python::insert_path ((char *) t.c_str()); 
    
    // Game specific path
    t = game::game_data_dir () + "/scripts/modules"; 
    python::insert_path ((char *) t.c_str ());
    t = game::game_data_dir () + "/scripts"; 
    python::insert_path ((char *) t.c_str ());
    
    // Initialise SWIG module. This should go if we ever switch 
    // to dynamic linking
    initadonthellc ();
    
    python::module = python::import_module ("adonthell"); 
    if (!python::module) return false;     
    
    data::globals = PyModule_GetDict (python::module);
    
    return true; 
}

/**
 * Game's main function.
 * It simply initialises the game and runs the "init.py" file in the game
 * directory given as first argument. Once the execution is finished,
 * it cleans everything up, and exits.
 *
 * @param argc Number of arguments to the program.
 * @param argv Array of strings containing the program's arguments.
 *
 * @return 0 in case of success, error code otherwise.
 * 
 */
int main(int argc, char * argv[])
{
#ifdef MEMORY_LEAKS
    // to debug memory leaks with mtrace. It's better to use
    // a tool like memprof or mpatrol though.
    mtrace ();
#endif

    config myconfig;

    // read the $HOME/.adonthell/adonthellrc file
    // and check the arguments we recieved.
    myconfig.read_adonthellrc ();
    myconfig.parse_arguments (argc, argv);

    // init game environment
    game::init (myconfig.gamedir);
    
    // init national language support
    nls::init (myconfig);
    
    // init game loading/saving system
    gamedata::init (myconfig.get_adonthellrc (), myconfig.gamedir, myconfig.game_name); 
    
    // init video subsystem
    screen::set_video_mode (320, 240);
    screen::set_fullscreen (myconfig.screen_mode); 
     
    // init audio subsystem
    if (myconfig.audio_volume > 0)
        audio::init (&myconfig);
    
    // init input subsystem
    input::init ();
    
    // init python interpreter
    python::init (); 
    init_python();

    // init the game data
    data::engine = new adonthell;
    data::the_player = NULL;

    // init window manager
    win_manager::init (); 

    // init random number generator
    yarg::init (myconfig.game_name);
    yarg::randomize ();
    
    // It's up to the game what happens here
    python::exec_file ("init");

    // close all windows
    // delete all themes and fonts
    win_manager::cleanup (); 
    
    // shutdown input subsystem
    input::shutdown ();

    // shutdown audio
    audio::cleanup ();

    // cleanup the saves
    gamedata::cleanup (); 
    
    // cleanup data
    delete data::engine;
    if (data::the_player)
        delete data::the_player;

    // shutdown python
    // Cleanup the global namespace of python interpreter
    // Note that we don't have to DECREF data::globals, because they're a
    // borrowed reference of py_module.
    Py_DECREF (python::module); 
    python::cleanup ();     

    // shutdown video and SDL
    SDL_Quit ();

    return 0;
}
