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

#include <stdio.h>
#include <string.h>
#include "fileops.h"
#include "character_base.h"
#include "data.h"

character_base::character_base ()
{
  dialogue = NULL;
  name = NULL;
  color = 0;
}

character_base::~character_base ()
{
  if (dialogue) delete dialogue;
  if (name) delete name;
}

void character_base::set_name (const char * newname)
{
  if (name) delete name;
  if (newname==NULL) name=NULL;
  else
    {
      name = new char[strlen (newname)+1];
      strcpy (name, newname);
    }
}

void character_base::set_dialogue (const char *newdlg)
{
    if (dialogue) delete dialogue;
    if (!newdlg) dialogue = NULL;
    else
    {
        dialogue = new char[strlen (newdlg) + strlen (DIALOG_DIR) + 1];
        strcpy (dialogue, DIALOG_DIR);
        strcat (dialogue, newdlg);
    }
}

void character_base::save(gzFile out)
{
  hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
  char *string;
  u_int32 j;
  
  // Save name
  fileops::put_string (out, name);
    
  // Save all attributes and flags
  j = data.size ();
  gzwrite (out, &j, sizeof (j));
  
  for (i = data.begin (); i != data.end (); i++)
    {
      string = strdup ((*i).first);
      fileops::put_string (out, string);
      free (string);
      gzwrite (out, &(*i).second, sizeof (s_int32));
    }

  fileops::put_string (out, dialogue);
}

void character_base::load (gzFile in)
{
    u_int32 i, size;
    s_int32 value;
    char *key;

    // load name
    name = fileops::get_string (in);

#ifndef _EDIT_
    // make the character available from data
    data::characters.set (name, this);
#endif

    // load all attributes and flags
    gzread (in, &size, sizeof(size));
    for (i = 0; i < size; i++)
    {
        key = fileops::get_string (in);
        gzread (in, &value, sizeof (value));
        set (key, value);
    }

    dialogue = fileops::get_string (in);
}
