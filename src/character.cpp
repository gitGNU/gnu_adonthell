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

#include "character.h"
#include "eval.h"
#include "game.h"
#include "dialog.h"
#include "py_inc.h"

void character::save (FILE *out)
{
    hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
    int j;

    // Save name
    j = strlen (name) + 1;
    fwrite (&j, sizeof (j), 1, out);
    fwrite (name, j, 1, out);        

    // Save position
    fwrite (&posx, sizeof(posx), 1, out);
    fwrite (&posy, sizeof(posy), 1, out);

    // Save all attributes and flags
    j = data.size ();
    fwrite (&j, sizeof (j), 1, out);
    
    for (i = data.begin (); i != data.end (); i++)
    {
        j = strlen ((*i).first) + 1;
        fwrite (&j, sizeof (j), 1, out);
        fwrite ((*i).first, j, 1, out);
        fwrite (&(*i).second, sizeof (s_int32), 1, out);
    }
}

void character::load (FILE *in)
{
    int i, j, size, value;
    char *key;

    // load name
    fread (&size, sizeof(size), 1, in);
    name = new char[size];
    fread (name, size, 1, in);

    // Load position
    fread (&posx, sizeof(posx), 1, in);
    fread (&posy, sizeof(posy), 1, in);

    // load all attributes and flags
    fread (&size, sizeof(size), 1, in);
    for (i = 0; i < size; i++)
    {
        fread (&j, sizeof(j), 1, in);
        key = new char[j];
        fread (key, j, 1, in);
        fread (&value, sizeof (value), 1, in);

        set (key, value);
    }
}

// Init NPC
npc::npc ()
{
    schedule = NULL;
    dialogue = NULL;
    schedule_file = NULL;
}

// Cleanup NPC
npc::~npc ()
{
    Py_XDECREF (schedule);
    if (dialogue) free (dialogue);
    if (schedule_file) free (schedule_file);
}

// Set/change active dialogue
void npc::set_dialogue (char *dlg)
{
    if (dialogue) free (dialogue);
    dialogue = strdup (dlg);
}

// Set/change active schedule
void npc::set_schedule (char* file)
{
    char script[255];
    strcpy (script, "scripts/");
    strcat (script, file);
    strcat (script, ".py");
    
    FILE *f = fopen (script, "r");

    // See whether the script exists at all
    if (f)
    {
        // Compile the script into a PyCodeObject for quicker execution
        _node *n = PyParser_SimpleParseFile (f, script, Py_file_input);
        if (n)
        {
            // If no errors occured update schedule code ...
            Py_XDECREF (schedule);
            schedule = PyNode_Compile (n, file);

            // ... and the schedule script file
            if (schedule_file) free (schedule_file);
            schedule_file = strdup (file);
        }
        else
        {
            cout << "\n*** Cannot set " << name << "'s schedule: Error in" << flush;
            show_traceback ();
        }
        fclose (f);
    }
    else cout << "\n*** Cannot open " << name << "'s schedule: file \"" << script
              << "\" not found!" << flush;
}

// Returns the NPC's dialogue
char* npc::get_dialogue ()
{
    char *str = new char[strlen (dialogue) + 11];
    strcpy (str, "dialogues/");
    strcat (str, dialogue);
    
    return str;
}

// Start conversation with the NPC
void npc::talk ()
{
    dialog_engine *de = new dialog_engine (name);
    PyDict_SetItemString (game::globals, "the_npc", pass_instance (this, "npc"));
    de->run ();
}

// Execute the active schedule and return the direction of movement
// or zero if character doesn't move.
u_int8 npc::move (u_int8 dir)
{
    PyObject *locals = Py_BuildValue ("{s:i,s:s}", "direction", dir, "name", name);
    PyEval_EvalCode (schedule, game::globals, locals);

#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_

    dir = PyInt_AsLong (PyDict_GetItemString (locals, "direction"));
    Py_DECREF (locals);

    return dir;
}

// save npc to disk
void npc::save (FILE *out)
{
    int i;

    character::save (out);

    if (!dialogue) dialogue = "";
    i = strlen (dialogue) + 1;
    fwrite (&i, sizeof (i), 1, out);
    fwrite (dialogue, i, 1, out);
    
    if (!schedule_file) schedule_file = "";
    i = strlen (schedule_file) + 1;
    fwrite (&i, sizeof (i), 1, out);
    fwrite (schedule_file, i, 1, out);
}

// load npc from disk
void npc::load (FILE *in)
{
    int i;

    character::load (in);

    fread (&i, sizeof (i), 1, in);
    if (dialogue) free (dialogue);
    dialogue = (char*) malloc (i);
    fread (dialogue, i, 1, in);
    
    fread (&i, sizeof (i), 1, in);
    if (schedule_file) free (schedule_file);
    schedule_file = (char*) malloc (i);
    fread (schedule_file, i, 1, in);
    set_schedule (schedule_file);   
}