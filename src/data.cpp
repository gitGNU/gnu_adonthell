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

#include <dirent.h>
#include <sys/stat.h>

#include "types.h"
#include "fileops.h"
#include "py_inc.h"
#include "quest.h"
#include "data.h"
#include "character.h"
#if defined(USE_MAP)
#include "mapengine.h"
#endif

#if defined(USE_PYTHON)
PyObject *data::globals;            // Global namespace for the Python interpreter
#endif
gametime *data::time;               // The gametime
character* data::the_player=NULL;   // The main character
objects data::characters;           // All the NPC data
objects data::quests;               // All the quest data (the state of the gameworld)
#if defined(USE_MAP)
mapengine * data::map_engine;
#endif

char *data::adonthell_dir=NULL;     // The user's private adonthell directory
vector<gamedata*> data::saves;      // The list of available savegames


gamedata::gamedata ()
{
    directory = NULL;
    description = NULL;
    location = NULL;
    time = NULL;
}

gamedata::gamedata (char *dir, char *desc)
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
    description = get_string (file);
    location = get_string (file);
    time = get_string (file);
}

void gamedata::save (gzFile file)
{
    put_string (file, description);
    put_string (file, location);
    put_string (file, time);
}

void gamedata::set_description (char *desc)
{
    if (description != NULL) delete description;
    description = strdup (desc);
}

void gamedata::set_directory (char *dir)
{
    if (directory != NULL) delete directory;
    directory = strdup (dir);
}

// data initialisation
void data::init (char* d)
{
	DIR *dir;
    gzFile in = NULL;
	struct dirent *dirent;
	struct stat statbuf;
    char filepath[256];

    // This is the user's private adonthell directory
    adonthell_dir = d;
    
    // Add the default savegame used to start a new game to the list of saves
    gamedata *gdata = new gamedata ( ".", "Start New Game");
    saves.push_back (gdata);

    // Init the global namespace of the python interpreter
#if defined(USE_PYTHON)
    PyObject *m = import_module ("ins_modules");
 	globals = PyModule_GetDict(m);
#endif    
    // Read the user's saved games (if any) - they'll be located in
    // $HOME/.adonthell/ and called adonthell-save-xxx
    if ((dir = opendir (adonthell_dir)) != NULL)
    {
        while ((dirent = readdir (dir)) != NULL)
        {
            sprintf (filepath, "%s/%s", adonthell_dir, dirent->d_name);
            if (stat (filepath, &statbuf) != -1 && S_ISDIR (statbuf.st_mode) && 
                strncmp ("adonthell-save-", dirent->d_name, 15) == 0)
			{
                // found a (possibly) valid saved game directory
                strcat (filepath, "/save.data");

                // Now try to read the saved game's data record
                in = gzopen (filepath, "r");

                if (in)
                {
                    // restore the pathname
                    sprintf (filepath, "%s/%s", adonthell_dir, dirent->d_name);

                    gdata = new gamedata;
                    gdata->load (in);
                    gdata->set_directory (filepath);
                    
                    saves.push_back (gdata);
                    gzclose (in);
                }
            }
        }

        closedir (dir);
    }
    
    the_player = NULL;
#if defined(USE_MAP)
    map_engine = new mapengine;
#if defined(USE_PYTHON)
    PyDict_SetItemString (globals, "map_engine", pass_instance (map_engine, "mapengine"));
#endif
#endif
}

// Cleanup everything
void data::cleanup () 
{
    unload ();
    delete adonthell_dir;
    delete time;
    
    for (vector<gamedata*>::iterator i = saves.begin (); i != saves.end (); i++)
      delete *i;

#if defined(USE_PYTHON)
    Py_XDECREF (globals);
#endif
}

