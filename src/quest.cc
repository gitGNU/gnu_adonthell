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
    
    // Save all variables and flags
    j = size ();
    j >> out; 
    
    for (i = begin (); i != end (); i++)
    {
        string s = (*i).first;
        s >> out;
        (*i).second >> out; 
    }
}

// Load a quest object and add it to the quest-arrays
void quest::load (igzstream& in)
{
    u_int32 i, size;
    s_int32 value;
    string key; 
    
    name << in; 
    
    size << in; 
    for (i = 0; i < size; i++)
    {
        key << in; 
        value << in; 
        set (key.c_str (), value);
    }
}
