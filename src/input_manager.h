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


#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "input_listener.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "joystick_event.h"
#include <list> 

class input_manager
{
public:
    static void update();
    static void raise_event(input_event & ev);

    static void add(input_listener * il)
    {
        listeners.push_back(il);
    }

    static void remove(input_listener * il)
    {
        listeners.remove(il);
    }

private:
    static list <input_listener *> listeners;

}; 

#endif
