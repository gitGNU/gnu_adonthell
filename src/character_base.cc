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

void character_base::save(ogzstream& out)
{
  hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
  u_int32 j;
  
  // Save name
  name >> out; 
  //   fileops::put_string (out, name);

  // save color
  color >> out; 
  //   gzwrite (out, &color, sizeof (color));
  
  // Save all attributes and flags
  j = data.size ();
  j >> out; 
//   gzwrite (out, &j, sizeof (j));
  
  for (i = data.begin (); i != data.end (); i++)
    {
      string s = (*i).first;
      s >> out; 
//       fileops::put_string (out, string);
//       free (string);
      (*i).second >> out; 
//       gzwrite (out, &(*i).second, sizeof (s_int32));
    }

  dialogue >> out; 
//   fileops::put_string (out, dialogue);
}

void character_base::load (igzstream& in)
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
