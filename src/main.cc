/*
   $Id$
 
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifdef MEMORY_LEAKS
#include <mcheck.h>
#endif

#include "fileops.h"
#include "game.h"
#include "data.h"
#include "py_inc.h"
#include "mapengine.h"
#include "window.h"
#include "win_theme.h"

#ifdef _DEBUG_
#include "landmap.h"
#endif

#ifdef _DEBUG_
    void mem_report(void)
      {
	cout << "******** Dynamically Allocated Objects report: ********\n";
	cout << "landmap: " << landmap::a_d_diff << " still allocated.\n";
	cout << "landsubmap: " << landsubmap::a_d_diff << " still allocated.\n";
	cout << "mapsquare: " << mapsquare::a_d_diff << " still allocated.\n";
	cout << "mapsquare_tile: " << mapsquare_tile::a_d_diff << " still allocated.\n";
	cout << "mapsquare_char: " << mapsquare_char::a_d_diff << " still allocated.\n";
	cout << "animationframe: " << animationframe::a_d_diff << " still allocated.\n";
	cout << "animation: " << animation::a_d_diff << " still allocated.\n";
	cout << "image: " << image::a_d_diff << " still allocated.\n";
	
      }
#endif

int main(int argc, char * argv[])
{
#ifdef MEMORY_LEAKS
    mtrace ();
#endif
#ifdef _DEBUG_
    atexit(&mem_report);
#endif
 
     game the_game (argc, argv);
    
    // Init the game engine
    if (!the_game.init ()) 
        return 1;
    
    exec_file("init.py");
    
//     screen::set_video_mode (320, 240); 
    
//      image im, im2;
//      im.load_pnm ("/home/alex/adonthell-data/adonthell/gfx/empty_slot.pnm"); 
//      im2 = im; 
//      im.draw (0, 0); 

//      im2.draw (0, 100); 
//      screen::show ();
     //      getchar (); 

//     delete test;
//     delete th;
//     //     delete f; 
    return 0;
}
