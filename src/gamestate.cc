/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "gamestate.h"

// Hash table for storing all the game state variables with their value
// I am not quite sure how big the initial number of buckets should be.
// Have to do a few tests for this later!
hash_map<const char*, s_int32, hash<const char*>, equal_key> game_state::data (100);

// Set a variable to a new value
void game_state::set (const char *key, s_int32 value)
{
    if (!value) data.erase (key);
    else data[key] = value;
}

// Get the value of a variable
s_int32 game_state::get (const char *key)
{
    return data[key];
}
