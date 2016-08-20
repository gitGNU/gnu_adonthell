/*
   Copyright (C) 2002/2003 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file time_event_handler.cc
 *
 * @author Kai Sterker
 * @brief Implements the time_event_handler class.
 */

#include <algorithm>
#include "gamedate.h"
#include "time_event.h"
#include "time_event_handler.h"


// See whether a matching event is registered and execute the
// according script(s) 
void time_event_handler::raise_event (const event * e)
{
    s_int32 repeat;
    event *evt;

    // As long as matching events are in the list
    while (!Events.empty () && Events.front ()->equals (e))
    {
        evt = Events.front ();

        // we remove the event in any case, as it needs to be
        // re-registered at a new position if it repeats
        Events.erase (Events.begin ());
        evt->set_registered (false);

        // events that don't repeat are destroyed automatically
        repeat = evt->execute (e);

        // re-register event if it needs be repeated
        if (repeat) register_event (evt);
        else delete evt;
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
