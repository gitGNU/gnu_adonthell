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

    /**
     * A bunch of methods to access the private attributes.
     * 
     */
    //@{
    
    /** 
     * Returns the directory where the saved game lies.
     * 
     * 
     * @return Directory where the saved game lies.
     */
    const char* directory () { return directory_.c_str (); }

    /**
     * Returns the description of the saved game.
     * 
     *
     * @return Description of the saved game.
     */ 
    const char* description () { return description_.c_str (); }

    /** 
     * Returns the location of the saved game.
     * 
     * 
     * @return Location of the saved game.
     */
    const char* location () { return location_.c_str (); }

    /** 
     * Returns the time of the saved game.
     * 
     * 
     * @return Time of the saved game.
     */
    const char* time () { return time_.c_str (); }

    /** 
     * Sets the description for this game.
     * 
     * @param string New description for this game.
     */
    void set_description (string);

    /** 
     * Sets the directory for this game.
     * 
     * @param string New directory for this game.
     */
    void set_directory (string);

    //@}

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

    /** 
     * Load the characters state from a saved game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_characters (u_int32 pos); 
 
    /** 
     * Load the quests state from a saved game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_quests (u_int32 pos); 

    /** 
     * Load the mapengine state from a saved game. 
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_mapengine (u_int32 pos); 


    /** 
     * Loads a previously saved game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load (u_int32 pos); 
    
    /** 
     * Save a game.
     * 
     * @param pos Slot number where to save to.
     * @param desc Description of the game to be saved.
     * 
     * @return \e true in case of success, false otherwise.
     */
    static bool save (u_int32 pos, string desc);
    
    /**
     * Unloads the current game.
     * 
     */ 
    static void unload (); 

    /** 
     * Returns a pointer to the next saved game.
     * 
     * 
     * @return Next saved game.
     */
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

    /** 
     * Returns a pointer to a saved game.
     * 
     * @param pos Slot number to return.
     * 
     * @return Pointer to the saved game at position \pos.
     */
    static gamedata * get_saved_game (u_int32 pos) 
    {
        return saves[pos];
    }

    /** 
     * Returns the global quests dictionary.
     * 
     * 
     * @return Global quests dictionary.
     */
    static dictionary <quest *> quests ()
    {
        return data::quests;
    }

    /** 
     * Returns the player character.
     * 
     * 
     * @return Player character.
     */
    static character* player ()
    {
        return data::the_player;
    }
 
    static character* get_character (string name) 
    {
        return data::characters [name]; 
    }

    static quest* get_quest (string name)
    {
        return data::quests [name];
    }

    /** 
     * Returns the characters dictionary
     * 
     * 
     * @return Characters dictionary.
     */
    static dictionary<character*> characters ()
    {
        return data::characters;
    }

    /** 
     * Returns a pointer to the global mapengine.
     * 
     * 
     * @return Pointer to the global mapengine.
     */
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
