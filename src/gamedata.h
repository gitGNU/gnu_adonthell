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


 
/**
 * Contains all the attributes related to a saved %game.
 * 
 */ 
class gamedata
{
public:
    gamedata ();
    gamedata (string desc, string dir);
    ~gamedata ();
    
    void put (ogzstream&);                     // save a record to disk
    bool get (igzstream&);                     // load a record from disk
    
    // a bunch of methods to access the private attributes
    const char* get_directory () { return directory.c_str (); }
    const char* get_description () { return description.c_str (); }
    const char* get_location () { return location.c_str (); }
    const char* get_time () { return time.c_str (); }
    void set_description (string);
    void set_directory (string);


    /** 
     * Initialise the saved games array.
     * 
     */
    static bool init (string udir, string gdir); 
    
    /** 
     * Cleanup the saved games array.
     * 
     */
    static void cleanup (); 
    
    static bool load (u_int32 pos); 
    
    static bool save (u_int32 pos, string desc);
    
    static gamedata* next_save ();
    
    /** 
     * Returns the user data directory ($HOME/.adonthell).
     * 
     * 
     * @return user data directory.
     */
    static string user_data_dir () 
    {
        return user_data_dir_; 
    }
    
    /** 
     * Returns the game data directory.
     * 
     * 
     * @return game data directory.
     */
    static string game_data_dir () 
    {
        return game_data_dir_; 
    }

    static gamedata * get_saved_game (u_int32 pos) 
    {
        return saves[pos];
    }
    
private:
#ifndef SWIG
    string directory;                        // the game's location on the harddisk
    string description;                      // user supplied description of the game
    string location;                         // the map or area the player is on
    string time;                             // the gametime of the save
    
    /** 
     * Keeps track of available save games.
     * 
     */
    static vector<gamedata*> saves; 
    
    /**
     * Unloads the current game.
     * 
     */ 
    static void unload (); 

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
