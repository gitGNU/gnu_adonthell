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

#include "event.h"
#include "py_inc.h"
#include "game.h"
#include "Python.h"

vector<event*> event_handler::handlers[MAX_EVENT];

void event_handler::raise_event (event *e)
{
    vector<event*> reg_evs = handlers[e->type];
    vector<event*>::iterator i;

    for (i = reg_evs.begin (); i != reg_evs.end (); i++)
        if (e->equals (*i)) (*i)->execute (e); 
}

void event_handler::register_event (event *e, const char *f)
{
    e->file = strdup (f);
    handlers[e->type].push_back (e);
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
    FILE *f = fopen (file, "r");
    if (f)
    {
        // PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t->x, 
        //    "posy", t->y, "dir", t->dir, "map", t->map, "name", t->c->name);
        PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t->x, 
            "posy", t->y, "dir", t->dir, "map", t->map, "name", "Kai");
        PyRun_File (f, file, Py_file_input, game::globals, locals);
        show_traceback ();
        fclose (f);
    }
    else cout << "\n*** Enter-event: cannot open \"" << file << "\"!" << flush;
}