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



#ifdef MEMORY_LEAKS
#include <mcheck.h>
#endif

#include "game.h"

#include "path.h"

using namespace std; 

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

    // initialise the different parts of the engine
    // (video, audio, python ...)
    if (!game::init (myconfig)) return 1;

    // It's up to the game what happens here
    python::exec_file ("init.py");

    // shut down the different parts of the engine
    game::cleanup ();
    
    return 0;
}
