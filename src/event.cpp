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

#include <algorithm>
#include <stdio.h>

#include "fileops.h"
#include "py_inc.h"
#include "data.h"
#include "compile.h"
#include "eval.h"
#include "event.h"
#include "character.h"

// "Automatic" unregistering of events no longer needed
event_list::~event_list ()
{
    event *e;

    while (!events.empty ())
    {
        e = events.back ();
        events.pop_back ();
        event_handler::remove_event (e);
        delete e;    
    }    
}

event::~event ()
{
    if (script_file) delete script_file;
    if (script) delete script;
}

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
        if ((*i)->equals (e)) (*i)->execute (e); 
}

// Unregister an event
void event_handler::remove_event (event *e)
{
    vector<event*> reg_evs = handlers[e->type];
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (reg_evs.begin (), reg_evs.end (), e);

    // found? -> get rid of it :)
    if (i != reg_evs.end ()) reg_evs.erase(i); 
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

// Load (and register an event)
event* event_handler::load_event (gzFile f, bool reg)
{
    u_int8 type;
    char *script_file;
    event *e = NULL;

    gzread (f, &type, sizeof (type));

    switch (type)
    {
        case ENTER_EVENT:
        {
            e = new enter_event;
            break;
        }

        case LEAVE_EVENT:
        {
            e = new leave_event;
            break;
        }

        case TIME_EVENT:
        {
            e = new time_event;
            break;
        }
        
        default:
        {
            cout << "\n*** Cannot load event. Unknown event tyoe!" << flush;
            return e;
        }
    }
    
    e->load (f);

    // should we register the event?
    if (reg)
    {
        script_file = new char[strlen (e->script_file) + 20];
        strcpy (script_file, "scripts/events/");
        strcat (script_file, e->script_file);
        strcat (script_file, ".py");
        
        register_event (e, script_file);
        delete script_file;
    }
    
    return e;
}

// =======================================================================
// HERE COME ALL THE DIFFERENT EVENTS:

enter_event::enter_event () : base_map_event ()
{
    type = ENTER_EVENT; 
}

leave_event::leave_event () : base_map_event ()
{
    type = LEAVE_EVENT; 
}

base_map_event::base_map_event ()
{
    x = y = dir = map = -1;
    c = NULL;
    script_file = NULL;
    script = NULL;
}

// compare two enter events
bool base_map_event::equals (event *e)
{
    // we know that we've got an enter_event :)
    base_map_event *tmp = (base_map_event *) e;

    if (x != -1 && tmp->x != x) return false;
    if (y != -1 && tmp->y != y) return false;
    if (dir != -1 && tmp->dir != dir) return false;
    if (map != -1 && tmp->map != map) return false;
    if (c && tmp->c != c) return false;

    return true;
}

// Execute enter event's script
void base_map_event::execute (event *e)
{
    base_map_event *t = (base_map_event *) e;

    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t->x, 
        "posy", t->y, "dir", t->dir, "map", t->map, "name", t->c->name);
    // Execute script
    PyEval_EvalCode (script, data::globals, locals);
    // Cleanup
    Py_DECREF (locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
}

// Load a enter event from file
void base_map_event::load (gzFile f)
{
    char* name;
        
    gzread (f, &x, sizeof (x));
    gzread (f, &y, sizeof (y));
    gzread (f, &dir, sizeof (dir));
    gzread (f, &map, sizeof (map));

    name = get_string (f);
    c = (character*) data::characters.get (name);
    delete name;

    script_file = get_string (f);
}

// Save enter_event to file
void base_map_event::save (gzFile out)
{
    gzwrite (out, &type, sizeof (type));
    gzwrite (out, &x, sizeof (x));
    gzwrite (out, &y, sizeof (y));
    gzwrite (out, &dir, sizeof (dir));
    gzwrite (out, &map, sizeof (map));    

    put_string (out, c->name);
    put_string (out, script_file);    
}

time_event::time_event ()
{
    time =minute = hour = day =  0;
    m_step = h_step = d_step = 1;
    script_file = NULL;
    type = TIME_EVENT;
    script = NULL;
}

// Save a time_event to file
void time_event::save (gzFile out)
{
    gzwrite (out, &type, sizeof (type));
    gzwrite (out, &minute, sizeof (minute));
    gzwrite (out, &m_step, sizeof (m_step));
    gzwrite (out, &hour, sizeof (hour));
    gzwrite (out, &h_step, sizeof (h_step));
    gzwrite (out, &day, sizeof (day));
    gzwrite (out, &d_step, sizeof (d_step));

    put_string (out, script_file);    
}

// Load a time event from file
void time_event::load (gzFile f)
{
    gzread (f, &minute, sizeof (minute));
    gzread (f, &m_step, sizeof (m_step));
    gzread (f, &hour, sizeof (hour));
    gzread (f, &h_step, sizeof (h_step));
    gzread (f, &day, sizeof (day));
    gzread (f, &d_step, sizeof (d_step));

    script_file = get_string (f);
}

// Execute time event's script
void time_event::execute (event *e)
{
    time_event *t = (time_event *) e;

    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i}", "minute", (int) t->minute, 
        "hour", (int) t->hour, "day", (int) t->day);
    // Execute script
    PyEval_EvalCode (script, data::globals, locals);
    // Cleanup
    Py_DECREF (locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
}

// Check whether this time_even matches a given gametime
bool time_event::equals (event *e)
{
    time_event *t = (time_event *) e;
    u_int32 time = t->time % 40320;
    u_int32 d, h, m = time % 60;
    
    if (m_step != 0) {
        if ((m - minute) % m_step != 0 || m < minute) return 0;
    }
    else
        if (m != minute) return 0;

    // this is the current hour
    h = ((time - m) % 1440) / 60;

    if (h_step != 0) {
        if ((h - hour) % h_step != 0 || h < hour) return 0;
    }
    else
        if (h != hour) return 0;
         
    // this is the current day
    d = (time - (m + h * 60)) / 1440;
    
    if (d_step != 0) {
        if ((d - day) % d_step != 0 || d < day) return 0; 
    }
    else
        if (d != day) return 0;

    // if the event matches, we set the actual minute, hour and day,
    // in case it is needed in the script
    t->minute = m;
    t->hour = h;
    t->day = d;
    
    return 1;
}