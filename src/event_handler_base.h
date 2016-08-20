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
 * @file   event_handler_base.h
 * @author Kai Sterker <kai.sterker@gmail.com>
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
     * Registers an %event.
     * 
     * @param ev pointer to the %event to register.
     */
    virtual void register_event (event* ev) = 0;

    /** 
     * Unregister an %event.
     * 
     * @param ev pointer to the %event to unregister.
     */
    virtual void remove_event (event* ev) = 0;

    /** 
     * Check if an %event corresponding to ev exists, and execute it. 
     * 
     * @param ev %event to raise.
     */
    virtual void raise_event (const event* ev) = 0;
};

#endif // EVENT_HANDLER_BASE_H__
