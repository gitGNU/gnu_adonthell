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

extern "C"
{
	void initcharacterc(void);
	void initquestc(void);
}

#include <zlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../py_inc.h"
#include "../../prefs.h"
#include "../../data.h"
#include "../../quest.h"
#include "main.h"
#include "graph.h"
#include "interface.h"
#include "../../character.h"

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
	Py_Initialize ();

	initcharacterc ();
	initquestc ();

    char *path = g_dirname (argv[0]);
    if (path[0] != '/') 
    {
        sprintf (tmp, "%s/%s", getcwd (NULL, 255-strlen (path)), path);
        insert_path (tmp);
    }
    else insert_path (path);
    free (path);
    
    // Insert our script directory to python's search path
    sprintf (tmp, "%s/scripts", myconf.datadir.c_str ());
    insert_path (tmp);
    
    // ... and the modules directory as well
    sprintf (tmp, "%s/scripts/modules", myconf.datadir.c_str ());
    insert_path (tmp);

    // Load module
    PyObject *m = import_module ("ins_dlgedit_modules");
    if (m == NULL) return 1;
        
    // Create a player
    MainWnd->myplayer = new character;

    // Add the player to the character array
    data::the_player = MainWnd->myplayer;

    // Make "the_player" available to the interpreter 
	data::globals = PyModule_GetDict(m);
    PyDict_SetItemString (data::globals, "the_player", pass_instance (MainWnd->myplayer, "character"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (data::globals, "characters", chars);

    sprintf (tmp, "%s/character.data", myconf.datadir.c_str ());

    // load characters from character.data
    gzFile in = gzopen (tmp, "r");
    if (in)
    {
        character *mynpc = NULL;
    
        while (gzgetc (in))
        {
            mynpc = new character;
            mynpc->load (in);
            PyDict_SetItemString (chars, mynpc->get_name(), pass_instance (mynpc, "character"));
        }

        if (mynpc == NULL)
        {
            mynpc = new character ();
            mynpc->set_name("Dummy Character");
        }
        
        // set a shortcut to one of the NPC's
        PyDict_SetItemString (data::globals, "the_npc", pass_instance (mynpc, "character"));
        MainWnd->mynpc = mynpc;

        gzclose (in);
    }

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (data::globals, "quests", quests);

    // try to open quest.data
    sprintf (tmp, "%s/quest.data", myconf.datadir.c_str ());
    in = gzopen (tmp, "r");

    if (in)
    {
        quest *myquest;
        
        // load quests
        while (gzgetc (in))
        {
            myquest = new quest;
            myquest->load (in);
        
            // Pass quest over to Python interpreter
            PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));

            // Make this quest available to the engine
            data::quests.set (myquest->name, myquest);
        }
        
        gzclose (in);    
    }
    
    // Misc initialization
    init_app (MainWnd);

    data::characters.set (MainWnd->myplayer->get_name(), MainWnd->myplayer);

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
    delete MainWnd->myplayer;
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

