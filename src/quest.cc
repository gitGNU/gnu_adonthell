/*
   Copyright (C) 2000 Kai Sterker <kai.sterker@gmail.com>
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

#include "quest.h"

dictionary <quest *> data::quests;

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
        set_val (key.c_str (), value);
    }
}
