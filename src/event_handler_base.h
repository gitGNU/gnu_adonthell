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
 * @file   event_handler_base.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the base class for event handlers.
 * 
 */

#ifndef EVENT_HANDLER_BASE_H__
#define EVENT_HANDLER_BASE_H__

#include "event.h"

/**
 * This is the base class for actual event handlers. It
 * keeps track of registered scripts, recieves triggered events
 * and executes scripts handling those events
 */ 
class event_handler_base
{
public:

    /**
     * Destructor
     */
    virtual ~event_handler_base () {}

    /** 
     * Registers an event.
     * 
     * @param ev pointer to the event to register.
     */
    virtual void register_event (event* ev) = 0;

    /** 
     * Unregister an event.
     * 
     * @param ev pointer to the event to unregister.
     */
    virtual void remove_event (event* ev) = 0;

    /** 
     * Check if an event corresponding to ev exists, and execute it. 
     * 
     * @param ev event to raise.
     */
    virtual void raise_event (const event& ev) = 0;
};

#endif // EVENT_HANDLER_BASE_H__
