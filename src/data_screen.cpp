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

#include <iostream.h>

#include "types.h"
#include "pnm.h"
#include "data.h"
#include "image.h"
#include "window.h"
#include "data_screen.h"

data_screen::data_screen (int m)
{
    mode = m;

    // To get a clear view of the map, we have to get the screenshot in advance,
    // but it would be better if we could get it only in case it is needed
    if (mode == SAVE_SCREEN) shot = screen::shot ();
    else shot = NULL;
    entry = NULL;

    // Create GUI
    font = new win_font (win_theme::theme);
    theme = new win_theme (win_theme::theme);

    image_list = new win_select (10, 5, 80, 210, theme);
    image_list->set_select_mode (WIN_SELECT_MODE_BORDER);
    image_list->set_signal_connect (makeFunctor (*this, &data_screen::on_select),
        WIN_SIG_ACTIVATE_KEY);

    text_list = new win_select (110, 5, 130, 210, theme);
    text_list->set_select_mode (WIN_SELECT_MODE_BRIGHTNESS);

    window = new win_container (30, 15, 260, 210, theme);
    window->add (text_list);
    window->add (image_list);

    // Add all the saved games to the list
    init ();

    // Show everything
    window->set_background_visible (true);
    window->set_border_visible (true);
    window->set_visible_all (true);

    text_list->set_visible_all (true);

    image_list->set_activate_keyboard (true);
    image_list->set_visible_all (true);
    image_list->set_activated (true);
}

data_screen::~data_screen ()
{
    delete window;
    delete theme;
    delete font;
}

void data_screen::init ()
{
    int height = 5;
    image *picture;
    win_image *shot;
    win_label *text;
    win_write *entry;    

    for (int i = 0; i < 2; i++)
    {
        picture = new image;
        picture->load_pnm ("empty_slot.pnm");

        shot = new win_image (5, height, picture, theme);
        image_list->add (shot);

        entry = new win_write (0, height, 130, 60, theme, font);
        entry->set_text ("Empty Slot");
        text_list->add (entry);

        height += 68;
    }

    // Add "Empty Slot"
    if (mode == SAVE_SCREEN)
    {
        picture = new image;
        picture->load_pnm ("empty_slot.pnm");

        shot = new win_image (5, height, picture, theme);
        image_list->add (shot);
        image_list->set_default (shot);

        entry = new win_write (0, height, 130, 60, theme, font);
        entry->set_text ("Empty Slot");
        text_list->add (entry);
        text_list->set_default (entry);
    }
}

void data_screen::update ()
{
    if (!entry)
    {
        u_int16 old_pos = image_list->get_pos ();
        image_list->update ();
        u_int16 new_pos = image_list->get_pos ();

        if (old_pos != new_pos) text_list->set_default (new_pos);
    }
    else
    {
        entry->update ();
    }
}

// Select a game
void data_screen::on_select ()
{
    int pos = image_list->get_pos ();
    
    if (mode == LOAD_SCREEN)
    {
        // data::load (pos);
    }
    else
    {
        entry = (win_write *) text_list->get ();
        char *txt = entry->get_text ();
        if (txt && !strncmp (txt, "Empty Slot", 10))
            entry->set_text ("");
        entry->set_activated (true);
        
        // char *path = data::save (pos);
        save_preview (/*path*/);
        // delete path;
    }
}

// Save the small thumbnail image
void data_screen::save_preview ()
{
    image *preview = new image ();

    preview->bytes_per_pixel = 3;
    preview->length = 72;
    preview->height = 54;
    preview->data = SDL_CreateRGBSurface (SDL_SWSURFACE, preview->get_length (),
        preview->get_height (), 24,	0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    preview->zoom (shot);

    SDL_RWops * file = SDL_RWFromFile ("shot.pnm", "w"); 
    pnmput (file, preview->data->pixels, preview->get_length (), preview->get_height ());
    SDL_RWclose(file);

    delete shot;
    delete preview;
}