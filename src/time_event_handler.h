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
 * @file time_event_handler.h
 *
 * @author Kai Sterker
 * @brief Declares the time_event_handler class.
 */

#ifndef TIME_EVENT_HANDLER_H__
#define TIME_EVENT_HANDLER_H__

#include <vector>
#include "event.h"

using std::vector;

/**
 * This class keeps track of time events, i.e. events that are raised
 * at a certain point in (%game) time. All registered events are 
 * sorted by the time they need to be raised, so that only one
 * comparison decides upon whether an %event is to be raised.
 */
class time_event_handler
{
public:
    /**
     * Register a time %event with the %event handler. It is inserted
     * into the vector of registered events depending on its "alarm"
     * time. The %event needs to be removed before it can be safely
     * deleted.
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
     * Raise one or more events in case the given time matches their
     * "alarm" time. When they need to be repeated, they are
     * re-inserted into the %event-vector.
     *
     * @param evnt An %event structure with the current %game time in 
     *      minutes.
     */
    void raise_event (const event &evnt);
    
private:
    // storage for registered time events.
    vector<event*> Events;
};

#endif // TIME_EVENT_HANDLER_H__
