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

#include "game.h"
#include "py_inc.h"
#include "quest.h"

// Save a quest object
void quest::save (FILE *out)
{
    hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
    int j;

    // Save name
    j = strlen (name) + 1;
    fwrite (&j, sizeof (j), 1, out);
    fwrite (name, j, 1, out);        

    // Save all variables and flags
    j = data.size ();
    fwrite (&j, sizeof (j), 1, out);
    
    for (i = data.begin (); i != data.end (); i++)
    {
        j = strlen ((*i).first) + 1;
        fwrite (&j, sizeof (j), 1, out);
        fwrite ((*i).first, j, 1, out);
        fwrite (&(*i).second, sizeof (s_int32), 1, out);
    }
}

// Load a quest object and add it to the quest-arrays
void quest::load (FILE *in)
{
    int i, j, size, value;
    char *key;

    // load name
    fread (&size, sizeof(size), 1, in);
    name = new char[size];
    fread (name, size, 1, in);

    // load all variables and flags
    fread (&size, sizeof(size), 1, in);
    for (i = 0; i < size; i++)
    {
        fread (&j, sizeof(j), 1, in);
        key = new char[j];
        fread (key, j, 1, in);
        fread (&value, sizeof (value), 1, in);

        set (key, value);
    }

    // Make this quest available to the engine
    game::quests.set (name, this);
}
