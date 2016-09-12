/*
   Copyright (C) 1999/2000/2001/2002/2003/2004 Kai Sterker
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   main.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Contains the main() function.
 * 
 * 
 */
 
#include "achievements.h"
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
#if PY_VERSION_HEX >= 0x03000000
	PyObject* PyInit__adonthell (void);
#else
	void init_adonthell (void);
#endif
}

static bool init_python(const char *application)
{
#if PY_VERSION_HEX >= 0x03000000
	// one would assume that this is properly handled by SWIG,
	// but it seems some extra effort is required on our side
	// to use a static module
	if (PyImport_AppendInittab ("_adonthell", PyInit__adonthell) < 0)
	{
		return false;
	}
#if PY_VERSION_HEX >= 0x03050000
	wchar_t *program = Py_DecodeLocale(application, NULL);
	if (program != NULL)
	{
		Py_SetProgramName(program);
	}
#endif
#endif

    python::init ();

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
#if PY_VERSION_HEX < 0x03000000
    init_adonthell();
#endif
    
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

        *(str + 1) = '/';
    }
    
    // get absolute path to current working directory
    char buf[500];
    getcwd (buf, sizeof (buf));
    
    char *str = buf;
    do if (*str == '\\') *str = '/'; 
    while (*(str++));
    
    // get absolute path to data directory
    string data_dir(DATA_DIR);
    while (data_dir.size() >= 3 && data_dir.compare(0, 3, "../") == 0)
    {
        str = strrchr (buf, '/');
        if (str != NULL)
        {
            *(str+1) = 0;
        }
        data_dir = data_dir.substr(3);
    }
    
    game::init (string(buf) + data_dir);
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

    // init achievements
    achievements::init();

    // init video subsystem
    if (!screen::init (320, 240, 0, myconfig))
    {
        printf("%s\n", screen::info().c_str());
        SDL_Quit();
        return 1;
    }

#ifdef DEBUG
    printf("%s\n", screen::info().c_str());
#endif
     
    // init audio subsystem
    if (myconfig.audio_volume > 0)
        audio::init (&myconfig);
    
    // init input subsystem
    input::init ();
    
    // init python interpreter
    if (!init_python(argv[0]))
    {
    	std::cout << "Initializing Python module failed" << std::endl;
        SDL_Quit ();
    	return false;
    }

    // init the game data
    data::engine = new adonthell;
    data::the_player = NULL;

    // init window manager
    win_manager::init (myconfig.font); 

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

    // shutdown screen
    screen::cleanup ();

    // shutdown video and SDL
    SDL_Quit ();

    return 0;
}
