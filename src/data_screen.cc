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


#include <iostream.h>

#include "types.h"
#include "pnm.h"
#include "gamedata.h"
#include "image.h"
#include "input.h"
#include "window.h"
#include "data_screen.h"
#include "mapengine.h"

#include "game.h"

data_screen::data_screen (int m) : win_container (30, 15, 260, 210, NULL)
{
    mode = m;
    quit = false;

    entry = NULL;

    // Create GUI
    font = new win_font (win_theme::theme);
    theme = new win_theme (win_theme::theme);
    set_theme (theme);

    image_list = new win_select (10, 0, 250, 210, theme);
    image_list->set_select_mode (WIN_SELECT_MODE_BRIGHTNESS);
    image_list->set_layout (WIN_LAYOUT_LIST);
    image_list->set_select_circle (true);
    image_list->set_space_between_border (9);
    image_list->set_space_between_object (9);
    image_list->set_activated (true);
    image_list->set_signal_connect (makeFunctor (*this, &data_screen::on_select),
        WIN_SIG_ACTIVATE_KEY);

    add (image_list);

    // Add all the saved games to the list
    init ();

    // Show everything
    set_background_visible (true);
    set_border_visible (true);
    set_visible_all (true);

    image_list->set_activate_keyboard (true);
    image_list->set_visible_all (true);
}

data_screen::~data_screen ()
{
    delete theme;
    delete font;
}

void data_screen::init ()
{
    int num = 0;
    char filepath[256];
    image *picture;
    win_image *shot;
    win_write *entry;
    win_container *box = NULL;
    gamedata *gdata;

    // display all the available saved games
    while ((gdata = gamedata::next_save ()) != NULL)
    {
        sprintf (filepath, "%s/preview.pnm", gdata->directory ());
    
        picture = new image;
        picture->load_pnm (filepath);

        shot = new win_image (5, 2, picture, theme);
        shot->set_border_size (WIN_SIZE_MINI);
        shot->set_border_visible (true);

        entry = new win_write (100, 2, 130, 54, theme, font);
        entry->set_text (gdata->description ());
        entry_list.push_back (entry);

        box = new win_container (0, 0, 230, 58, theme);
        box->add (shot);
        box->add (entry);
        box->set_visible_all (true);

        image_list->add (box);
        
        delete picture;
        num++;
    }

    // If we're saving the game, add "Empty Slot"
    if (mode == SAVE_SCREEN)
    {
        picture = new image;
        picture->load_pnm ("gfx/empty_slot.pnm");

        shot = new win_image (5, 2, picture, theme);
        shot->set_border_size (WIN_SIZE_MINI);
        shot->set_border_visible (true);

        entry = new win_write (100, 2, 130, 54, theme, font);
        entry->set_text ("Empty Slot");
        entry_list.push_back (entry);
        
        box = new win_container (0, 0, 230, 58, theme);
        box->add (shot);
        box->add (entry);
        box->set_visible_all (true);

        image_list->add (box);
        image_list->set_default (box);

        delete picture;
        num++;
    }
    else image_list->set_default (1);

    // display scrollbar if necessary
    if (num > 3)
    {
        image_list->set_scrollbar_visible (true);

        if (mode == SAVE_SCREEN) 
            while (num-- >= 0) image_list->down ();
    }
}

bool data_screen::update ()
{
    // cancel saving/loading
    if (input::has_been_pushed (SDLK_ESCAPE)) return false;

    // nothing selected --> browse through available games
    if (!entry)
    {
        image_list->update ();
    }
    // a slot for saving selected --> enter description of the game
    else
    {
        entry->update ();
    }

    return !quit;
}

// Select a game
void data_screen::on_select ()
{
    int pos = image_list->get_pos ();

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
        // allow to enter a description
        entry = entry_list[image_list->get_pos ()-1];
        char *txt = entry->get_text ();
        if (txt && !strncmp (txt, "Empty Slot", 10))
            entry->set_text ("");
        entry->set_signal_connect (makeFunctor (*this, &data_screen::on_save),
            WIN_SIG_ACTIVATE_KEY);
        entry->set_activated (true);
        input::clear_keys_queue ();
    }
}

void data_screen::on_save ()
{
    char* description = entry->get_text ();
    int pos = image_list->get_pos ();

    gamedata::save (pos, description);
    gamedata *gdata = gamedata::get_saved_game (pos);

    // save sucessful --> save preview
    if (gdata != NULL)
    {
        char filepath[256];
        sprintf (filepath, "%s/preview.pnm", gdata->directory ());
        save_preview (filepath);
    }

    set_return_code (1);
    quit = true;
}

// Save the small thumbnail image
void data_screen::save_preview (char *path)
{
    image temp (screen::length (), screen::height ()); 
    image preview (72, 54);

    data::map_engine->draw (0, 0, NULL, &temp); 
    preview.zoom (temp); 
    preview.save_pnm (path);     
}
