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
 * @file map_event_handler.cc
 *
 * @author Kai Sterker
 * @brief Implements the map_event_handler class.
 */

#include <algorithm>
#include "map_event.h"
#include "map_event_handler.h"


// See whether a matching event is registered and execute the
// according script(s) 
void map_event_handler::raise_event (const event* e)
{
    vector<event*>::iterator j;
    
    // As long as matching events are in the list
    for (vector<event*>::iterator i = Events.begin (); i != Events.end ();)
    {
        j = i;
        j++;
        
        if ((*i)->equals (e))
            // execute them. Note that events that use up their repeat
            // count are automatically deleted and unregistered.
            (*i)->execute (e);
    
        i = j;    
    }
    return;
}

// Unregister an event
void map_event_handler::remove_event (event *e)
{
    vector<event*>::iterator i;

    // Search for the event we want to remove
    i = find (Events.begin (), Events.end (), e);

    // found? -> get rid of it :)
    if (i != Events.end ()) Events.erase (i);
}

// register an event with the handler
void map_event_handler::register_event (event *e)
{
    Events.push_back (e);
}
