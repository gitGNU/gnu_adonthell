/*
   $Id$

   Copyright (C) 2001 by Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   data_screen.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the data_screen class.
 * 
 * 
 */


#include <string.h>

#include "types.h"
#include "pnm.h"
#include "gamedata.h"
#include "image.h"
#include "input.h"
#include "window.h"
#include "data_screen.h"
#include "mapengine.h"

data_screen::data_screen (int m)
{
    mode = m;
    quit = false;

    entry = NULL;

    //Init Position and Size
    win_container::move(30,15);
    win_container::resize(260,210);
    
    // Create GUI
    font = new win_font (WIN_THEME_ORIGINAL);
    theme = new win_theme (WIN_THEME_ORIGINAL);
    
    //Set features used
    set_border(*theme);
    set_background(*theme);

    //Set special Features
    set_trans_background(true);
     
    //Create a win_select
    image_list = new win_select();
    image_list->move(10,0);
    image_list->resize(250,210);
    image_list->set_mode (win_select::MODE_BRIGHTNESS);
    image_list->set_layout (win_container::LIST_LAYOUT);
    image_list->set_circle (true);
    image_list->set_space_with_border (9);
    image_list->set_space_with_object (9);
    
    image_list->set_scrollbar(*theme);
    image_list->set_visible_scrollbar(true);
    
    //activate the list
    image_list->set_activate (true);
    
    //when this have focus, give focus to the list
    set_focus_object(image_list);
    
    image_list->set_signal_connect (
        makeFunctor (*this, &data_screen::on_select), 
        win_event::ACTIVATE_KEY);
    
    //add the win_select to *this
    add (image_list);
    
    // Add all the saved games to the list
    init ();
    
    // Show everything
    set_visible_background (true);
    set_visible_border (true);
    
    set_visible_all (true);
}

data_screen::~data_screen ()
{
    delete theme;
    delete font;
}

void data_screen::init ()
{
    string filepath;
    win_image *shot;
    win_write *entry;
    win_container *box = NULL;
    gamedata *gdata;
    
    // display all the available saved games
    while ((gdata = gamedata::next_save ()) != NULL)
    {
        filepath = gdata->directory ();
        filepath += "/preview.pnm";
    
	    shot = new win_image();
        shot->image::load_pnm (filepath); 
	    shot->move(5,2);
	    shot->set_border(*theme,win_border::MINI);
	    shot->set_visible_border (true);
        shot->pack();
	
	    entry = new win_write();
	    entry->move(100,2);
	    ((label_input*)entry)->resize(130,54);
	    entry->set_font(*font);
	    entry->set_text (gdata->description ());
	    entry->pack();
	
	    entry_list.push_back (entry);
	
        box = new win_container ();
	    box->move(0,0);
	    box->resize(230,58);
        box->add (shot);
        box->add (entry);
        box->set_visible_all (true);
	
	    // when the box is activated, we set the entry as 
        // focus object of the box
	    box->set_focus_object(entry);

        image_list->add (box);
    }
    
    // If we're saving the game, add "Empty Slot"
    if (mode == SAVE_SCREEN)
    {
        shot = new win_image ();
        shot->move(5,2);
        shot->load_pnm("gfx/empty_slot.pnm");
	    shot->set_border(*theme, win_border::MINI);
        shot->set_visible_border (true);
	    shot->pack (); 

        entry = new win_write ();
	    entry->set_font(*font);
        entry->move(100,2);
	    ((label_input*) entry)->resize(130,54);
	    entry->set_text ("Empty Slot");
	    entry->pack();
	
	    entry_list.push_back (entry);
        
        box = new win_container ();
        box->move(0,0);
	    box->resize(230,58);
	    box->add (shot);
        box->add (entry);
	    box->set_visible_all (true);
	
	    // when the box is activated, we set the entry as
        // focus object of the box
	    box->set_focus_object(entry);
	
        image_list->add (box);
        image_list->set_default_object (box);
    }
    else image_list->set_default_position (0);     
}

bool data_screen::update ()
{
    if (!win_container::update() || input::has_been_pushed (SDLK_ESCAPE)) 
        return false;
  
    return !quit;
}

// Select a game
void data_screen::on_select ()
{
    int pos = image_list->get_selected_position ();
    
    // loading
    if (mode == LOAD_SCREEN)
    {
        gamedata::load (pos);
        set_return_code (1);
        quit = true;
    }
    // saving
    else
    {
	    win_container * tmp = (win_container*)image_list->get_selected_object();
	    image_list->set_focus_object(tmp);
	
	    entry = (win_write*) tmp->focus_object();
		entry->set_cursor ( true);
		entry->set_cursor_moveable (true);
	
	    const char *txt = entry->text_char ();
	    if (txt && !strncmp (txt, "Empty Slot", 10))
	        entry->set_text ("");
  
	    entry->set_signal_connect (makeFunctor (*this, &data_screen::on_save),
				   win_event::ACTIVATE_KEY);
	    entry->set_activate (true);
	    input::clear_keys_queue ();
    }
}

void data_screen::on_save ()
{
    const char* description = entry->text_char ();
    int pos = image_list->get_selected_position ();

    gamedata::save (pos, description);
    gamedata *gdata = gamedata::get_saved_game (pos);

    // save sucessful --> save preview
    if (gdata != NULL)
    {
        string filepath = gdata->directory ();
        filepath += "/preview.pnm";
        save_preview (filepath);
    }

    set_return_code (1);
    quit = true;
}

// Save the small thumbnail image
void data_screen::save_preview (string path)
{
    image temp (screen::length (), screen::height ()); 
    image preview (72, 54);
    
    data::map_engine->draw (0, 0, NULL, &temp); 
    preview.zoom (temp); 
    preview.save_pnm (path);     
}
