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


/**
 * @file   gamedata.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the gamedata and data classes.
 * 
 * 
 */ 


#ifndef GAMEDATA_H__
#define GAMEDATA_H__

#include <zlib.h>
#include <string> 
#include <vector>
#include "fileops.h"

#include "quest.h"
#include "character.h"
#include "mapengine.h"

/**
 * Contains all the attributes related to a saved %game.
 * 
 */ 
class gamedata
{
public:
    /**
     * Default constructor.
     * 
     */ 
    gamedata ();

#ifndef SWIG
    /** 
     * Alternate constructor.
     *
     * @attention not available from %Python!
     * 
     * @param desc description of the saved game.
     * @param dir directory of the saved game.
     */
    gamedata (string desc, string dir); 
#endif

    /** 
     * Destructor.
     * 
     */
    ~gamedata ();

    /** 
     * Save a record to an opened file.
     * 
     * @param ogzstream& opened file to save to.
     */
    void put (ogzstream&);

    /** 
     * Load a record from an opened file.
     * 
     * @param igzstream& opened file to load from.
     * 
     * @return true in case of success, false otherwise.
     */
    bool get (igzstream&);
    
    // a bunch of methods to access the private attributes
    const char* directory () { return directory_.c_str (); }
    const char* description () { return description_.c_str (); }
    const char* location () { return location_.c_str (); }
    const char* time () { return time_.c_str (); }
    void set_description (string);
    void set_directory (string);


    /** 
     * Initialise the saved games array.
     * 
     */
    static bool init (string udir, string gdir, string gname); 
    
    /** 
     * Cleanup the saved games array.
     * 
     */
    static void cleanup (); 
    
    static bool load (u_int32 pos); 
    
    static bool save (u_int32 pos, string desc);
    
    /**
     * Unloads the current game.
     * 
     */ 
    static void unload (); 

    static gamedata* next_save ();
    
    /** 
     * Returns the user %data directory ($HOME/.adonthell).
     * 
     * 
     * @return user %data directory.
     */
    static string user_data_dir () 
    {
        return user_data_dir_; 
    }
    
    /** 
     * Returns the %game %data directory.
     * 
     * 
     * @return %game %data directory.
     */
    static string game_data_dir () 
    {
        return game_data_dir_; 
    }

    static gamedata * get_saved_game (u_int32 pos) 
    {
        return saves[pos];
    }

    static dictionary <quest *> quests ()
    {
        return data::quests;
    }

    static character* player ()
    {
        return data::the_player;
    }

    static dictionary<character*> characters ()
    {
        return data::characters;
    }

    static mapengine* map_engine ()
    {
        return data::map_engine;
    }

private:
#ifndef SWIG
    string directory_;                        // the game's location on the harddisk
    string description_;                      // user supplied description of the game
    string location_;                         // the map or area the player is on
    string time_;                             // the gametime of the save
    
    static string game_name; 
    
    /** 
     * Keeps track of available save games.
     * 
     */
    static vector<gamedata*> saves; 
     
    /**
     * $HOME/.adonthell
     * 
     */ 
    static string user_data_dir_; 

    /**
     * Game data directory.
     * 
     */ 
    static string game_data_dir_; 
#endif
};

#endif // GAMEDATA_H__
