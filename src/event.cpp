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

#include "py_inc.h"
#include "game.h"
#include "compile.h"
#include "eval.h"
#include "event.h"

// Array with the registered events; each type of event is kept in
// a vector of its own for faster access
vector<event*> event_handler::handlers[MAX_EVENT];

// See whether a matching event is registered and execute the
// according script(s) 
void event_handler::raise_event (event *e)
{
    vector<event*> reg_evs = handlers[e->type];
    vector<event*>::iterator i;

    // Search through all registered events with the type of the raised event
    for (i = reg_evs.begin (); i != reg_evs.end (); i++)
        // Execute the script; pass recieved event on to get event data
        if (e->equals (*i)) (*i)->execute (e); 
}

// Register a event with it's script
void event_handler::register_event (event *e, char *file)
{
    FILE *f = fopen (file, "r");
    // See whether the script exists at all
    if (f)
    {
        // Compile the script into a PyCodeObject for quicker execution
        _node *n = PyParser_SimpleParseFile (f, file, Py_file_input);
        if (n)
        {
            e->script = PyNode_Compile (n, file);       
            handlers[e->type].push_back (e);
        }
        else
        {
            cout << "\n*** Cannot register event: Error in" << flush;
            show_traceback ();
        }
        fclose (f);
    }
    else cout << "\n*** Cannot register event: file \"" << file << "\" not found!" << flush;
}


enter_event::enter_event ()
{ 
    type = ENTER_EVENT; 
    x = y = dir = map = -1;
    c = NULL;
}

bool enter_event::equals (event *e)
{
    // we know that we've got an enter_event :)
    enter_event *tmp = (enter_event *) e;

    if (tmp->x != -1 && tmp->x != x) return false;
    if (tmp->y != -1 && tmp->y != y) return false;
    if (tmp->dir != -1 && tmp->dir != dir) return false;
    if (tmp->map != -1 && tmp->map != map) return false;
    if (tmp->c && tmp->c != c) return false;

    return true;
}

void enter_event::execute (event *e)
{
    enter_event *t = (enter_event *) e;

    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t->x, 
        "posy", t->y, "dir", t->dir, "map", t->map, "name", t->c->name);
    // Execute script
    PyEval_EvalCode (script, game::globals, locals);
    // Cleanup
    Py_DECREF (locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
}