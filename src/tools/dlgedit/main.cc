/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

class dialog;

#include <stdio.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../py_inc.h"
#include "../../prefs.h"
#include "../../game.h"
#include "../../quest.h"
#include "main.h"
#include "graph.h"
#include "interface.h"

int 
main (int argc, char *argv[])
{
    char tmp[256];
    
    // The Application Data
    MainFrame *MainWnd = new MainFrame;

    // try to read adonthellrc to get path to script directory
    config myconf (argc > 1 ? argv[1] : "");
    if (!myconf.read_adonthellrc ())
        return 1;

    gtk_init (&argc, &argv);

    // Init Python interpreter
    if (!init_python ())
    {
        // This is unlikely to happen
        fprintf(stderr, "Couldn't initialise Python - stopping\n");
        return 1;
    }

    // Insert our script directory to python's search path
    sprintf (tmp, "%s/scripts", myconf.datadir.c_str ());
    insert_path (tmp);
    
    // ... and the modules directory as well
    sprintf (tmp, "%s/scripts/modules", myconf.datadir.c_str ());
    insert_path (tmp);

    // Load module
    PyObject *m = import_module ("ins_modules");
        
    // Create a player
    MainWnd->myplayer = new player;

    // Add the player to the game objects
    game::characters.set ("the_player", MainWnd->myplayer);

    // Make "myplayer" available to the interpreter 
	game::globals = PyModule_GetDict(m);
    PyDict_SetItemString (game::globals, "the_player", pass_instance (MainWnd->myplayer, "player"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (game::globals, "characters", chars);

    sprintf (tmp, "%s/character.data", myconf.datadir.c_str ());

    // load characters from character.data
    FILE *in = fopen (tmp, "r");
    if (in)
    {
        npc *mynpc = NULL;
    
        while (fgetc (in))
        {
            mynpc = new npc;
            mynpc->load (in, false);
            PyDict_SetItemString (chars, mynpc->name, pass_instance (mynpc, "npc"));
        }

        // set a shortcut to one of the NPC's
        PyDict_SetItemString (game::globals, "the_npc", pass_instance (mynpc, "npc"));
    
        fclose (in);
    }

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (game::globals, "quests", quests);

    // try to open quest.data
    sprintf (tmp, "%s/quest.data", myconf.datadir.c_str ());
    in = fopen (tmp, "r");

    if (in)
    {
        quest *myquest;
        
        // load quests
        while (fgetc (in))
        {
            myquest = new quest;
            myquest->load (in);
        
            // Pass quest over to Python interpreter
            PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));

            // Make this quest available to the engine
            game::quests.set (myquest->name, myquest);
        }
        
        fclose (in);    
    }
    
    // Misc initialization
    init_app (MainWnd);

    MainWnd->wnd = NULL;
    MainWnd->text_dlg = NULL;
    MainWnd->pixmap = NULL;
    MainWnd->project = NULL;
    MainWnd->err = NULL;

    // Create Top Level Window and Controls
    create_mainframe (MainWnd);

    // Event - Loop
    gtk_main ();

    // Clean up
    kill_python ();

    delete_dialogue (MainWnd);
    delete MainWnd->myplayer;
    delete MainWnd;

    return 0;
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

    MainWnd->myplayer->name = strdup ("Banec");
    MainWnd->myplayer->set ("race", 0);     // Dwarf
    MainWnd->myplayer->set ("gender", 1);   // Male
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

