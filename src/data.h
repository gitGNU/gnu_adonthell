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
#include <string> 
#include "fileops.h" 
#include "storage.h"
#include "gametime.h"

#if defined(USE_PYTHON)
#include "Python.h"
#endif

#if defined(USE_MAP)
class mapengine;
#endif

class character;

#ifndef SWIG
// all the attributes related to a saved game
class gamedata
{
public:
    gamedata ();
    gamedata (string desc, string dir);
    ~gamedata ();
    
    void save (ogzstream&);                     // save a record to disk
    bool load (igzstream&);                     // load a record from disk

    // a bunch of methods to access the private attributes
    const char* get_directory () { return directory.c_str (); }
    const char* get_description () { return description.c_str (); }
    const char* get_location () { return location.c_str (); }
    const char* get_time () { return time.c_str (); }
    void set_description (string);
    void set_directory (string);
    
private:
    string directory;                        // the game's location on the harddisk
    string description;                      // user supplied description of the game
    string location;                         // the map or area the player is on
    string time;                             // the gametime of the save
};
#endif

// that's most of the data needed by the game engine plus methods 
// to load and save that data
class data
{
public:
    static bool init (string);               // Data initialisation
    static void cleanup ();                 // Delete everything
    static bool load (u_int32);             // Load a game
    static gamedata* save (u_int32, string); // Save the game
    static gamedata* next_save ();          // Iterate over saved game descriptions
    static const char* get_adonthell_dir () // Return the user's adonthell directory
      { return (const char*) adonthell_dir.c_str (); }

#if defined(USE_PYTHON)
    static PyObject *globals;               // Global namespace to use in scripts
    static PyObject *py_module;             // The Adonthell Python wrapper
#endif
    static gametime *time;                  // The gametime object
    static objects characters;              // All the characters 
    static objects quests;                  // All the quests
    static character *the_player;           // The main character
#if defined(USE_MAP)
    static mapengine * map_engine;          // The map engine
#endif

private:
#ifndef SWIG
    static void unload ();                  // Unload the game

    static vector<gamedata*> saves;         // Keeps track of available save games 
    static string adonthell_dir;             // The $HOME/.adonthell/ directory
#endif
};

#endif // __DATA_H__
