/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <iostream.h>
#include <string.h>
#include "character.h"
#include "data.h"
#include "input.h"
#include "image.h"
#include "py_inc.h"
#include "input.h"
#include "dialog_engine.h"

// Init the dialogue engine
dialog_engine::dialog_engine (character_base *mynpc, char * dlg_file, u_int8 size) :
    win_container (20, 20, 280, 100, NULL)
{
  init (mynpc, dlg_file, size);
}

void dialog_engine::init(character_base *mynpc, char * dlg_file, u_int8 size)
{
    is_running = true;
    instance = NULL;

    // Load the different fonts
    fonts[0] = new win_font ("white/");
    fonts[1] = new win_font ("yellow/");
    fonts[2] = new win_font ("red/");
    fonts[3] = new win_font ("violet/");
    fonts[4] = new win_font ("blue/");
    fonts[5] = new win_font ("green/");
    
    theme = new win_theme (win_theme::theme);
    set_theme (theme);

    // Full or half-sized window
    if (size)
    {
        move (20, 20);
        resize (280, 200);
    }

    // Make the npc available to the dialogue engine
    PyDict_SetItemString (data::globals, "the_npc", pass_instance (mynpc, "character_base"));
    
    // Init the low level dialogue stuff
    dlg = new dialog;

    // Create dialogue window
    // The NPC's portrait
    face = new win_image (5, 5, 64, 64, theme);

    // The NPC's name
    name = new win_label (5, 74, 64, 0, theme, fonts[0]);
    name->set_auto_height (true);

    // The list with the dialogue
    sel = new win_select (80, 0, 200, height (), theme);
    sel->set_select_mode (WIN_SELECT_MODE_BRIGHTNESS);
    sel->set_layout (WIN_LAYOUT_LIST);
    sel->set_space_between_border (5);
    sel->set_space_between_object (5);
    sel->set_select_circle (true);
    sel->set_scrollbar_visible (true);
    sel->set_activate_keyboard (true);
    sel->set_activated (true);

    // Notification when a dialogue item get's selected
    sel->set_signal_connect (makeFunctor (*this, 
        &dialog_engine::on_select), WIN_SIG_ACTIVATE_KEY);

    // set the NPC's portrait (later:  get the portrait to use from the npc data)
    set_portrait ("gfx/portraits/lyanna.pnm");
    // ... and name
    set_name ((char *) mynpc->get_name().c_str ());

    // add everything to our container
    add (face);
    add (name);
    add (sel);

    set_visible_all (true);
    set_border_visible (true);
    set_background_visible (true);

    // Load dialogue
	if (!dlg->init (dlg_file, strrchr (dlg_file, '/')+1))
	{
        cout << "\n*** Error loading dialogue script " << strrchr (dlg_file, '/')+1 << "\n";
        show_traceback ();
        cout << flush;
        answer = -1;	
	}
	else
	{
        // Make the set_portrait/name/npc functions available to the dialogue script
        instance = pass_instance (this, "dialog_engine");

        PyObject *setname = PyObject_GetAttrString (instance, "set_name");
        PyObject *setportrait = PyObject_GetAttrString (instance, "set_portrait");
        PyObject *setnpc = PyObject_GetAttrString (instance, "set_npc");

        PyObject *dlg_instance = dlg->get_instance ();

        PyObject_SetAttrString (dlg_instance, "set_name", setname);
        PyObject_SetAttrString (dlg_instance, "set_portrait", setportrait);
        PyObject_SetAttrString (dlg_instance, "set_npc", setnpc);

        answer = 0;
    }
}

dialog_engine::~dialog_engine ()
{
    sel->set_activated (false);

    delete dlg;
    delete theme;
    
    for (int i = 0; i < MAX_COLOR; i++)
        delete fonts[i];

    Py_XDECREF (instance);

    // get rid of any keys that might have been accidently pressed during dialogue
    input::clear_keys_queue ();
}

void dialog_engine::run ()
{
    u_int32 i;
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
    if (!dlg->text ())
    {
        is_running = false;
        return;
    }

    // Add NPC text and all player reactions to container
    for (i = 0; i < dlg->text_size (); i++)
    {
        l = new win_label (0, 0, 180, 0, theme, 
            i == 0 ? fonts[dlg->npc_color()] : fonts[1]);
        l->set_auto_height (true);
        l->set_text (dlg->text()[i]);
        l->set_visible (true);
        
        cur_answers.push_back (l);
        sel->add (l);  
    }

    // Either select the single NPC speech ...
    if (dlg->text_size() == 1) sel->set_default (cur_answers.front ());
    
    // ... or the player's first answer
    else 
    {
        cur_answers[0]->set_can_be_selected (false);
        sel->set_default (cur_answers [1]);
    }
}

bool dialog_engine::update ()
{
    sel->update ();

    // catch other keypresses
    if (input::is_pushed (SDLK_LEFT)) sel->up ();
    if (input::is_pushed (SDLK_RIGHT)) sel->down ();
    
    return is_running;
}

void dialog_engine::on_select ()
{
    vector<win_label*>::iterator i;

    // remember choice
    answer = sel->get_pos () - 1;

    // remove all the text
    for (i = cur_answers.begin (); i != cur_answers.end (); i++)
    {
        sel->remove (*i);
        delete *i;
    }
        
    cur_answers.clear ();
    run ();
}

void dialog_engine::insert_plugin ()
{
}

// Set / change the NPC-portrait
void dialog_engine::set_portrait (char *new_portrait)
{
    image *portrait = new image (64, 64);
    portrait->load_pnm (new_portrait);
    portrait->set_mask (true);

    face->set_image (portrait);
    delete portrait;
}

// Set / change the NPC-name
void dialog_engine::set_name (char *new_name)
{
    name->set_text (new_name);
}

// Set a different NPC
void dialog_engine::set_npc (char* new_npc)
{
    character_base *mynpc = (character_base *) data::characters.get (new_npc);
    
    set_name ((char *) mynpc->get_name().c_str ());
    set_portrait ("gfx/portraits/lyanna.pnm");
}
