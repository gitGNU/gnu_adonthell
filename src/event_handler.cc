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
#include "event_list.h"
#include "map_event.h"
#include "map_event_handler.h"
#include "time_event.h"
#include "time_event_handler.h"

// Array with registered event handlers
event_handler_base* event_handler::Handler[MAX_EVENTS];

// functions that return newly instanciated events
// of a certain type
NEW_EVENT (time_event)
NEW_EVENT (enter_event); 
NEW_EVENT (leave_event); 
NEW_EVENT (action_event); 

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
    for (int i; i < MAX_EVENTS; i++)
        if (Handler[i] != NULL)
            delete Handler[i];
}
