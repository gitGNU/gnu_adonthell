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

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "input_event.h"
#include <string>

class mouse_event : public input_event
{
public:
    typedef enum
    {
        BUTTON_PUSHED, BUTTON_RELEASED, MOUSE_MOTION
    }event_type;

    typedef enum
    {
        NO_BUTTON = 0, LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON, 
        WHEEL_UP, WHEEL_DOWN, NBR_BUTTONS
    }button_type;

    mouse_event (event_type t, button_type b, u_int16 xpos, u_int16 ypos)
        : input_event (MOUSE_EVENT)
    {
        Type = t;
        Button = b;
        X = xpos;
        Y = ypos;
    }

    event_type type() const
    {
        return Type;
    }

    button_type button() const
    {
        return Button;
    }

    u_int16 x() const
    {
        return X;
    }

    u_int16 y() const
    {
        return Y;
    }

    const string & button_symbol() const;

private:
    static string Button_symbol[NBR_BUTTONS];
    event_type Type;
    u_int16 X, Y;
    button_type Button;
};

#endif
