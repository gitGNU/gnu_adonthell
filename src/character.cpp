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

#include "fileops.h"
#include "character.h"
#include "eval.h"
#include "data.h"
#include "py_inc.h"

character::character()
{
#ifndef _EDIT_
  PyDict_SetItemString(locals,"char_myself",pass_instance(this,"character"));
#endif
  char_instance=this;
}

void character::save (gzFile out)
{
    hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
    vector<event*>::iterator k;
    char *string;
    u_int32 j;

    // Save name
    put_string (out, name);
    
    // Save position
    gzwrite (out, &posx, sizeof(posx));
    gzwrite (out, &posy, sizeof(posy));

    // Save all attributes and flags
    j = data.size ();
    gzwrite (out, &j, sizeof (j));
    
    for (i = data.begin (); i != data.end (); i++)
    {
        string = strdup ((*i).first);
        put_string (out, string);
        free (string);
        gzwrite (out, &(*i).second, sizeof (s_int32));
    }

    // save events
    j = events.size ();
    gzwrite (out, &j, sizeof (j));
    
    for (k = events.begin (); k != events.end (); k++)
        (*k)->save (out);
}

void character::load (gzFile in, bool reg_event)
{
    u_int32 i, size;
    s_int32 value;
    event *e;
    char *key;

    // load name
    name = get_string (in);

    // We have to insert the character here *before* loading it's events
    data::characters.set (name, this);

    // Load position
    gzread (in, &posx, sizeof(posx));
    gzread (in, &posy, sizeof(posy));

    // load all attributes and flags
    gzread (in, &size, sizeof(size));
    for (i = 0; i < size; i++)
    {
        key = get_string (in);
        gzread (in, &value, sizeof (value));

        set (key, value);
    }

    // load all events
    gzread (in, &size, sizeof(size));
    for (i = 0; i < size; i++)
    {
        e = event_handler::load_event (in, reg_event);
        if (e) events.push_back (e);
    }

    color = 0;
}

// Init NPC
npc::npc ()
{
    schedule = NULL;
    dialogue = NULL;
    schedule_file = NULL;
#ifndef _EDIT_
    npc_instance=this;
    PyDict_SetItemString(locals,"npc_myself",pass_instance(this,"npc"));
#endif
}

// Cleanup NPC
npc::~npc ()
{
    if (schedule) delete schedule;
    if (dialogue) free (dialogue);
    if (schedule_file) free (schedule_file);
}

// Set/change active dialogue
void npc::set_dialogue (char *dlg)
{
    if (dialogue) free (dialogue);
    dialogue = strdup (dlg);
}

// Returns the NPC's dialogue
char* npc::get_dialogue ()
{
    char *str = new char[strlen (dialogue) + 11];
    strcpy (str, "dialogues/");
    strcat (str, dialogue);
    
    return str;
}

// save npc to disk
void npc::save (gzFile out)
{
    character::save (out);

    put_string (out, dialogue);
    put_string (out, schedule_file);
}

// load npc from disk
void npc::load (gzFile in, bool load_script)
{
    character::load (in, load_script);

    if (dialogue) free (dialogue);
    dialogue = get_string (in);    

    get_string (in);
}
