/*
   $Id$

   Copyright (C) 1999/2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   game.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the game class.
 * 
 * 
 */


#include "game.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


string game::User_data_dir; 
string game::Global_data_dir; 
string game::Game_data_dir; 


void game::init (string game_dir) 
{
    Global_data_dir = game_dir;
#ifndef WIN32
    User_data_dir = getenv ("HOME");
    User_data_dir += "/.adonthell";
#else
    User_data_dir = Global_data_dir;
#endif
}

void game::set_game_data_dir(string game_dir)
{
    Game_data_dir = game_dir;
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