// Load a game from the gamedir directory
bool data::load (u_int32 pos)
{
    gzFile in = NULL;
    char filepath[256];
    character *mynpc;
    quest *myquest;

    // First, unload the current game
    unload ();
    
    // Create a player (later: load from file or whatever)
    the_player = new character;
    the_player->set_name("Player");

    // Add the player to the game objects
    characters.set (the_player->get_name(), the_player);

#if defined(USE_PYTHON)
    // Make "myplayer" available to the interpreter 
    PyDict_SetItemString (globals, "the_player", pass_instance (the_player, "character"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (globals, "characters", chars);
    PyDict_SetItemString (chars, the_player->get_name(), pass_instance (the_player, "character"));
#endif
    // try to open character.data
    sprintf (filepath, "%s/character.data", saves[pos]->get_directory ());
    in = gzopen (filepath, "r");

    if (!in)
    {
        fprintf (stderr, "Couldn't open \"%s\" - stopping\n", filepath);
        return false;
    }

    // load characters     
    while (gzgetc (in))
    {
        mynpc = new character;
        mynpc->character_base::load (in);
#if defined(USE_PYTHON)
        // Pass character over to Python interpreter
        PyDict_SetItemString (chars, mynpc->get_name(), pass_instance (mynpc, "character"));
#endif
    }
    
    gzclose (in);

#if defined(USE_PYTHON)
    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (globals, "quests", quests);
#endif

    // try to open quest.data
    sprintf (filepath, "%s/quest.data", saves[pos]->get_directory ());
    in = gzopen (filepath, "r");

    if (!in)
    {
        fprintf (stderr, "Couldn't open \"%s\" - stopping\n", filepath);
        return false;
    }
    
    // load quests
    while (gzgetc (in))
    {
        myquest = new quest;
        myquest->load (in);
        
#if defined(USE_PYTHON)
        // Pass quest over to Python interpreter
        PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));
#endif
        // Make this quest available to the engine
        data::quests.set (myquest->name, myquest);
    }

    gzclose (in);
    return true;
}

// Unload a game
void data::unload ()
{
    character *mychar;
    quest *myquest;

    // delete all characters
    while ((mychar = (character *) characters.next ()) != NULL)
    {
        characters.erase (mychar->get_name());
        delete mychar;
    }

    // delete all quests
    while ((myquest = (quest *) data::quests.next ()) != NULL)
    {
        data::quests.erase (myquest->name);
        delete myquest;
    }    

    // the main character was deleted with the other characters already
    the_player = NULL;
}

// Save all dynamic gamedata to the gamedir
gamedata* data::save (u_int32 pos, char *desc)
{
    gamedata *gdata;
    char filepath[256];
    character *mychar;
    quest *myquest;

    // make sure we don't overwrite the default game
    if (pos == 0) return NULL;

    // see whether we're going to save to a new slot
    if (pos >= saves.size ())
    {
        int success = 1;
        
        // make sure we save to an unused directory
        while (success)
        {
            // that's the directory we're going to save to
            sprintf (filepath, "%s/adonthell-save-%03i", adonthell_dir, pos++);
            success = mkdir (filepath, 0700);
        }
        
        // we'll need a new gamedata record
        gdata = new gamedata (filepath, desc);
    }
    else
    {
        gdata = saves[pos];
        gdata->set_description (desc);
    }

    // save characters 
    sprintf (filepath, "%s/character.data", gdata->get_directory ());
    gzFile file = gzopen (filepath, "w6"); 

    if (!file)
    {
        fprintf (stderr, "Couldn't create \"%s\" - save failed\n", filepath);
        return NULL;
    }

    while ((mychar = (character *) characters.next ()) != NULL)
    {
        // don't save the player
        if ((character*) mychar == (character*) the_player) continue;
    
        // tell the character.data loader that another entry follows
        gzputc (file, 1);

        // append the character data
        mychar->character_base::save (file);
    }

    // write EOF
    gzputc (file, 0);
    gzclose (file);

    // save quests
    sprintf (filepath, "%s/quest.data", gdata->get_directory ());
    file = gzopen (filepath, "w6"); 

    if (!file)
    {
        fprintf (stderr, "Couldn't create \"%s\" - save failed\n", filepath);
        return NULL;
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

    // save gamedata
    sprintf (filepath, "%s/save.data", gdata->get_directory ());
    file = gzopen (filepath, "w6"); 

    if (!file)
    {
        fprintf (stderr, "Couldn't create \"%s\" - save failed\n", filepath);
        return NULL;
    }
    
    gdata->save (file);
    gzclose (file);

    // only now it is safe to add the new record to the array
    if (pos >= saves.size ()) saves.push_back (gdata);
    
    return gdata;
}

// Iterate over saved game descriptions
gamedata* data::next_save ()
{
    static vector<gamedata*>::iterator i = saves.begin ();
    static u_int32 size = saves.size ();

    if (size != saves.size ())
    {
        size = saves.size ();
        i = saves.begin ();
    }

    if (++i == saves.end ())
    {
        i = saves.begin ();
        return NULL;
    }
    
    return *i;
}

// Return the user's adonthell directory
char* data::get_adonthell_dir ()
{
    return adonthell_dir;
}
