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

#include "mouse_event.h"

string mouse_event::Button_symbol[mouse_event::NBR_BUTTONS] =
{
    "no button",
    "left button",
    "middle button",
    "right_button",
    "wheel up",
    "wheel down"
};

const string & mouse_event::button_symbol() const
{
    return Button_symbol[button()];
}
