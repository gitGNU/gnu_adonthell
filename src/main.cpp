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

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "types.h"
#include "input.h"
#include "image.h"
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "map.h"
#include "mapengine.h"
#include "SDL.h"
#include "SDL_thread.h"
#include "game.h"
#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#include "audio.h"
#endif
#include "animation.h"
#include "cutscene.h"
#include "prefs.h"
#include "event.h"
#include "py_inc.h"

int main(int argc, char * argv[])
{
    config myconfig (argc > 1 ? argv[1] : "");

    // try to read adonthellrc
    if (!myconfig.read_adonthellrc ())
        return 1;
    
    // try to change into data directory
    if (chdir (myconfig.datadir.c_str ()))
    {
        printf ("\nSeems like %s is no valid data directory.", myconfig.datadir.c_str ());
        printf ("\nIf you have installed the Adonthell data files into a different location,");
        printf ("\nplease make sure to update the $HOME/.adonthell/adonthellrc file\n");
        return 1;
    }

    game::init (myconfig);

    landmap * map1 = new landmap;
    if (map1->load (myconfig.mapname.data ())) 
    {
        printf("Error loading map %s!\n", myconfig.mapname.data ());
        return 1;
    }

    map1->load_map_data();
    //    map1->save("okidoki.map");

    // Set focus to the map
    game::engine = map1;

    /* test:
    enter_event *e = new enter_event;
    event_handler::register_event (e, "scripts/events/event_test.py");

    time_event *t = new time_event;
    event_handler::register_event (t, "scripts/events/timer_test.py");
    */
    
    mapengine::map_engine(map1);
    
    // save the config file
    myconfig.write_adonthellrc ();

    game::cleanup();
  
    return 0;
}
