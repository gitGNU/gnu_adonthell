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

#include <zlib.h>
#include <sys/stat.h>

#include "types.h"
#include "fileops.h"
#include "character.h"
#include "py_inc.h"
#include "quest.h"
#include "data.h"

PyObject *data::globals;        // Global namespace for the Python interpreter
gametime *data::time;           // The gametime
objects data::characters;       // All the character data
objects data::quests;           // All the quest data (the state of the gameworld)
char *data::adonthell_dir=NULL; // The user's private adonthell directory
vector<gamedata*> data::saves;  // The list of available savegames

gamedata::gamedata ()
{
    directory = NULL;
    description = NULL;
    location = NULL;
    time = NULL;
}

gamedata::gamedata (char *desc, char *dir)
{
    directory = strdup (dir);
    description = strdup (desc);
    location = NULL;    // later landmap::curmap->location ();
    time = NULL;        // later game::time->to_string ();
}

gamedata::~gamedata ()
{
    if (directory != NULL) delete directory;
    if (description != NULL) delete description;
    if (location != NULL) delete location;
    if (time != NULL) delete time;
}

void gamedata::load (gzFile file)
{
    directory = get_string (file);
    description = get_string (file);
    location = get_string (file);
    time = get_string (file);
}

void gamedata::save (gzFile file)
{
    put_string (file, directory);
    put_string (file, description);
    put_string (file, location);
    put_string (file, time);
}

// data initialisation
void data::init (char* dir)
{
    // This is the user's private adonthell directory
    adonthell_dir = dir;
    
    // Add the default savegame used to start a new game to the list of saves
    gamedata *gdata = new gamedata ("Start New Game", "adonthell-start");
    saves.push_back (gdata);
}

// Load a game from the gamedir directory
bool data::load (u_int32 pos)
{
    gzFile in = NULL;
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
    sprintf (filepath, "%s/character.data", saves[pos]->get_directory ());
    in = gzopen (filepath, "r");

    if (!in)
    {
        fprintf (stderr, "Couldn't open \"character.data\" - stopping\n");
        return false;
    }

    // load characters     
    while (gzgetc (in))
    {
        mynpc = new npc;
        mynpc->load (in);

        // Pass character over to Python interpreter
        PyDict_SetItemString (chars, mynpc->name, pass_instance (mynpc, "npc"));
    }
    
    gzclose (in);

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (globals, "quests", quests);

    // try to open quest.data
    sprintf (filepath, "%s/quest.data", saves[pos]->get_directory ());
    in = gzopen (filepath, "r");

    if (!in)
    {
        fprintf (stderr, "Couldn't open \"quest.data\" - stopping\n");
        return false;
    }
    
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
    return true;
}

// Save all dynamic gamedata to the gamedir
bool data::save (u_int32 pos, char *desc)
{
    gamedata *gdata;
    char filepath[256];
    character *mychar;
    quest *myquest;

    // make sure we don't overwrite the default game
    if (pos == 0) return false;

    // see whether we're going to save to a new slot
    if (pos > saves.size ())
    {
        char dir[32];
        
        // that's the directory we're going to save to
        sprintf (dir, "adonthell-save-%i", saves.size ());

        // create that new directory
        sprintf (filepath, "%s/%s", adonthell_dir, dir);
        mkdir (filepath, 0700);

        // we'll need a new gamedata record
        gdata = new gamedata (dir, desc);

        saves.push_back (gdata);
    }
    else gdata = saves[pos];

    // save characters 
    sprintf (filepath, "%s/%s/character.data", adonthell_dir, gdata->get_directory ());
    gzFile file = gzopen (filepath, "w6"); 

    if (!file)
    {
        fprintf (stderr, "Couldn't create \"%s\" - save failed\n", filepath);
        return false;
    }

    while ((mychar = (character *) characters.next ()) != NULL)
    {
        // tell the character.data loader that another entry follows
        gzputc (file, 1);

        // append the character data
        mychar->save (file);
    }

    // write EOF
    gzputc (file, 0);
    gzclose (file);

    // save quests
    sprintf (filepath, "%s/%s/quest.data", adonthell_dir, gdata->get_directory ());
    file = gzopen (filepath, "w6"); 

    if (!file)
    {
        fprintf (stderr, "Couldn't create \"%s\" - save failed\n", filepath);
        return false;
    }

    while ((myquest = (quest *) data::quests.next ()) != NULL)
    {
        // tell the quest.data loader that another entry follows
        gzputc (file, 1);

        // append the character data
        myquest->save (file);
    }

    // write EOF
    gzputc (file, 0);
    gzclose (file);

    return true;
}