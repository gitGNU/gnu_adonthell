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

#include "control_event.h"

string control_event::Button_symbol[control_event::NBR_BUTTONS] =
{
    "no button"
    "up",
    "down",
    "right",
    "left",
    "A button",
    "B button",
    "C button",
    "D button"
};

control_event::control_event(event_type t, button_type b) : input_event(CONTROL_EVENT)
{
    Type = t;
    Button = b;
}

const string & control_event::button_symbol() const
{
    return Button_symbol[button()];
}

control_event::button_type control_event::Keyboard_map[keyboard_event::NBR_KEYS] = 
    { control_event::NO_BUTTON };
control_event::button_type control_event::Mouse_map[mouse_event::NBR_BUTTONS] = 
    { control_event::NO_BUTTON };
control_event::button_type control_event::Joystick_map[joystick_event::NBR_BUTTONS * 
                                                    joystick_event::max_nbr_joysticks] = 
    { control_event::NO_BUTTON };
