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

#include <algorithm>
#include "event_list.h"
#include "event_handler.h"
 
// Array with callbacks to return a newly instanciated event
new_event event_list::instanciate_event[MAX_EVENTS];

// constructor
event_list::event_list ()
{
    Paused = false;
}

// destructor
event_list::~event_list ()
{
    clear (); 
}

// Unregisters and deletes all events.
void event_list::clear () 
{
    event *ev;
    
    while (!Events.empty ())
    {
        ev = Events.back ();
        ev->set_list (NULL);
        Events.pop_back ();
        delete ev;    
    }    
}

// Adds an event to the list and register it with the event_handler.
void event_list::add_event (event* ev)
{
    ev->set_list (this);
    Events.push_back (ev);
    if (Paused) ev->pause ();
    event_handler::register_event (ev); 
}

// Remove an event from the list
void event_list::remove_event (event *ev)
{
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (Events.begin (), Events.end (), ev);

    // found? -> get rid of it :)
    if (i != Events.end ()) Events.erase (i);
}

// disable all events in the list
void event_list::pause ()
{
    Paused = true;
    for (vector<event*>::iterator i = Events.begin (); i != Events.end (); i++)
        (*i)->pause ();
}

// enable all events in the list
void event_list::resume ()
{
    Paused = false;
    for (vector<event*>::iterator i = Events.begin (); i != Events.end (); i++)
        (*i)->resume ();
}

// Register an event for loading
void event_list::register_event (u_int8 type, new_event e)
{
    if (type < MAX_EVENTS)
        instanciate_event[type] = e;
}

// Save an event_list to file
void event_list::put_state (ogzstream& out) const
{
    std::vector <event *>::iterator i;
    u_int32 nbr_events = Events.size ();
    
    nbr_events >> out; 

    for (i = Events.begin (); i != Events.end (); i++)
        (*i)->put_state (out); 
}

// Loads an event_list from file
bool event_list::get_state (igzstream& in)
{
    u_int32 nbr_events;
    u_int8 type;
    event *ev;

    nbr_events << in;
    
    while (nbr_events--) 
    {
        ev = NULL;
        type << in;
        
        // Instanciate an event of the given type
        if (type < MAX_EVENTS && instanciate_event[type] != NULL)
            ev = instanciate_event[type]();
 
        // try to load it, ...
        if (ev != NULL && ev->get_state (in))
            add_event (ev);
        
        // ... otherwise fail.
        else
        {
            fprintf (stderr, "Could not load event #%i. Aborting ...\n", type);
            return false;
        }    
    }
    
    return true;
}
