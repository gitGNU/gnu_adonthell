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
     
     config myconfig;
     myconfig.read_adonthellrc ();
     myconfig.parse_arguments (argc, argv); 
    
     if (!game::init (myconfig)) return 1;

//      landmap lmap;
//      lmap.load ("test.map");

//      path mypath;
//      mypath.refmap = &lmap;
//      mypath.submap = 0;
//      mypath.start.x = 27; 
//      mypath.start.y = 21; 
//      mypath.goal.x = 27; 
//      mypath.goal.y = 15;

//      mypath.calculate (); 

//      u_int16 i;
//      for (i = 0; i < mypath.nbr_moves (); i++) 
//      {
//          switch (mypath.get_move (i)) 
//          {
//              case WALK_NORTH:
//                  cout << "WALK_NORTH" << endl;
//                  break; 
//              case WALK_SOUTH:
//                  cout << "WALK_SOUTH" << endl;
//                  break; 
//              case WALK_WEST:
//                  cout << "WALK_WEST" << endl;
//                  break; 
//              case WALK_EAST:
//                  cout << "WALK_EAST" << endl;
//                  break; 
//          }
//      }
     
     python::exec_file("init.py");
    
     game::cleanup (); 
    
     return 0;
}
