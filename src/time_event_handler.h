/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file time_event_handler.h
 *
 * @author Kai Sterker
 * @brief Declares the time_event_handler class.
 */

#ifndef TIME_EVENT_HANDLER_H__
#define TIME_EVENT_HANDLER_H__

#include <vector>
#include "event_handler_base.h"

using std::vector;

/**
 * This class keeps track of time events, i.e. events that are raised
 * at a certain point in (%game) time. All registered events are 
 * sorted by the time they need to be raised, so that only one
 * comparison decides upon whether an %event is to be raised.
 */
class time_event_handler : public event_handler_base
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
    void raise_event (const event *evnt);
    
private:
    // storage for registered time events.
    vector<event*> Events;
};

#endif // TIME_EVENT_HANDLER_H__
