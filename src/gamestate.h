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

#ifndef _GAME_STATE_
#define _GAME_STATE_

#include <string.h>
#include <hash_map>

#include "types.h"


// Checks two strings for equality
struct equal_key
{
    bool operator() (const char* s1, const char* s2) const
    {
        return strcmp (s1, s2) == 0;
    } 
};


// Game state storage
class game_state
{
public:
    static void set (const char*, s_int32);
    static s_int32 get (const char*);
    
protected:
    static hash_map<const char*, s_int32, hash<const char*>, equal_key> data;
};

#endif // _GAME_STATE_
