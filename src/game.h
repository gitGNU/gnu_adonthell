/*
   $Id$

   Copyright (C) 1999/2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   game.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the game class.
 * 
 * 
 */



#ifndef GAME_H__
#define GAME_H__


#include <string>
#include "types.h"

#ifndef SWIG 
using std::string; 
#endif

typedef enum { CONFIG, USER_DATA } sys_dir_type;

/**
 * Holds information about global settings.
 *
 * This static class should be the first to be initialised in your application,
 * because many others depends on it's correct settings.
 * 
 */
class game
{
public:
    static string User_data_dir;
    static string Global_data_dir;
    static string Game_data_dir; 

    /** 
     * Initialise the game framework.
     * 
     * @param game_dir Global data directory.
     */
    static void init (string game_dir); 

    /** 
     * Specify an additional data directory containing game data.
     * 
     * @param game_dir Game data directory.
     */
    static void set_game_data_dir (string game_dir); 
    
    /** 
     * Returns the absolute path to the user data directory (usually ~/.adonthell).
     * 
     * 
     * @return user data directory
     */
    static string user_data_dir ()
    {
        return User_data_dir; 
    }

    /** 
     * Returns the absolute path to the global data directory.
     * 
     * 
     * @return global data directory
     */
    static string global_data_dir ()
    {
        return Global_data_dir; 
    }

    /** 
     * Returns the absolute path to the current game's directory (if any).
     * 
     * 
     * @return current game data directory, or empty string if none set.
     */
    static string game_data_dir ()
    {
        return Game_data_dir; 
    }

    /** 
     * Finds a file in the directories hierarchy, starting searching from
     * game_data_dir(), then global_data_dir() and finally user_data_dir().
     *
     * If a matching file is found, the full absolute path is returned, else
     * an empty string "" is returned. If the path was already absolute, it is
     * returned immediatly.
     * 
     * @param fname name of the find to search for.
     * 
     * @return complete absolute path to the file if found, passed string if the given
     *         path was already absolute, or "" if the file wasn't found.
     */
    static string find_file (const string & fname);

    /** 
     * Finds a directory in the directories hierarchy, starting searching from
     * game_data_dir(), then global_data_dir() and finally user_data_dir().
     *
     * If a matching directory is found, the full absolute path is returned, else
     * an empty string "" is returned. If the path was already absolute, it is
     * returned immediatly.
     * 
     * @param fname name of the find to search for.
     * 
     * @return complete absolute path to the directory if found, passed string if the given
     *         path was already absolute, or "" if the directory wasn't found.
     */
    static string find_directory (const string & dirname); 

#ifndef SWIG
    /**
     * Return the OS-specific directory of the given type. If the directory does
     * not yet exist, it is also created.
     *
     * @param type Either CONFIG or USER_DATA
     *
     * @return appropriate system directory for the current user.
     */
    static string get_system_dir(const sys_dir_type & type);

private:
    static bool directory_exist (const string & dirname); 
    static bool file_exist (const string & fname); 
#endif
};


#endif // GAME_H__
