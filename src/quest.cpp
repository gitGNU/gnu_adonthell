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

#include "fileops.h"
#include "quest.h"

// Save a quest object
void quest::save (gzFile out)
{
    hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
    char *string;
    u_int32 j;
    
    // Save name
    fileops::put_string (out, name);
    
    // Save all variables and flags
    j = data.size ();
    gzwrite (out, &j, sizeof (j));
    
    for (i = data.begin (); i != data.end (); i++)
    {
        string = strdup ((*i).first);
        fileops::put_string (out, string);
        free (string);
        gzwrite (out, &(*i).second, sizeof (s_int32));
    }
}

// Load a quest object and add it to the quest-arrays
void quest::load (gzFile in)
{
    u_int32 i, size;
    s_int32 value;
    char *key;

    // load name
    name = fileops::get_string (in);
    
    // load all variables and flags
    gzread (in, &size, sizeof (size));
    for (i = 0; i < size; i++)
    {
        key = fileops::get_string (in);
        gzread (in, &value, sizeof (value));

        set (key, value);
    }
}
