/*
   Copyright (C) 2000/2001/2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   event_handler.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Implements the event_handler class.
 * 
 */
 
#include "event_handler.h"
#include "map_event.h"
#include "map_event_handler.h"
#include "time_event.h"
#include "time_event_handler.h"

// Array with registered event handlers
event_handler_base* event_handler::Handler[MAX_EVENTS];

// functions that return newly instanciated events
// of a certain type
NEW_EVENT (time_event)
NEW_EVENT (enter_event)
NEW_EVENT (leave_event)
NEW_EVENT (action_event)

// Initialize the game event system
void event_handler::init ()
{
    // register event handlers
    Handler[ENTER_EVENT] = new map_event_handler;
    Handler[LEAVE_EVENT] = new map_event_handler;
    Handler[ACTION_EVENT] = new map_event_handler;
    Handler[TIME_EVENT] = new time_event_handler;

    // register events
    REGISTER_EVENT (TIME_EVENT, time_event)
    REGISTER_EVENT (ENTER_EVENT, enter_event)
    REGISTER_EVENT (LEAVE_EVENT, leave_event) 
    REGISTER_EVENT (ACTION_EVENT, action_event) 
}

// Clear the registered handlers
void event_handler::cleanup ()
{
    for (int i = 0; i < MAX_EVENTS; i++)
        if (Handler[i] != NULL)
            delete Handler[i];
}
