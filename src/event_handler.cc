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
 * @file   event_handler.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the event_handler class.
 * 
 */

#include "event_handler.h"

// Array with the registered events; each type of event is kept in
// a vector of its own for faster access
vector<event*> event_handler::handlers[MAX_EVENT];

// See whether a matching event is registered and execute the
// according script(s) 
void event_handler::raise_event (const event& e)
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
