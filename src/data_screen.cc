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
#include "gamedate.h"
#include "image.h"
#include "input.h"
#include "window.h"
#include "data_screen.h"
#include "gametime.h"
#include "win_manager.h"

data_screen::data_screen (int m)
{
    mode = m;
    aborted = false;

    entry = NULL;

    //Init Position and Size
    win_container::move (30, 15);
    win_container::resize (260, 210);
    
    // Create GUI
    font = win_manager::get_font ("original");
    theme = win_manager::get_theme ("original"); 
    
    //Set features used
    set_border(*theme);
    set_background(*theme);

    //Set special Features
    set_trans_background(true);
     
    //Create a win_select
    image_list = new win_select();
    image_list->move (10, 0);
    image_list->resize (250, 210);
    image_list->set_mode (win_select::MODE_BRIGHTNESS);
    image_list->set_layout (win_container::LIST_LAYOUT);
    image_list->set_circle (true);
    image_list->set_space_with_border (9);
    image_list->set_space_with_object (9);
    
    image_list->set_scrollbar (*theme);
    image_list->set_visible_scrollbar (true);
    
    // activate the list
    image_list->set_activate (true);
    
    // give focus to the list
    set_focus_object (image_list);
    
    image_list->set_signal_connect (
        makeFunctor (*this, &data_screen::on_select), 
        win_event::ACTIVATE_KEY);
    
    // add the win_select to *this
    add (image_list);
    
    // add all the saved games to the list
    init ();
    
    // show everything
    set_visible_background (true);
    set_visible_border (true);
    set_visible_all (true);
}

data_screen::~data_screen ()
{
    // fade in from black after loading a game
    if (mode == LOAD_SCREEN && !aborted) data::engine->fade_in ();
}

void data_screen::init ()
{
    string filepath;
    u_int16 num = 0; 
    win_image *shot;
    win_write *entry;
    win_label *date;
    win_container *box = NULL;
    win_font *yellow = win_manager::get_font ("yellow");
    gamedata *gdata;
    
    // display all the available saved games
    while ((gdata = gamedata::next_save ()) != NULL)
    {
        filepath = gdata->directory ();
        filepath += "/preview.pnm";
    
        shot = new win_image ();
        shot->image::load_pnm (filepath); 
        shot->move (5, 2);
        shot->set_border (*theme, win_border::MINI);
        shot->set_visible_border (true);
        shot->pack();
	    
        date = new win_label ();
        date->move (100, 2);
        ((label*)date)->resize (130, 14);
        date->set_font (*yellow);
        date->set_text (gdata->gametime ());
        date->set_cursor_visible (false);
        date->pack();
        
        entry = new win_write ();
        entry->move (100, 18);
        ((label_input*)entry)->resize (130, 40);
        entry->set_font (*font);
        entry->set_text (gdata->description ());
        entry->set_cursor_visible (false);
        entry->pack();
	
        entry_list.push_back (entry);
	
        box = new win_container ();
        box->move (0, 0);
        box->resize (230, 58);
        box->add (shot);
        box->add (date);
        box->add (entry);
        box->set_visible_all (true);
	
        // when the box is activated, we set the entry as 
        // focus object of the box
        box->set_focus_object (entry);
        
        image_list->add (box);
        
        num++;
    }
    
    // If we're saving the game, add "Empty Slot"
    if (mode == SAVE_SCREEN)
    {
        sprintf (gametime, "Day %i - %02i:%02i", gamedate::day (), 
            gamedate::hour (), gamedate::minute ());
        
        shot = new win_image ();
        shot->move (5, 2);
        shot->load_pnm ("gfx/empty_slot.pnm");
        shot->set_border (*theme, win_border::MINI);
        shot->set_visible_border (true);
        shot->pack (); 
        
        date = new win_label ();
        date->move (100, 2);
        ((label*)date)->resize (130, 14);
        date->set_font (*yellow);
        date->set_text (gametime);
        date->set_cursor_visible (false);
        date->pack();

        entry = new win_write ();
        entry->set_font (*font);
        entry->move (100, 18);
        ((label_input*) entry)->resize (130, 40);
        entry->set_text ("Empty Slot");
        entry->set_cursor_visible (false);
        entry->pack ();
	
        entry_list.push_back (entry);
        
        box = new win_container ();
        box->move (0, 0);
        box->resize (230, 58);
        box->add (shot);
        box->add (date);
        box->add (entry);
        box->set_visible_all (true);
	
        // when the box is activated, we set the entry as
        // focus object of the box
        box->set_focus_object(entry);
	
        image_list->add (box);
        image_list->set_default_object (box);
	
        num++;
    }
    else image_list->set_default_position (0);

    // If there are no saved games, display a message.
    if (!num) 
    {
        box = new win_container ();
        box->move(0, 0);
        box->resize(230, 150);
        
        win_label * mess = new win_label ();
        mess->set_font (*font); 
        mess->move (0, 65);
        mess->set_form (label::AUTO_SIZE); 
        mess->set_text ("You have no saved games yet!");
        mess->set_align (win_base::ALIGN_CENTER);  
        mess->pack ();
        box->add (mess); 
        
        mess = new win_label ();
        mess->set_font (*font); 
        mess->move (0, 115);
        mess->set_form (label::AUTO_SIZE); 
        mess->set_text ("Press ESC.");
        mess->set_align (win_base::ALIGN_CENTER);  
        mess->pack ();
        box->add (mess);
        
        box->set_visible_all (true);
        box->set_can_be_selected (false); 
 
        image_list->add (box); 
    }
}

