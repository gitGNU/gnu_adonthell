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
#include <dirent.h>
#include <sys/stat.h>
#include <fstream.h>

#include "types.h"
#include "fileops.h"
#include "py_inc.h"
#include "quest.h"
#include "data.h"
#include "character.h"
#if defined (USE_MAP)
#include "mapengine.h"
#endif

#if defined (USE_PYTHON)
PyObject *data::globals = NULL;     // Global namespace for the Python interpreter
PyObject *data::py_module = NULL;
#endif

#if defined (USE_MAP)
mapengine * data::map_engine;
#endif

gametime *data::time;               // The gametime
character* data::the_player = NULL; // The main character
objects data::characters;           // All the NPC data
objects data::quests;               // All the quest data (the state of the gameworld)
string data::adonthell_dir;         // The user's private adonthell directory
vector<gamedata*> data::saves;      // The list of available savegames


gamedata::gamedata ()
{
}

gamedata::gamedata (string dir, string desc)
{
    directory = dir;
    description = desc; 
}

gamedata::~gamedata ()
{
}

bool gamedata::load (igzstream& file)
{
    if (!fileops::get_version (file, 2, 2, "save.data")) 
        return false;
    directory << file; 
    description << file;
    location << file;
    time << file; 
    return true;
}

void gamedata::save (ogzstream& file)
{
    fileops::put_version (file, 2);
    directory >> file;
    description >> file;
    location >> file;
    time >> file; 
}

void gamedata::set_description (string desc)
{
    description = desc; 
}

void gamedata::set_directory (string dir)
{
    directory = dir;
}

// data initialisation
bool data::init (string d)
{
    DIR *dir;
    igzstream in;
    struct dirent *dirent;
    struct stat statbuf;
    string filepath;
    
    // This is the user's private adonthell directory
    adonthell_dir = d;
    
    // Add the default savegame used to start a new game to the list of saves
    gamedata *gdata = new gamedata ( ".", "Start New Game");
    saves.push_back (gdata);

    // Init the global namespace of the python interpreter
#if defined (USE_PYTHON)
    py_module = import_module ("ins_modules");
    if (!py_module)
        return false;
 	globals = PyModule_GetDict (py_module);
#endif

    // Read the user's saved games (if any) - they'll be located in
    // $HOME/.adonthell/ and called adonthell-save-xxx
    if ((dir = opendir (adonthell_dir.c_str ())) != NULL)
    {
        while ((dirent = readdir (dir)) != NULL)
        {
            filepath = adonthell_dir;
            filepath += "/";
            filepath += dirent->d_name; 
            //             sprintf (filepath, "%s/%s", adonthell_dir.c_str (), dirent->d_name);
            if (stat (filepath.c_str (), &statbuf) != -1 && S_ISDIR (statbuf.st_mode) && 
                strncmp ("adonthell-save-", dirent->d_name, 15) == 0)
            {
                // found a (possibly) valid saved game directory
                filepath += "/save.data"; 
                // Now try to read the saved game's data record
                in.open (filepath); 
                
                if (in.is_open ())
                {
                    // restore the pathname
                    filepath = adonthell_dir;
                    filepath += "/";
                    filepath += dirent->d_name;
                    
                    gdata = new gamedata;
                    if (gdata->load (in))
                    {
                        gdata->set_directory (filepath);
                        saves.push_back (gdata);
                    }
                    else delete gdata;
                    
                    in.close (); 
                }
            }
        }

        closedir (dir);
    }
    
    the_player = NULL;
#if defined (USE_MAP)
    map_engine = new mapengine;
#if defined (USE_PYTHON)
    PyDict_SetItemString (globals, "map_engine", pass_instance (map_engine, "mapengine"));
#endif
#endif

    return true;
}

// Cleanup everything
void data::cleanup () 
{
    unload ();
    delete time;
    
    for (vector<gamedata*>::iterator i = saves.begin (); i != saves.end (); i++)
      delete *i;

#if defined (USE_PYTHON)
    // Note that we don't have to DECREF globals, because they're a borrowed
    // reference of py_module
    Py_XDECREF (py_module);
#endif
}

