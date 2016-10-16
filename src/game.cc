/*
   Copyright (C) 1999/2000/2001/2002 Kai Sterker <kai.sterker@gmail.com>
   Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file   game.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the game class.
 * 
 * 
 */


#include "game.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


string game::User_data_dir; 
string game::Global_data_dir; 
string game::Game_data_dir; 


void game::init (string game_dir) 
{
    Global_data_dir = game_dir;

    // set OS specific directory containing the saved games
    User_data_dir = get_system_dir(USER_DATA);
}

void game::set_game_data_dir(string game_dir)
{
    Game_data_dir = game_dir;
}

string game::get_system_dir(const sys_dir_type & type)
{
    std::string result;
#if defined(__APPLE__)		
    // OSX
    result = string (getenv ("HOME")) + "/Library/Application Support/Adonthell/";
#elif defined (WIN32)
    // Windows
    char *appDataDir = getenv ("APPDATA");
    if (appDataDir != NULL && strlen (appDataDir) > 0)
        result = string (getenv("APPDATA")) + "/Adonthell/";
    else
	result = "./";
#elif defined(__HAIKU__)
    // Haiku
    char *homeDir = getenv ("HOME");
    if (homeDir != NULL)
        result = string (homeDir) + "/config/settings/adonthell/";
    else
        result = string ("/boot/home/config/settings/adonthell/");
#else
    // Unix
    const char* xdgEnv = type == USER_DATA ? "XDG_DATA_HOME" : "XDG_CONFIG_HOME";
    char *xdgDir = getenv (xdgEnv);
    if (xdgDir != NULL && strlen (xdgDir) > 0)
        result = string (xdgDir) + "/adonthell";
    else
    {
        if (type == USER_DATA)
            result = string (getenv ("HOME")) + "/.local/share/adonthell/";
        else
            result = string (getenv ("HOME")) + "/.config/adonthell/";
    }
#endif

    // make sure save data directory exists, otherwise create it
    if (!game::directory_exist (result))
    {
#ifndef WIN32
        if (mkdir (result.c_str (), 0700) == -1)
#else
        if (mkdir (result.c_str ()) == -1)
#endif
        {
            int ecd = errno;
            std::cerr << "Creating directory '" << result << "' failed: "
                       << strerror (ecd) << std::endl;
        }
    }

    return result;
}

bool game::directory_exist (const string & dirname)
{
    DIR * dir = opendir (dirname.c_str ());

    if (dir) 
    {
        closedir (dir);
        return true; 
    }

    return false; 
}

bool game::file_exist (const string & fname) 
{
    FILE * file = fopen (fname.c_str (), "r");

    if (file) 
    {
        fclose (file);
        return true; 
    }

    return false; 
}

string game::find_file (const string & fname) 
{
    string ret;

    // If the name is already absolute, no need to search...
    if (fname[0] == '/') return fname; 
    
    // First check in the current game directory
    if ((ret = game_data_dir () + "/") != "/" && file_exist (ret + fname))
        ret += fname; 
    // Then check the global data directory
    else if (file_exist ((ret = global_data_dir () + "/") + fname)) 
        ret += fname;
    // Finally, try the user data directory
    else if (file_exist ((ret = user_data_dir () + "/") + fname))
        ret += fname;
    // Nothing found! So bad...
    else ret = "";

    return ret; 
}

string game::find_directory (const string & dirname) 
{
    string ret;

    // If the name is already absolute, no need to search...
    if (dirname[0] == '/') return dirname; 

    // First check in the current game directory
    if ((ret = game_data_dir () + "/") != "/" && directory_exist (ret + dirname))
        ret += dirname; 
    // Then check the global data directory
    else if (directory_exist ((ret = global_data_dir () + "/") + dirname)) 
        ret += dirname;
    // Finally, try the user data directory
    else if (directory_exist ((ret = user_data_dir () + "/") + dirname))
        ret += dirname;
    // Nothing found! So bad...
    else ret = "";

    return ret; 
}
