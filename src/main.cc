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
#include "py_inc.h"


/**
 * Game's main function.
 * It simply initialise the game and run the "init.py" file of the chosen game's
 * directory. Once the execution is finished, it cleanups everything, and exits.
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
    mtrace ();
#endif
    
    if (!game::init (argc, argv)) return 1; 
    
    exec_file("init.py");
    
    game::cleanup (); 
    
    return 0;
}
