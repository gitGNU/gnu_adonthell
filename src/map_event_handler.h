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
 * @file map_event_handler.h
 *
 * @author Kai Sterker
 * @brief Declares the map_event_handler class.
 */

#ifndef MAP_EVENT_HANDLER_H__
#define MAP_EVENT_HANDLER_H__

#include <vector>
#include "event_handler_base.h"

using std::vector;

/**
 * This class keeps track of map events, i.e. events that are raised
 * when a character enters or leaves a tile, or initiates an action.
 */
class map_event_handler : public event_handler_base
{
public:
    /**
     * Register a map %event with the %event handler. The %event 
     * needs to be removed before it can be safely deleted.
     *
     * @param evnt Pointer to the %event to be registered.
     */
    void register_event (event *evnt);
    
    /**
     * Removes the given %event from the %event handler. Once it is
     * no longer needed, it can be freed.
     * 
     * @param evnt Pointer to the %event to be removed.
     */
    void remove_event (event *evnt);
    
    /**
     * Raise one or more events in case the given 'trigger' matches.
     * Unless they need to be repeated, they are removed from the 
     * %event-vector.
     *
     * @param evnt A map_event structure.
     */
    void raise_event (const event &evnt);
    
private:
    // storage for registered time events.
    vector<event*> Events;
};

#endif // MAP_EVENT_HANDLER_H__
