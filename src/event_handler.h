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
 * @brief  Declares the event_handler class.
 * 
 */

#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <vector>
#include "event.h"

/**
 * Keeps track of registered scripts, recieves triggered events 
 * and executes scripts handling those events
 */ 
class event_handler
{
public:
    /** 
     * Registers an event.
     * 
     * @param ev pointer to the event to register.
     */
    static void register_event (event* ev);

    /** 
     * Unregister an event.
     * 
     * @param event* pointer to the event to unregister.
     */
    static void remove_event (event* ev);

    /** 
     * Check if an event corresponding to ev exists, and execute it. 
     * 
     * @param ev event to raise.
     */
    static void raise_event (const event& ev);
    
private:
#ifndef SWIG
    // registered events storage
    static vector<event*> handlers[MAX_EVENT];
#endif
};

#endif // EVENT_HANDLER_H__
