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
    // we have to iterate back to front as executing an event might
    // erase it from the vector. This invalidates any iterators pointing
    // _after_ the deleted element.
    for (vector<event*>::iterator i = Events.end (); i > Events.begin ();)
    {
        i--;
        
        // if the events match, execute them. Note that events that use up
        // their repeat count are deleted (and automatically unregistered).
        if ((*i)->equals (e)) 
            if (!(*i)->execute (e))
                delete *i;
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
