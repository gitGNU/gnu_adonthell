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


/**
 * @file   character_base.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the character_base class.
 * 
 * 
 */


#include "character_base.h"
#include "data.h"


using namespace std; 


character_base::character_base ()
{
  color = 1;
}

character_base::~character_base ()
{
}

void character_base::set_name (const string & newname)
{
    name = newname; 
}

void character_base::set_dialogue (const string & newdlg)
{
    dialogue = newdlg; 
}

void character_base::put_state(ogzstream& out)
{
//   hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
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
}

void character_base::get_state (igzstream& in)
{
    u_int32 i, size;
    s_int32 value;
    
    // load name
    name << in; 

    // load color
    color << in; 

    // make the character available from data
    data::characters.set (name.c_str (), this);

    // load all attributes and flags
    size << in; 
    for (i = 0; i < size; i++)
    {
        char * s; 
        string key; 
        key << in;
        
        /// @bug : We should be able to pass a string to objects
        /// instead of a char *, which memory isn't freed at exit.

        s = new char [key.size () + 1]; 
        strcpy (s, key.c_str ()); 
        value << in;
        set (s, value);
    }

    dialogue << in; 
}
