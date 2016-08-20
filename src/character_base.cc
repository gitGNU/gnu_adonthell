/*
   Copyright (C) 2000/2001 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   character_base.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Defines the character_base class.
 * 
 * 
 */


#include "character_base.h"
#include <iostream>

using namespace std; 


character_base::character_base ()
{
    color = 1;
    name = "";
    dialogue = "";

    // characters are NPC's by default
    set_val ("type", NPC);
}

character_base::~character_base ()
{
}

void character_base::set_name (string newname)
{
    name = newname;
}

void character_base::set_dialogue (string newdlg)
{
    dialogue = newdlg; 
}

void character_base::put_state(ogzstream& out)
{
    storage::iterator i;
    
    u_int32 j;
    
    // Save name
    name >> out; 
    
    // save color
    color >> out; 
    
    // Save all attributes and flags
    j = size ();
    j >> out; 
    
    for (i = begin (); i != end (); i++)
    {
        string s = (*i).first;
        s >> out; 
        (*i).second >> out; 
    }
    
    dialogue >> out;
    portrait >> out; 
}

void character_base::get_state (igzstream& in)
{
    u_int32 i, size;
    s_int32 value;
    
    // load name
    name << in; 

    // load color
    color << in; 
    
    // load all attributes and flags
    size << in; 
    for (i = 0; i < size; i++)
    {
        string key;
        key << in;
        
        /// @bug : We should be able to pass a string to objects
        /// instead of a char *, which memory isn't freed at exit.
        value << in;
        set_val (key.c_str (), value);
    }

    dialogue << in;
    portrait << in; 
}