// Load a game from the gamedir directory
bool data::load (u_int32 pos)
{
    igzstream in;
    
    string filepath;
    character *mynpc;
    quest *myquest;

    // First, unload the current game
    unload ();
    
    // Create a player (later: load from file or whatever)
    the_player = new character;
    the_player->set_name ("Player");

    // Add the player to the game objects
    characters.set (the_player->get_name(), the_player);

#if defined (USE_PYTHON)
    // Make "myplayer" available to the interpreter 
    PyDict_SetItemString (globals, "the_player", pass_instance (the_player, "character"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (globals, "characters", chars);
    PyDict_SetItemString (chars, the_player->get_name(), pass_instance (the_player, "character"));
#endif
    // try to open character.data
    filepath = saves[pos]->get_directory ();
    filepath += "/character.data";     
    in.open (filepath);
    
    if (!in.is_open ())
    {
        cerr << "Couldn't open \"" << filepath << "\" - stopping\n" <<  endl; 
        return false;
    }
    
    if (!fileops::get_version (in, 2, 2, filepath)) 
        return false;
    
    // load characters     

    char ctemp;
    while (ctemp << in) 
    {
        mynpc = new character;
        mynpc->character_base::load (in.file);
#if defined (USE_PYTHON)
        // Pass character over to Python interpreter
        PyDict_SetItemString (chars, mynpc->get_name(), pass_instance (mynpc, "character"));
#endif
    }
    
    in.close (); 
    
#if defined (USE_PYTHON)
    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (globals, "quests", quests);
#endif

    // try to open quest.data
    filepath = saves[pos]->get_directory ();
    filepath += "/quest.data"; 
    in.open (filepath); 

    if (!in.is_open ())
    {
        cerr <<  "Couldn't open \"" << filepath << " - stopping\n" << endl; 
        return false;
    }
    
    if (!fileops::get_version (in, 1, 1, filepath))
        return false;
    
    // load quests
    while (ctemp << in) 
    {
        myquest = new quest;
        myquest->load (in.file);
        
#if defined (USE_PYTHON)
        // Pass quest over to Python interpreter
        PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));
#endif
        // Make this quest available to the engine
        data::quests.set (myquest->name, myquest);
    }

    in.close (); 
    
    
#ifdef USE_MAP
    // Load mapengine state
    filepath = saves[pos]->get_directory(); 
    filepath += "/mapengine.data";
    in.open (filepath); 

    if (!in.is_open ())
    {
        cerr <<  "Couldn't open \"" << filepath << " - stopping\n" << endl; 
        return false;
    }
    //  FIXME : put a version info in mapengine state file
    map_engine->get_state(in); 
    in.close (); 
#endif
    return true;
}

// Unload a game
void data::unload ()
{
#if defined (USE_MAP)
    landmap *map = map_engine ? map_engine->get_landmap () : NULL;
#endif
    character *mychar;
    quest *myquest;

    // delete all characters
    while ((mychar = (character *) characters.next ()) != NULL)
    {
        characters.erase (mychar->get_name());

#if defined (USE_MAP)
        if (map)
        {
            map->remove_mapchar (mychar, mychar->get_submap (),
                mychar->get_posx (), mychar->get_posy ());

            // This might be needed to catch moving characters (???)
            map->remove_mapchar (mychar, mychar->get_submap (), 
                mychar->get_posx ()-1, mychar->get_posy ());
            map->remove_mapchar (mychar, mychar->get_submap (), 
                mychar->get_posx (), mychar->get_posy ()-1);
        }
#endif
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

    // clean the map
#if defined (USE_MAP)
    //     if (map) map->mapchar.clear ();
    if (map) map->clear ();
#endif
}

// Save all dynamic gamedata to the gamedir
gamedata* data::save (u_int32 pos, string desc)
{
    gamedata *gdata;
    string filepath;
    char t[10]; 
    character *mychar;
    quest *myquest;
    ogzstream file; 
    char vnbr; 
    
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
            sprintf(t,"%03i",pos++);
            filepath = get_adonthell_dir();
	    filepath += "/adonthell-save-";
	    filepath += t;
            success = mkdir (filepath.c_str(), 0700);

            // prevent infinite loop if we can't write to the directory
            if (pos >= 1000) 
            {
                cerr << "Save failed - seems like you have no write permission in\n" << adonthell_dir << endl; 
                return NULL;
            }
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
    filepath = gdata->get_directory ();
    filepath += "/character.data"; 
    file.open (filepath); 

    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n"; 
        return NULL;
    }
    
    fileops::put_version (file, 2);
    while ((mychar = (character *) characters.next ()) != NULL)
    {
        // don't save the player
        if ((character*) mychar == (character*) the_player) continue;
    
        // tell the character.data loader that another entry follows
        vnbr = 1; 
        vnbr >> file; 

        // append the character data
        mychar->character_base::save (file.file);
    }

    // write EOF
    vnbr = 0; 
    vnbr >> file;
    file.close (); 

    // save quests
    filepath = gdata->get_directory ();
    filepath += "/quest.data"; 
    file.open (filepath); 
    
    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n";
        return NULL;
    }

    fileops::put_version (file, 1);
    while ((myquest = (quest *) data::quests.next ()) != NULL)
    {
        // tell the quest.data loader that another entry follows
        vnbr = 1;
        vnbr >> file; 
        //         gzputc (file, 1);

        // append the character data
        myquest->save (file.file);
    }

    // write EOF
    vnbr = 0; 
    vnbr >> file; 
    file.close ();
    
    // save gamedata
    filepath = gdata->get_directory ();
    filepath += "/save.data"; 

    file.open (filepath); 
    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n"; 
        return NULL;
    }
    
    gdata->save (file);
    file.close ();
    
#ifdef USE_MAP
    // Save mapengine state
    filepath = gdata->get_directory(); 
    filepath += "/mapengine.data";
    file.open (filepath); 
    map_engine->put_state(file);
    file.close (); 
#endif

    // only now it is safe to add the new record to the array
    if (pos >= saves.size ()) saves.push_back (gdata);
    
    return gdata;
}

// Iterate over saved game descriptions
gamedata* data::next_save ()
{
    static vector<gamedata*>::iterator i = saves.begin ();
    static u_int32 size = saves.size ();

    // check whether a new save has been added
    if (size != saves.size ())
    {
        size = saves.size ();
        i = saves.begin ();
    }

    // check whether we reached the end of the list
    if (++i == saves.end ())
    {
        i = saves.begin ();
        return NULL;
    }
    
    return *i;
}
