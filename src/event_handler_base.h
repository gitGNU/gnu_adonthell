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
 * @file event_handler_base.h
 *
 * @author Kai Sterker
 * @brief Base class for %event handlers.
 */

#ifndef EVENT_HANDLER_BASE_H__
#define EVENT_HANDLER_BASE_H__

#include "event.h"

/**
 * Docu follows soon ...
 */
class event_handler_base
{
public:
    /**
     */
    virtual void register_event (event *evnt) = 0;
    
    /**
     */
    virtual void remove_event (event *evnt) = 0;
    
    /**
     */
    virtual void raise_event (const event &evnt) = 0;
};

#endif // EVENT_HANDLER_BASE_H__
