/*
   $Id:

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


class input_event
{
public:
    typedef enum
    {
        KEYBOARD_EVENT,
        MOUSE_EVENT,
        JOYSTICK_EVENT,
        CONTROL_EVENT, 
        NBR_INPUT_TYPES
    } input_type;
    
    input_event (input_type t)
    {
        Type = t; 
    }

    input_type type () 
    {
        return Type; 
    }

private:
    input_type Type;
    
}; 

#endif
