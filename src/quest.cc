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

#include "quest.h"

dictionnary <quest *> data::quests; 

quest::quest ()
{
}

quest::~quest ()
{
}

// Save a quest object
void quest::save (ogzstream& out)
{
    storage::iterator i;
    u_int32 j;
    
    // Save name
    name >> out; 
//     fileops::put_string (out, name);
    
    // Save all variables and flags
    j = size ();
    j >> out; 
//     gzwrite (out, &j, sizeof (j));
    
    for (i = begin (); i != end (); i++)
    {
        string s = (*i).first;
        s >> out;
        (*i).second >> out; 
//         string = strdup ((*i).first);
//         fileops::put_string (out, string);
//         free (string);
//         gzwrite (out, &(*i).second, sizeof (s_int32));
    }
}

// Load a quest object and add it to the quest-arrays
void quest::load (igzstream& in)
{
    u_int32 i, size;
    s_int32 value;
    string key; 
    
    // load name
//     if (name) delete[] name;
//     name = fileops::get_string (in);
    name << in; 
    
    // load all variables and flags
//     gzread (in, &size, sizeof (size));
    size << in; 
    for (i = 0; i < size; i++)
    {
//         key = fileops::get_string (in);
        key << in; 
//         gzread (in, &value, sizeof (value));
        value << in; 
        set (key.c_str (), value);
    }
}
