/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   input_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the input_event base class.
 * 
 * 
 */


#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "types.h"

/**
 * Base class for input events classes.
 * 
 */
class input_event
{
public:

    /**
     * Type of the input event.
     * 
     */
    typedef enum
    {
        KEYBOARD_EVENT = 0,
        MOUSE_EVENT,
        JOYSTICK_EVENT,
        CONTROL_EVENT, 
        NBR_INPUT_TYPES
    } input_type;
    
    /** 
     * Constructor
     * 
     * @param t input event type.
     */
    input_event (input_type t)
    {
        Type = t; 
    }


    /** 
     * Returns the type of this event.
     * 
     * 
     * @return type of this event.
     */
    input_type type () 
    {
        return Type; 
    }

private:
    input_type Type;
    
}; 

#endif
