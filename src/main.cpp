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
#include "landmap.h"
#include "mapview.h"

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
    
    // Trying to load the map
    if(mymap.load("smallmap.map"))
      {
	printf("Sorry, I can't load the map!\n");
	exit(1);
      }
    
    // The map view will display 15 squares horizontally and 11 vertically
    // This is perfect for a 320x240 display, as 15*20=300 and 11*20=220
    // The number of squares to display must be even, or the character won't
    // be centered on the map (may change soon)
    mview.resize(15,11);
    // The map view will display "mymap"
    mview.attach_map(&mymap);

    // Now a little window tour!
    win_font * font=new win_font(WIN_THEME_ELFE);
    win_theme * theme=new win_theme(WIN_THEME_ELFE);
    win_container * container=new win_container(50,24,150,80,theme);
    win_label * label=new win_label(0,7,0,0,theme,font);
    image im;
    if(im.load_pnm("yeti.pnm"))
      {
	printf("Sorry, I can't load the cute Yeti image!\n");
	exit(1);
      }
    im.set_mask(true);
    win_image * img=new win_image(50,20,&im,theme);
    label->set_text("Adonthell Powered By ");
    label->set_auto_size(true);
    container->add(label);
    container->add(img);
    container->set_justify(WIN_JUSTIFY_CENTER);
    container->set_space_between_border(5);
    container->set_border_visible(true);
    container->set_background_visible(true);
    container->set_visible_all(true);

    // This is quite self-explicit...
    while(1)
      {
  	input::update();
	if(input::has_been_pushed(SDLK_ESCAPE)) break;
	for(int i=0;i<screen::frames_to_do();i++)
	  {
	    if(input::is_pushed(SDLK_RIGHT)) mview.scroll_right();
	    if(input::is_pushed(SDLK_LEFT)) mview.scroll_left();
	    if(input::is_pushed(SDLK_UP)) mview.scroll_up();
	    if(input::is_pushed(SDLK_DOWN)) mview.scroll_down();
	    mymap.update();
	    container->update();
	    // Updating the label position
	    {
	      static s_int8 xfac=1, yfac=1;
	      if((container->x()+container->length()+23>=screen::length()) ||
		 (container->x()<=23))
		xfac=-xfac;
	      
     	      if((container->y()+container->height()+23>=screen::height()) ||
		 (container->y()<=23))
		yfac=-yfac;

	      container->move(container->x()+xfac,container->y()+yfac);
	    }
	  }
	//      	screen::clear();
	mview.draw(10,10);
	// FIXME: there should be a draw(u_int16, u_int16) method to comply
	// to the programming style. It would move the container then draw
	// it.
	container->draw();
	screen::show();
      }
    // Cleaning up things...
    delete container;
    delete font;
    delete theme;
    return 0;
}
