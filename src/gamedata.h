/*
   $Id$

   Copyright (C) 2001/2002 by Kai Sterker <kaisterker@linuxgames.com>
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

#include "quest.h"
#include "character.h"
#include "adonthell.h"

/**
 * Contains all the attributes related to a saved %game and the
 * high level methods for loading/saving the %game.
 *
 * A word about saved games: all games are stored inside
 * $HOME/.adonthell/ into a individual subdirectory, consisting
 * of the %game's name (e.g. wastesedge) with the appendix "-save-xxx"
 * where "xxx" is a number between 001 and 999. All %data that belongs
 * to a saved %game is contained in that directory, thus allowing
 * to copy individual games to another machine and/or user.
 *
 * The numbering of the %game directories has no special meaning. Saved
 * games are recognized by the first part of their name, and saving a
 * new %game will never overwrite an existing.
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
     * @param desc description of the saved %game.
     * @param dir directory of the saved %game.
     * @param time Textual representation of in-game time.
     */
    gamedata (string desc, string dir, string time); 
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
     * Returns the directory where the saved %game lies.
     * 
     * @return Directory where the saved %game lies.
     */
    const char* directory () { return Directory.c_str (); }

    /**
     * Returns the description of the saved %game.
     *
     * @return Description of the saved %game.
     */ 
    const char* description () { return Description.c_str (); }

    /** 
     * Returns the location of the saved %game.
     * 
     * @return Location of the saved %game.
     */
    const char* location () { return Location.c_str (); }

    /** 
     * Returns the in-game time of the saved %game.
     * 
     * @return In-game time of the saved %game.
     */
    const char* gametime () { return Gametime.c_str (); }

    /**
     * Returns the (real) time when this game has been saved
     *
     * @return (Real) time when this game has been saved
     */
    u_int32 timestamp () { return Timestamp; }
    
    /** 
     * Sets the description for this %game.
     * 
     * @param string New description for this %game.
     */
    void set_description (string);

    /** 
     * Sets the directory for this %game.
     * 
     * @param string New directory for this %game.
     */
    void set_directory (string);

    /** 
     * Set the in-game time of the saved %game.
     * 
     * @param string In-game time of the saved %game.
     */
    void set_gametime (string);
    //@}

    /** 
     * Initialise the saved games array. Searches the user directory
     * for available save games and loads their description.
     *
     * @param udir The user directory, usually $HOME/.adonthell
     * @param gdir The %game data directory, usually /usr/local/share/adonthell
     * @param gname The name of the %game we are running, e.g. wastesedge
     * @param qload Whether quick-loading should be enabled or disabled
     *
     * @return \e true in case of success, false otherwise.
     */
    static bool init (string udir, string gdir, string gname, u_int8 qload); 
    
    /** 
     * Cleanup the saved %game array.
     * 
     */
    static void cleanup (); 

    /** 
     * Load the characters state from a saved %game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_characters (u_int32 pos); 
 
    /** 
     * Load the quests state from a saved %game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_quests (u_int32 pos); 

    /** 
     * Load the mapengine state from a saved %game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_mapengine (u_int32 pos); 

    /** 
     * Load the audio system state from a saved %game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_audio (u_int32 pos);

    static bool load_achievements (u_int32 pos);

    /** 
     * Loads a previously saved %game. Slot 0 points to the
     * initial %game %data and needs to be loaded when starting
     * a fresh %game.
     * 
     * @param pos Slot number to load.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load (u_int32 pos); 
    
    /** 
     * Loads the most recent saved %game. This method only takes
     * games created by the player into account, not the initial
     * saved %game.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    static bool load_newest (); 

    /** 
     * Save a %game. When given a slot number in the range of
     * the available saved games, the according %game will be
     * overwritten, otherwise a new saved %game is created.
     * Saving to slot 0 is not possible, as it contains the
     * initial %game %data.
     * 
     * @param pos Slot number where to save to.
     * @param desc Description of the %game to be saved.
     * @param time Textual representation of in-game time.
     * 
     * @return \e true in case of success, false otherwise.
     */
    static bool save (u_int32 pos, string desc, string time);
    
    /**
     * Unloads the current %game, resetting the engine to it's
     * initial state.
     * 
     */ 
    static void unload (); 

    /** 
     * Returns a pointer to the next saved %game.
     * 
     * 
     * @return Next saved %game.
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
     * Returns a pointer to a saved %game.
     * 
     * @param pos Slot number to return.
     * 
     * @return Pointer to the saved %game at position \pos.
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
     * Returns the player %character.
     * 
     * 
     * @return Player %character.
     */
    static character* player ()
    {
        return data::the_player;
    }
 
    /**
     * Returns a certain NPC when given the name. Use player () to get
     * the player %character, as his/her name will be set at runtime.
     *
     * @param name The name of the %character to return
     *
     * @return a %character.
     */
    static character* get_character (string name)
    {
        return data::characters [name]; 
    }

    /**
     * Returns a certain quest when given the name.
     *
     * @param name The name of the %quest to return
     *
     * @return a %quest
     */
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
     * Returns a pointer to the global game engine.
     * 
     * 
     * @return Pointer to the global game engine.
     */
    static adonthell* engine ()
    {
        return data::engine;
    }

private:
#ifndef SWIG
    string Directory;               // the game's location on the harddisk
    string Description;             // user supplied description of the game
    string Location;                // the map or area the player is on
    string Gametime;                // the gametime of the saved game
    u_int32 Timestamp;              // time of last save to this file
    
    static string game_name; 
    static u_int8 quick_load;
    
    /** 
     * Keeps track of available saved games.
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
