/*
   $Id$
 
   Copyright (C) 1999/2000/2001/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
#include "event_handler.h"
#include "game.h"
#include "gamedata.h"
#include "gametime.h"
#include "input.h"
#include "nls.h"
#include "python_class.h"
#include "screen.h"
#include "yarg.h"
#include "win_manager.h"

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
    void init_adonthell (void);
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
    init_adonthell ();
    
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
extern "C"
int main(int argc, char * argv[])
{
    config myconfig;

#ifdef MEMORY_LEAKS
    // to debug memory leaks with mtrace. It's better to use
    // a tool like memprof, mpatrol or valgrind though.
    mtrace ();
#endif

    // init game environment
#if !defined (SINGLE_DIR_INST) 
    game::init (DATA_DIR);
#else
    // change working directory to the application's location.
    if (argc && argv[0])
    {
        char *str = argv[0];
        do if (*str == '\\') *str = '/'; 
        while (*(str++));
        
        str = strrchr (argv[0], '/');
        *(str + 1) = 0;
        chdir (argv[0]);

        // FIXME: this should go once we have our 'game launcher' gui 
        myconfig.gamedir = string (argv[0]) + "/games/wastesedge";

        *(str + 1) = '/';
    }
    
    // get absolute path to current working directory
    char buf[500];
    getcwd (buf, sizeof (buf));
    
    char *str = buf;
    do if (*str == '\\') *str = '/'; 
    while (*(str++));
    
    game::init (buf);
#endif

    // read the $HOME/.adonthell/adonthellrc file
    // and check the arguments we recieved.
    myconfig.read_adonthellrc ();
    myconfig.parse_arguments (argc, argv);
    
    // init national language support
    nls::init (myconfig);
    
    game::set_game_data_dir(myconfig.gamedir);
    
    // init game loading/saving system
    gamedata::init (game::user_data_dir(), game::game_data_dir(),
        myconfig.game_name, myconfig.quick_load); 

    // init video subsystem
    screen::set_video_mode (320, 240, 0, myconfig.double_screen, myconfig.screen_mode);

#ifdef DEBUG
    printf("%s\n", screen::info().c_str());
#endif
     
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

    // event system
    event_handler::init ();

    // gametime system
    // (180 realtime minutes make approx. 1 gametime day)
    gametime::init (180);

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

    // cleanup event system
    event_handler::cleanup ();
    
    // shutdown python
    python::cleanup ();     

    // shutdown video and SDL
    SDL_Quit ();

    return 0;
}
