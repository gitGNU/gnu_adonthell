/*
   $Id$
   
   Copyright (C) 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   character_base.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the character_base class.
 * 
 * 
 */


#include "character_base.h"
#include <iostream>

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
