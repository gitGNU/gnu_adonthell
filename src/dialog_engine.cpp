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

#include "character.h"
#include "data.h"
#include "input.h"
#include "image.h"
#include "screen.h"
#include "py_inc.h"
#include "dialog_engine.h"

// Init the dialogue engine
dialog_engine::dialog_engine (npc *mynpc)
{
/*
    // save active engine and ...
    engine = game::engine;
    // ... set focus to the dialogue engine
    game::engine = this;
*/
    portrait = NULL;
    name = NULL;
    face = NULL;
    
    // npc *mynpc = (npc *) game::characters.get (npc_name);
    char *dlg_file = mynpc->get_dialogue ();

    // Make the npc available to the dialogue engine
    PyDict_SetItemString (data::globals, "the_npc", pass_instance (mynpc, "npc"));
    
    // Init the low level dialogue stuff
    dlg = new dialog;

    // Create window
    font = new win_font (win_theme::theme);
    th= new win_theme (win_theme::theme);
    wnd = new win_container (40, 20, 240, 160, th);

    // set the NPC's portrait (later:  get the portrait to use from the npc data)
    set_portrait ("gfxtree/portraits/lyanna.pnm");
    // ... and name
    set_name (mynpc->name);

    win_select::next_key=SDLK_DOWN;
    win_select::previous_key=SDLK_UP;

    txt = new win_container (80, 5, 155, 150,th);
    wnd->add(txt);
    question=new win_label(10,10,100,30,th,font);
    txt->add(question);
    question->set_auto_height(true);
    sel = new win_select (0,0,150,145,th);
    txt->add(sel);
    sel->set_activated(true);
    sel->set_activate_keyboard(true);

    wnd->set_visible_all (true);
    txt->set_visible_all (true);
    wnd->set_border_visible(true);
    wnd->set_background_visible(true);
    wnd->draw ();

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
        // Make the set_portrait/name/color functions available to the dialogue script
        instance = pass_instance (this, "dialog_engine");

        PyObject *setname = PyObject_GetAttrString (instance, "set_name");
        PyObject *setportrait = PyObject_GetAttrString (instance, "set_portrait");
        PyObject *setcolor = PyObject_GetAttrString (instance, "set_color");
        PyObject *setnpc = PyObject_GetAttrString (instance, "set_npc");

        PyObject *dlg_instance = dlg->get_instance ();

        PyObject_SetAttrString (dlg_instance, "set_name", setname);
        PyObject_SetAttrString (dlg_instance, "set_portrait", setportrait);
        PyObject_SetAttrString (dlg_instance, "set_color", setcolor);
        PyObject_SetAttrString (dlg_instance, "set_npc", setnpc);
    
	   answer = 0;
    }
    delete dlg_file;
}

dialog_engine::~dialog_engine ()
{
    // refresh screen
    screen::drawbox (0, 0, 320, 200, 0);

    // restore engine
    // game::engine = engine;

    sel->set_activated (false);
    delete wnd;
    delete th;
    delete portrait;
    delete font;

    Py_XDECREF (instance);
}

void dialog_engine::run ()
{
    u_int32 i, h = 10;
    win_label *l;
    
    // Possibly error
    if (answer < 0)
    {
        delete this;
        return;
    }

    // Continue dialogue with selected answer
    dlg->run (answer);

    // End of dialogue
    if (!dlg->text)
    {
        delete this;
        return;
    }

    // Add NPC text and all player reactions to container

    question->set_text(dlg->text[0]);

    for (i = 1; i < dlg->text_size; i++)
    {
        l = new win_label (10, h, 100, h+font->height(), th, font);
        l->set_auto_height (true);
        l->set_text (dlg->text[i]);
        txt->add(l);
        sel->add (l);  
        cur_answers.push_back (l);

        h += l->height()+10;
    }

    // Either select the single NPC speech ...
    if (dlg->text_size == 1) 
    {
        l = new win_label (10, h, 120, h+font->height(), th, font);
        l->set_auto_height (true);
        l->set_text ("Continue...");
	    txt->add(l);
        sel->add (l);  
        cur_answers.push_back (l);	
        sel->set_default (cur_answers.front ());
    }
    
    // ... or the player's first answer
    else sel->set_default (cur_answers[0]);
    
    sel->move(0,question->y()+question->height()+10);
    sel->resize(sel->length(),txt->height()-question->height()-15);
    txt->set_visible_all (true);
    txt->draw ();
}

void dialog_engine::realtime_tasks ()
{
    update_keyboard ();
}

void dialog_engine::update_keyboard ()
{
    // SPACE activates the current selection
    if (input::has_been_pushed (SDLK_SPACE))
    {
        vector<win_label*>::iterator i;

        // remember choice
        answer = sel->get_pos ();
        if(cur_answers.size()==1) answer--;

        // remove all the displayed text
        for (i = cur_answers.begin (); i != cur_answers.end (); i++)
        {
            sel->remove (*i);
            txt->remove (*i);
            delete *i;
        }
        
        cur_answers.clear ();
        run ();
    }

    // Cursor keys iterate through possible answers
    sel->update();
}

void dialog_engine::gametime_tasks ()
{
//    engine->gametime_tasks ();
    wnd->draw ();
}

void dialog_engine::insert_plugin ()
{
}

// Set / change the NPC-portrait
void dialog_engine::set_portrait (char *new_portrait)
{
    if (face)
    {
        wnd->remove (face);
        delete face;
    }
    if (portrait) delete portrait;
    
    portrait = new image (64, 64);
    portrait->load_pnm (new_portrait);
    portrait->set_mask (true);

    face = new win_image (10, 10, portrait, th);
    wnd->add(face);
    face->set_visible (true);
}

// Set / change the NPC-name
void dialog_engine::set_name (char *new_name)
{
    if (name)
    {
        wnd->remove (name);
        delete name;
    }
    
    name = new win_label (10, 75, 64, 10, th, font);
    wnd->add(name);
    name->set_auto_height (true);
    name->set_text (new_name);
    name->set_visible (true);
}

// Set / change the NPC-text color 
void dialog_engine::set_color (u_int32 new_color)
{
}

// Set a different NPC
void dialog_engine::set_npc (char* new_npc)
{
    npc *mynpc = (npc *) data::characters.get (new_npc);
    
    set_name (mynpc->name);
    set_portrait ("gfxtree/portraits/lyanna.pnm");
    set_color (0);
}
