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

#include "image.h"
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "map.h"
#include "mapengine.h"
#include "game.h"

int main(int argc, char * argv[])
{
    game the_game (argc, argv);

    // Init the game engine
    if (!the_game.init ()) 
        return 1;

    // All this map-loading should go somewhere else and the mapname should
    // come from the dynamic gamedata instead of the configuration file!
    // - Kai
    /*
    landmap * map1 = new landmap;
    if (map1->load ("maptest.map"))
    {
        printf("Error loading map %s!\n", "maptest.map");
        return 1;
    }

    map1->load_map_data();

    // Set focus to the map
    // game::engine = map1;
    
    mapengine::map_engine(map1);
    */
    return 0;
}
