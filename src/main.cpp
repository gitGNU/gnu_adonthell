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

#include "input.h"
#include "game.h"
#include "data.h"
#include "landmap.h"
#include "mapview.h"
#include "py_inc.h"

// This is nice but very long! `:/
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_anim.h"
#include "win_container.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_background.h"
#include "data_screen.h"
#include "dialog_engine.h"

int main(int argc, char * argv[])
{
    game the_game (argc, argv);

    // Init the game engine
    if (!the_game.init ()) 
        return 1;

    // All this map-loading should go somewhere else and the mapname should
    // come from the dynamic gamedata instead of the configuration file!
    // - Kai

    // Alright... a little map tour!
    // We need a map, and a map view or we won't see anything!
    landmap mymap;
    mapview mview;
    data::load (0);
    data_screen *ds = NULL;
    dialog_engine *de = NULL;
    
    // Trying to load the map
    if(mymap.load("smallmap.map"))
      {
	printf("Sorry, I can't load the map!\n");
	exit(1);
      }

    mapcharacter * heroe=new mapcharacter;
    heroe->load("player.mchar");
    mymap.add_mapcharacter(heroe);
    heroe->stand_south();
    heroe->set_on_map(&mymap);
    heroe->set_pos(0,3,3);
    
    // The map view should cover the entire screen
    mview.resize(320,240);
    // The map view will display "mymap"
    mview.attach_map(&mymap);

    screen::init_frame_counter();
    // This is quite self-explicit...
    while(1)
      {
  	input::update();
	if(input::has_been_pushed(SDLK_ESCAPE)) break;
	for(int i=0;i<screen::frames_to_do();i++)
	  {
	    if (!ds && !de) 
	      {
		if(input::is_pushed(SDLK_RIGHT)) mview.scroll_right();
		if(input::is_pushed(SDLK_LEFT)) mview.scroll_left();
		if(input::is_pushed(SDLK_UP)) mview.scroll_up();
		if(input::is_pushed(SDLK_DOWN)) mview.scroll_down();
		
		if(input::is_pushed(SDLK_d)) heroe->go_north();
		if(input::is_pushed(SDLK_c)) heroe->go_south();
		if(input::is_pushed(SDLK_x)) heroe->go_west();
		if(input::is_pushed(SDLK_v)) heroe->go_east();
		
		if (input::is_pushed(SDLK_l)) ds = new data_screen (LOAD_SCREEN);
		if (input::is_pushed(SDLK_s)) ds = new data_screen (SAVE_SCREEN);

		if (input::is_pushed(SDLK_t)) 
		{
		  de = new dialog_engine ((npc*)data::characters.next(), new win_theme (win_theme::theme));
            de->run ();
         }
	      }
	    mymap.update();
        if (de && !de->update ())
        {
            delete de;
            de = NULL;
        }
	    if (ds && ds->update ())
	      {
		delete ds;
		ds = NULL;
	      }
	  }
	//      	screen::clear();
	mview.draw(0,0);
	if (ds) ds->draw ();
	if (de) de->draw ();
	screen::show();
      }

    return 0;
}
