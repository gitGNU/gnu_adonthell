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
 * @file   listener.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the listener class.
 * 
 * 
 */

#include "listener.h"

using namespace input;

listener::listener ()
{
    for (u_int16 i = 0; i < event::NBR_INPUT_TYPES; i++)
    {
        Listen_to[i] = false;
        Callback_set[i] = false;
    }
}

listener::~listener()
{
}

void listener::connect_function(event::input_type t, 
                                      Functor1wRet<event *, int> f)
{
    Callbacks[t] = f;
    Callback_set[t] = true;
    Listen_to[t] = true;
}

void listener::disconnect_function(event::input_type t)
{
    Callback_set[t] = false;
    Listen_to[t] = false;
}

int listener::raise_event (event * ev)
{
    if (is_listening_to(ev->type()) && Callback_set[ev->type()]) 
        return Callbacks[ev->type()](ev);
    return 0; 
}