bool data_screen::update ()
{
    if (!win_container::update() || input::has_been_pushed (SDLK_ESCAPE))
    {
        aborted = true;
        data::engine->main_quit ();
    }
    
    return true;
}

// Select a game
void data_screen::on_select ()
{
    int pos = image_list->get_selected_position ();
    
    // loading
    if (mode == LOAD_SCREEN)
    {
        // fade to black before doing the actual work
        data::engine->fade_out ();
        set_visible (false);
        if (!gamedata::load (pos)) aborted = true;
        data::engine->main_quit ();
    }
    // saving
    else
    {
	    win_container * tmp = (win_container*)image_list->get_selected_object();
	    image_list->set_focus_object(tmp);
	
	    entry = (win_write*) tmp->focus_object();
        entry->set_cursor_visible (true);
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
        gdata->set_gametime (gametime); 
        string filepath = gdata->directory ();
        filepath += "/preview.pnm";
        save_preview (filepath);
    }
    
    data::engine->main_quit ();
}

// Save the small thumbnail image
void data_screen::save_preview (string path)
{
    drawing_area da (0, 0, screen::length () >> 1, screen::height () >> 1);
    image temp (da.length (), da.height());
    image preview (72, 54);

    mapview *view = data::engine->get_mapview ();
    mapsquare_area *area = data::engine->get_landmap ()->get_submap
        (data::the_player->submap ());

    u_int16 offx = 0;
    u_int16 offy = 0;

    // In those cases where the mapview is smaller than the physical screen,
    // it will be centered on the screen -> get the offset
    if (area->area_length () * MAPSQUARE_SIZE < view->length ())
        offx = (view->length () - area->area_length () * MAPSQUARE_SIZE) >> 1;
    if (area->area_height () * MAPSQUARE_SIZE < view->height ())
        offy = (view->height () - area->area_height () * MAPSQUARE_SIZE) >> 1;

    // Calculate the player's absolute position on the screen
    s_int16 x = (data::the_player->posx () - view->posx ()) * MAPSQUARE_SIZE + offx;
    s_int16 y = (data::the_player->posy () - view->posy ()) * MAPSQUARE_SIZE + offy;

    // this is a quarter of the screen's size.
    u_int16 length = da.length() >> 1;
    u_int16 height = da.height() >> 1;

    // If the player is too close to the border, make sure we still stay
    // within the screen
    if (x + length > screen::length ()) x = -da.length ();
    else if (x - length < 0) x = 0;
    else x = length - x;

    if (y + height > screen::height ()) y = -da.height ();
    else if (y - height < 0) y = 0;
    else y = height - y;

    data::engine->draw (x, y, &da, &temp);
    preview.zoom (temp); 
    preview.save_pnm (path);     
}
