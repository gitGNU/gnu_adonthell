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

#ifndef __DATA_H__
#define __DATA_H__

#include <zlib.h>

#include "storage.h"
#include "gametime.h"
#include "Python.h"

class mapengine;

// all the attributes related to a saved game
class gamedata
{
public:
    gamedata ();
    gamedata (char *desc, char *dir);
    ~gamedata ();
    
    void save (gzFile);                     // save a record to disk
    void load (gzFile);                     // load a record from disk

    // a bunch of methods to access the private attributes
    char* get_directory () { return directory; }
    char* get_description () { return description; }
    char* get_location () { return location; }
    char* get_time () { return time; }
    void set_description (char*);
        
private:
    char *directory;                        // the game's location on the harddisk
    char *description;                      // user supplied description of the game
    char *location;                         // the map or area the player is on
    char *time;                             // the gametime of the save
};

// that's most of the data needed by the game engine plus methods 
// to load and save that data

class character;
class mapcharacter;

class data
{
public:
    static void init (char*);               // Data initialisation
    static void cleanup ();                 // Delete everything
    static bool load (u_int32);             // Load a game
    static gamedata* save (u_int32, char*); // Save the game
    static gamedata* next_save ();          // Iterate over saved game descriptions
    static char* get_adonthell_dir ();      // Return the user's adonthell directory

    static PyObject *globals;               // Global namespace to use in scripts
    static gametime *time;                  // The gametime object
    static objects characters;              // All the characters 
    static objects quests;                  // All the quests
    static character *the_player;           // The main character
    static mapcharacter *the_map_player;
#ifndef _EDIT_
    static mapengine * map_engine;          // The map engine
#endif

private:
    static void unload ();                  // Unload the game

    static vector<gamedata*> saves;         // Keeps track of available save games 
    static char *adonthell_dir;             // The $HOME/.adonthell/ directory
};

#endif // __DATA_H__
