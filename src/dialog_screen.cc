/*
   $Id$

   (C) Copyright 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   dialog_screen.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief  Defines the dialog_screen class.
 *
 *
 */


#include <iostream>
#include <string>
#include "gamedata.h"
#include "input.h"
#include "image.h"
#include "python_class.h"
#include "input.h"
#include "dialog_screen.h"
#include "win_manager.h"

#include "audio.h"

// Init the dialogue engine
dialog_screen::dialog_screen (character_base *mynpc, char * dlg_file, u_int8 size)
{
    init (mynpc, dlg_file, size);
}

void dialog_screen::init(character_base *mynpc, char * dlg_file, u_int8 size)
{
    string path = dlg_file;
    string file = strrchr (dlg_file, '.') ? strrchr (dlg_file, '.') + 1 : dlg_file;

    audio::play_wave (-1, 0);
    is_running = true;
    portrait = "None";
    
    // Init position & size
    win_container::move (20, 20);
    win_container::resize (280, 105);

    // Load the different fonts
    fonts[0] = win_manager::get_font ("white");
    fonts[1] = win_manager::get_font ("yellow");
    fonts[2] = win_manager::get_font ("red");
    fonts[3] = win_manager::get_font ("violet");
    fonts[4] = win_manager::get_font ("blue");
    fonts[5] = win_manager::get_font ("green");

    theme = win_manager::get_theme ("original");

    set_border (*theme);
    set_background (*theme);
    set_trans_background (true);

    // Full or half-sized window
    if (size)
    {
        move (20, 15);
        resize (280, 210);
    }

    // Init the low level dialogue stuff
    dlg = new dialog (mynpc);

    // Create dialogue window
    // The NPC's portrait
    face = new win_image();
    face->move (5, 5);
    ((image*)face)->resize (64, 64);
    face->pack();
    face->set_visible (true);

    // The NPC's name
    name = new win_label ();
    name->set_font (*(fonts[0]));
    name->move (5, 74);
    ((label*)name)->resize (64, 0);
    name->set_form (label::AUTO_HEIGHT);
    name->set_visible (true);

    // The list with the dialogue
    sel = new win_select ();
    sel->set_scrollbar (*theme);
    sel->move (80, 0);
    sel->resize (200, height ());
    sel->set_mode (win_select::MODE_BRIGHTNESS);
    sel->set_layout (win_container::LIST_LAYOUT);
    sel->set_space_with_border (5);
    sel->set_space_with_object (5);
    sel->set_circle (true);
    sel->set_visible_scrollbar (true);
    // Commented 'cause it makes troubles during dialogues right now :) 
    //     sel->set_auto_scrollbar (true);
    sel->set_activate (true);

    sel->set_visible (true);
    sel->set_focus (true); //due an error from window system

    // Notification when a dialogue item get's selected
    sel->set_signal_connect (makeFunctor (*this,  &dialog_screen::on_select),
                             win_event::ACTIVATE_KEY);

    // add everything to our container
    add (face);
    add (name);
    add (sel);
    set_focus_object (sel);

    set_visible_border (true);
    set_visible_background (true);

    set_visible (true);
    set_activate (true);

    // Make the npc and player available to the dialogue engine
    PyObject *args = PyTuple_New (2);
    PyTuple_SetItem (args, 0, python::pass_instance (data::the_player, "character"));
    PyTuple_SetItem (args, 1, python::pass_instance (mynpc, "character_base"));

    // Load dialogue
    if (!dlg->init (path, file, args))
    {
        cout << "\n*** Error loading dialogue script " << file << "\n";
#ifdef PY_DEBUG
        python::show_traceback ();
#endif
        cout << flush;
        answer = -1;	
    }
    else answer = 0;

    // Clean up
    Py_DECREF (args);
}

dialog_screen::~dialog_screen ()
{
    sel->set_activate (false);

    delete dlg;

    // get rid of any keys that might have been accidently pressed during dialogue
    input::clear_keys_queue ();
}

void dialog_screen::run ()
{
    u_int32 i = 0;
    win_label *l;

    // Possibly error
    if (answer < 0)
    {
        is_running = false;
        return;
    }

    // Continue dialogue with selected answer
    dlg->run (answer);

    // End of dialogue
    if (dlg->text_size () == 0)
    {
        is_running = false;
        return;
    }

    // update the NPC's portrait and name
    set_portrait (dlg->npc_portrait ());
    set_name (dlg->npc_name ());

    // Add NPC text and all player reactions to container
    for (string txt = dlg->text (); txt != ""; txt = dlg->text (), i++)
    {
        l = new win_label();
        l->set_font (i == 0 ? *fonts[dlg->npc_color ()] : *fonts[1]);
        l->move(0,0);
        ((label*)l)->resize(180,0);
        l->set_form(label::AUTO_HEIGHT);
        l->set_text (txt);
        l->set_visible (true);
        l->pack();
        cur_answers.push_back (l);
        sel->add (l);
    }

    // Either select the single NPC speech ...
    if (dlg->text_size () == 1)
        sel->set_default_object (cur_answers.front ());

    // ... or the player's first answer
    else
    {
        cur_answers[0]->set_can_be_selected (false);
        sel->set_default_object (cur_answers[1]);
    }

    // Center on the NPC answer
    sel->set_pos (0);
}

bool dialog_screen::update ()
{
    return (win_container::update() && is_running);
}

void dialog_screen::on_select ()
{
    vector<win_label*>::iterator i;

    // remember choice
    answer = sel->get_selected_position () - 1;

    // remove all the text
    for (i = cur_answers.begin (); i != cur_answers.end (); i++)
    {
        sel->remove (*i);
        delete *i;
    }
    sel->destroy ();

    cur_answers.clear ();
    run ();
}

void dialog_screen::insert_plugin ()
{
}

// Set / change the NPC-portrait
void dialog_screen::set_portrait (const string & new_portrait)
{
    if (new_portrait == portrait) return;
    else portrait = new_portrait;

    if (new_portrait == "")
    {
        face->image::resize (64, 64);
        face->fillrect (0, 0, 64, 64, 0x00ff00ff);
        return;
    }
    face->load_pnm(string ("gfx/portraits/") + new_portrait);
    face->set_mask(true);
    face->pack();
}

// Set / change the NPC-name
void dialog_screen::set_name (const string & new_name)
{
    name->set_text (new_name);
    name->pack ();
}

// Set a different NPC
void dialog_screen::set_npc (const string & new_npc)
{
    character_base *mynpc = (character_base *) data::characters[new_npc.c_str ()];
    
    set_name (mynpc->get_name());
    set_portrait (mynpc->get_portrait ());
}
