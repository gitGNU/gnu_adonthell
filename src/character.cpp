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
#include "py_inc.h"


// Init NPC
npc::npc ()
{
    schedule = NULL;

    // test:
    dialogues.push_back ("dialogues/tomas_hterin");
    active_dialogue = 0;
}

// Returns the active dialogue
char* npc::talk ()
{
    return dialogues[active_dialogue];
}

// Set the active dialogue
void npc::set_dialogue (u_int32 dlg)
{
    active_dialogue = dlg;
}

// Set/change active schedule
void npc::set_schedule (char* file)
{
    if (schedule) Py_DECREF (schedule);

    FILE *f = fopen (file, "r");

    // See whether the script exists at all
    if (f)
    {
        // Compile the script into a PyCodeObject for quicker execution
        _node *n = PyParser_SimpleParseFile (f, file, Py_file_input);
        if (n) schedule = PyNode_Compile (n, file);       
        else
        {
            cout << "\n*** Cannot set " << name << "'s schedule: Error in" << flush;
            show_traceback ();
        }
        fclose (f);
    }
    else cout << "\n*** Cannot open " << name << "'s schedule: file \"" << file
              << "\" not found!" << flush;
}

u_int8 npc::move (u_int8 dir)
{
    PyObject *locals = Py_BuildValue ("{s:i,s:s}", "the_dir", dir, "name", name);
    PyEval_EvalCode (schedule, game::globals, locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
    return PyInt_AsLong (PyDict_GetItemString (locals, "the_dir"));
}
