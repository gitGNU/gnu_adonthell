/*
   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   event_handler.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the event_handler class
 * 
 */

#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include "event_handler_base.h"
#include "event_list.h"

/**
 * It ensures global access to the individual %event handlers.
 */
class event_handler
{
public:
    /**
     * Instanciate the actual event handlers. Event handlers
     * can be specific to a certain event, or take care of
     * different events.
     */
    static void init ();

    /**
     * Delete the %event handlers.
     */
    static void cleanup ();

    /** 
     * Unregister an %event.
     * 
     * @param ev pointer to the %event to unregister.
     */
    static void remove_event (event* ev)
    {
        ev->set_registered (false);
        Handler[ev->type ()]->remove_event (ev);
    }

    /** 
     * Check if an %event corresponding to ev exists, and execute it. 
     * 
     * @param ev %event to raise.
     */
    static void raise_event (const event* ev)
    {
        Handler[ev->type ()]->raise_event (ev);
    }

protected:
    /** 
     * Registers an %event.
     * 
     * @param ev pointer to the %event to register.
     */
    static void register_event (event* ev)
    {
        ev->set_registered (true);
        Handler[ev->type ()]->register_event (ev);
    }

    /**
     * Only %event_list is allowed to register events with the
     * %event_handler.
     */
    friend void event_list::add_event (event* ev);

    /**
     * As is event::resume.
     */
    friend void event::resume ();
    
private:
    /**
     * A list of the actual %event handlers
     */
    static event_handler_base* Handler[MAX_EVENTS];
};

#endif // EVENT_HANDLER_H__
