/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file time_event_handler.cc
 *
 * @author Kai Sterker
 * @brief Implements the time_event_handler class.
 */

#include <algorithm>
#include "time_event_handler.h"
#include "time_event.h"

// See whether a matching event is registered and execute the
// according script(s) 
void time_event_handler::raise_event (event& e)
{
    // no time event registered
    if (Events.empty ()) return;
    
    // As long as matching events are in the list
    while (Events.front ()->equals (e))
    {
        event *evt = Events.front ();
        evt->execute (e);
        
        // remove event
        Events.erase (Events.begin ());
        
        // re-register event if it needs be repeated
        if (evt->repeat ()) register_event (evt);
    }
    
    return;
}

// Unregister an event
void time_event_handler::remove_event (event *e)
{
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (Events.begin (), Events.end (), e);

    // found? -> get rid of it :)
    if (i != Events.end ()) Events.erase (i);
}

// register an event with the handler
void time_event_handler::register_event (event *e)
{
    vector<event*>::iterator i = Events.begin ();

    // search for the proper place to insert new event
    while (i != Events.end ())
    {
        // skip events that are raised earlier than e
        if (((time_event *) e)->time () <= ((time_event *) (*i))->time ()) break;
        i++;
    }
    
    Events.insert (i, e);
}
