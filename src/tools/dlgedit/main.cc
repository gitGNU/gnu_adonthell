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
#include "main.h"
#include "graph.h"
#include "interface.h"

int 
main (int argc, char *argv[])
{
    // The Application Data
    MainFrame *MainWnd = new MainFrame;

    // try to read adonthellrc to get path to script directory
    config myconf (argc > 1 ? argv[1] : "");
    if (!myconf.read_adonthellrc ())
        return 1;

    char script_dir[256];
    sprintf (script_dir, "%s/scripts", myconf.datadir.c_str ());

    gtk_init (&argc, &argv);

    // Init Python interpreter
    if (!init_python ())
    {
        // This is unlikely to happen
        fprintf(stderr, "Couldn't initialise Python - stopping\n");
        return 1;
    }

    // Insert our script directory to python's search path
    insert_path (script_dir);

    // Load module
    PyObject *m = import_module ("player");
        
    // Create a player
    MainWnd->myplayer = new player;

    // Add the player to the game objects
    objects::set ("the_player", MainWnd->myplayer);

    // Make "myplayer" available to the interpreter 
	PyObject *d = PyModule_GetDict(m);
    PyDict_SetItemString (d, "the_player", pass_instance (m, MainWnd->myplayer, "player"));

    // this is just a hack, have to do it proper in the future:
    PyDict_SetItemString (d, "FEMALE", PyInt_FromLong (0));
    PyDict_SetItemString (d, "MALE",  PyInt_FromLong (1));
    PyDict_SetItemString (d, "HUMAN", PyInt_FromLong (3));
    PyDict_SetItemString (d, "ELF", PyInt_FromLong (1));
    PyDict_SetItemString (d, "HALFELF", PyInt_FromLong (2));
    PyDict_SetItemString (d, "DWARF", PyInt_FromLong (0));

    // Misc initialization
    init_app (MainWnd);

    MainWnd->dict = d;
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
    MainWnd->file_name = g_strdup ("./new_dialogue");
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

