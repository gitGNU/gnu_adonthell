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

#include <stdio.h>

#include "types.h"
#include "character.h"
#include "py_inc.h"
#include "quest.h"
#include "data.h"

PyObject *data::globals;        // Global namespace for the Python interpreter
gametime *data::time;           // The gametime
objects data::characters;       // All the character data
objects data::quests;           // All the quest data (the state of the gameworld)


// Load a game. First try to load all dynamic data from gamedir, then load 
// everything else from the static data directory.
bool data::load (const char *gamedir, const char *staticdir)
{
    FILE *in = NULL;
    char filepath[256];
    npc *mynpc;
    quest *myquest;
    
    // Load some modules
    PyObject *m = import_module ("ins_modules");
    Py_INCREF(m);
    
    // Create a player (later: load from file or whatever)
    player *myplayer = new player;
    myplayer->name = "Player";

    // Add the player to the game objects
    characters.set ("the_player", myplayer);

    // Make "myplayer" available to the interpreter 
	globals = PyModule_GetDict(m);
    PyDict_SetItemString (globals, "the_player", pass_instance (myplayer, "player"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (globals, "characters", chars);
    PyDict_SetItemString (chars, "the_player", pass_instance (myplayer, "player"));

    // try to open character.data
    sprintf (filepath, "%s/character.data", gamedir);
    in = fopen (filepath, "r");

    if (!in)
    {
        // try loading from static data-dir then
        sprintf (filepath, "%s/character.data", staticdir);
        in = fopen (filepath, "r");
        
        if (!in)
        {
            fprintf (stderr, "Couldn't open \"character.data\" - stopping\n");
            return false;
        }
    }

    // load characters     
    while (fgetc (in))
    {
        mynpc = new npc;
        mynpc->load (in);

        // Pass character over to Python interpreter
        PyDict_SetItemString (chars, mynpc->name, pass_instance (mynpc, "npc"));
    }
    
    fclose (in);

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (globals, "quests", quests);

    // try to open quest.data
    sprintf (filepath, "%s/quest.data", gamedir);
    in = fopen (filepath, "r");

    if (!in)
    {
        // try loading from static data-dir then
        sprintf (filepath, "%s/quest.data", staticdir);
        in = fopen (filepath, "r");
        
        if (!in)
        {
            fprintf (stderr, "Couldn't open \"quest.data\" - stopping\n");
            return false;
        }
    }
    
    // load quests
    while (fgetc (in))
    {
        myquest = new quest;
        myquest->load (in);
        
        // Pass quest over to Python interpreter
        PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));

        // Make this quest available to the engine
        data::quests.set (myquest->name, myquest);
    }

    fclose (in);
    return true;
}

// Save all dynamic gamedata to the gamedir
void data::save (const char* gamedir)
{
}
