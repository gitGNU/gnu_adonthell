/*
   $Id$
   
   Copyright (C) 1999/2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

class dialog;

extern "C"
{
	void initcharacterc(void);
	void initquestc(void);
}

#include <zlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../python_class.h"
#include "../../prefs.h"
#include "../../quest.h"
#include "../../fileops.h"
#include "main.h"
#include "graph.h"
#include "interface.h"


int main (int argc, char *argv[])
{
    char tmp[256];
    PyObject *pyref;
    
    // The Application Data
    MainFrame *MainWnd = new MainFrame;

    // try to read adonthellrc to get path to script directory
    config myconf;
    if (!myconf.read_adonthellrc ())
        return 1;

    // we need some data files from a game, so make sure we get them
    myconf.parse_arguments (argc, argv);

    // Init GTK+
    gtk_init (&argc, &argv);

    // Init Python interpreter
	Py_Initialize ();

	// Init the python modules
	initcharacterc ();
	initquestc ();

    char *path = g_dirname (argv[0]);
    if (path[0] != '/') 
    {
        sprintf (tmp, "%s/%s", getcwd (NULL, 255-strlen (path)), path);
        python::insert_path (tmp);
    }
    else python::insert_path (path);
    free (path);
    
    // Insert our script directory to python's search path
    sprintf (tmp, "%s/scripts", myconf.gamedir.c_str ());
    python::insert_path (tmp);
    
    // ... and the modules directory as well
    sprintf (tmp, "%s/scripts/modules", myconf.gamedir.c_str ());
    python::insert_path (tmp);

    // Load module
    PyObject *m = python::import_module ("ins_dlgedit_modules");
    if (m == NULL) return 1;
        
    // Create a player
	data::globals = PyModule_GetDict(m);
    MainWnd->myplayer = (character *) new character_base ();

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (data::globals, "characters", chars);

    sprintf (tmp, "%s/character.data", myconf.gamedir.c_str ());

    // load characters from character.data
    igzstream in (tmp);
    if (in.is_open ())
    {
        char ctemp;
        character_base *mynpc = NULL;
    
        if (!fileops::get_version (in, 2, 2, tmp))
            return 1;

        while (ctemp << in)
        {
            mynpc = new character_base;
            mynpc->get_state (in);

            // Pass character over to Python interpreter
            pyref = python::pass_instance (mynpc, "character");
            PyDict_SetItemString (chars, (char *) mynpc->get_name().c_str (), pyref);
            Py_DECREF (pyref);

            // Make this character available to the engine
            data::characters[mynpc->get_name ().c_str ()] = (character*) mynpc;
        }

        if (mynpc == NULL)
        {
            mynpc = new character_base ();
            mynpc->set_name ("Dummy Character");
        }

        MainWnd->set_npc (mynpc);

        in.close ();
    }
    else
    {
        cout << "Failed to read file " << tmp << endl << flush;
        return 1;
    }

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (data::globals, "quests", quests);

    // try to open quest.data
    sprintf (tmp, "%s/quest.data", myconf.gamedir.c_str ());
    in.open (tmp); 

    if (in.is_open ())
    {
        char ctemp;
        quest *myquest;
        
        if (!fileops::get_version (in, 1, 1, tmp))
            return 1;

        // load quests
        while (ctemp << in)
        {
            myquest = new quest;
            myquest->load (in);

            // Pass quest over to Python interpreter
            pyref = python::pass_instance (myquest, "quest");
            PyDict_SetItemString (quests, (char *) myquest->name.c_str (),
                pyref);
            Py_DECREF (pyref);

            // Make this quest available to the engine
            data::quests[myquest->name.c_str ()] = myquest;
        }

        in.close (); 
    }
    else
    {
        cout << "Failed to read file " << tmp << endl << flush;
        return 1;
    }

    // Misc initialization
    init_app (MainWnd);

    // Add the player to the character array
    data::the_player = MainWnd->myplayer;

    // Make "the_player" available to the interpreter
    pyref = python::pass_instance (MainWnd->myplayer, "character");
    PyDict_SetItemString (data::globals, "the_player", pyref);
    Py_DECREF (pyref);

    MainWnd->wnd = NULL;
    MainWnd->text_dlg = NULL;
    MainWnd->pixmap = NULL;
    MainWnd->dbg_dlg = NULL;
    MainWnd->test_dlg = NULL;
    MainWnd->debug = 0;

    // Create Top Level Window and Controls
    create_mainframe (MainWnd);

    // Event - Loop
    gtk_main ();

    // Clean up
    Py_Finalize ();

    delete_dialogue (MainWnd);
    delete MainWnd;

    return 0;
}

void MainFrame::set_changed ()
{
    if (changed) return;

    char *title = GTK_WINDOW (wnd)->title;
    gtk_window_set_title (GTK_WINDOW (wnd), g_strjoin (NULL, title, " (changed)", NULL));
    changed = 1;
}

// set a shortcut to one of the NPC's
void MainFrame::set_npc (character_base *npc)
{
    mynpc = (character *) npc;

    PyObject *charref = python::pass_instance (mynpc, "character");
    PyDict_SetItemString (data::globals, "the_npc", charref);
    Py_DECREF (charref);
}

/* Set variables to safe values */
void 
init_app (MainFrame * MainWnd)
{
    MainWnd->mode = IDLE;
    MainWnd->number = 0;
    MainWnd->below_pointer = NULL;
    MainWnd->selected_node = NULL;
    MainWnd->dragged_node = NULL;
    MainWnd->tooltip = NULL;
    MainWnd->file_name = g_strdup ("./new_dlg");
    MainWnd->x_offset = 0;
    MainWnd->y_offset = 0;
    MainWnd->scroll = 0;
    MainWnd->scroll_x = 0;
    MainWnd->scroll_y = 0;
    MainWnd->pset_vars = "";
    MainWnd->cust_func = "";
    MainWnd->changed = 0;
    
    MainWnd->myplayer->set_name ("Banec");
    MainWnd->myplayer->set_val ("race", 0);     // Dwarf
    MainWnd->myplayer->set_val ("gender", 1);   // Male

    data::characters.erase (MainWnd->myplayer->get_name().c_str ());
    data::characters[MainWnd->myplayer->get_name().c_str ()] = MainWnd->myplayer;
}

/* free allocated memory */
void 
delete_dialogue (MainFrame * wnd)
{
    u_int32 i;

    /* Delete all nodes */
    for (i = 0; i < wnd->number; i++)
        delete wnd->nodes[i];

    wnd->nodes.clear ();
    
    g_free (wnd->file_name);
    if (wnd->tooltip) gtk_widget_destroy (wnd->tooltip);
}

