/*
   $Id$

   Copyright (C) 2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   event_list.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the event_list class.
 * 
 */
 
#include "event_handler.h"
 
// Array with callbacks to return a newly instanciated event
new_event event_list::instanciate_event[MAX_EVENT];

// destructor
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
void event_list::put_state (ogzstream& out) const
{
    vector <event *>::iterator i;
    u_int32 nbr_events = events.size ();
    
    nbr_events >> out; 

    for (i = events.begin (); i != events.end (); i++)
        (*i)->save (out); 
}

// Loads an event_list from file
bool event_list::get_state (igzstream& in)
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
