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
 * @file   event.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the event_list, event and event_handler class.
 * 
 * 
 */

#include <algorithm>

#include "fileops.h"
#include "python_class.h"
#include "event.h"

// Array with callbacks to return a newly instanciated event
new_event event_list::instanciate_event[MAX_EVENT];


event_list::~event_list ()
{
    clear (); 
}

// Unregisters and deletes all events.
void event_list::clear () 
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

// Adds an event to the list and register it with the event_handler.
void event_list::add_event (event* ev)
{
    events.push_back (ev);
    event_handler::register_event (ev); 
}

// Register an event for loading
void event_list::register_event (u_int8 type, new_event e)
{
    if (type < MAX_EVENT)
        instanciate_event[type] = e;
}

// Save an event_list to file
void event_list::save (ogzstream& out) const
{
    vector <event *>::iterator i;
    u_int32 nbr_events = events.size ();
    
    nbr_events >> out; 

    for (i = events.begin (); i != events.end (); i++)
        (*i)->save (out); 
}

// Loads an event_list from file
bool event_list::load (igzstream& in)
{
    u_int32 nbr_events;
    u_int8 type;

    nbr_events << in;
    
    while (nbr_events--) 
    {
        event * e = NULL;
        type << in;
        
        // Instanciate an event of the given type
        if (type < MAX_EVENT && instanciate_event[type] != NULL)
            e = instanciate_event[type]();
 
        // try to load it, ...
        if (e != NULL && e->load (in))
            add_event (e);
        
        // ... otherwise fail.
        else
        {
            fprintf (stderr, "Could not load event #%i. Aborting ...\n", type);
            return false;
        }    
    }
    
    return true;
}

event::event () 
{
    script_args = NULL; 
}

event::~event ()
{
}

void event::set_script (string filename, PyObject * args = NULL)
{
    if (filename == "") 
    {
        script.clear ();
        Py_XDECREF (script_args);
        script_args = NULL; 
    }
    else 
    {
        Py_XINCREF (args);
        script_args = args; 
        u_int16 argssize = args == NULL ? 1 : PyTuple_Size (args) + 1; 
        PyObject * theargs;
        
        theargs = PyTuple_New (argssize);
        
        // We can pass_instance directly 'cause PyTuple_SetItem steals a
        // reference to the result of pass_instance.
        PyTuple_SetItem (theargs, 0, python::pass_instance (this, "event"));
        for (u_int16 i = 1; i < argssize; i++)
        {
            PyObject * intref = PyTuple_GetItem (args, i - 1);
            Py_INCREF (intref); 
            PyTuple_SetItem (theargs, i, intref); 
        }
        script.create_instance (EVENTS_DIR + filename, filename, theargs);
        Py_DECREF (theargs); 
    }
    script_file_ = filename;
}

void event::put_script_state (ogzstream & file) const
{
    script_file () >> file;
    if (script_args) 
    {
        true >> file; 
        python::put_tuple (script_args, file);
    }
    else false >> file; 
}

void event::get_script_state (igzstream & file) 
{
    string t;
    bool bo;
    
    PyObject * args = NULL; 
    t << file;
    bo << file; 
    if (bo) args = python::get_tuple (file);
    set_script (t, args);      
    Py_XDECREF (args); 

}

// Array with the registered events; each type of event is kept in
// a vector of its own for faster access
vector<event*> event_handler::handlers[MAX_EVENT];

// See whether a matching event is registered and execute the
// according script(s) 
void event_handler::raise_event (event& e)
{
    vector<event*>::iterator i;
    // Search through all registered events with the type of the raised event
    for (i = handlers[e.type].begin (); i != handlers[e.type].end (); i++)
        // Execute the script; pass recieved event on to get event data
        if ((*i)->equals (e)) (*i)->execute (e); 
}


// Unregister an event
void event_handler::remove_event (event *e)
{
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (handlers[e->type].begin (), handlers[e->type].end (), e);

    // found? -> get rid of it :)
    if (i != handlers[e->type].end ()) handlers[e->type].erase(i);

}

// Register a event with it's script
void event_handler::register_event (event *e)
{
    handlers[e->type].push_back (e);
}
