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
 * @file   event_handler.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the event_handler class
 * 
 */

#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include "event_handler_base.h"

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
     * Remove all events from all handlers
     */
    static void clear ();
    
    /** 
     * Registers an %event.
     * 
     * @param ev pointer to the %event to register.
     */
    static void register_event (event* ev)
    {
        Handler[ev->type ()]->register_event (ev);
    }

    /** 
     * Unregister an %event.
     * 
     * @param ev pointer to the %event to unregister.
     */
    static void remove_event (event* ev)
    {
        Handler[ev->type ()]->remove_event (ev);
    }

    /** 
     * Check if an %event corresponding to ev exists, and execute it. 
     * 
     * @param ev %event to raise.
     */
    static void raise_event (const event& ev)
    {
        Handler[ev.type ()]->raise_event (ev);
    }

private:
    /**
     * A list of the actual %event handlers
     */
    static event_handler_base* Handler[MAX_EVENTS];
};

#endif // EVENT_HANDLER_H__
