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
        Callback_set[i] = NO_CALLBACK;
    }
}

listener::~listener()
{
}

void listener::connect_function(event::input_type t, 
                                Functor1wRet<event *, int> f)
{
    Callbacks[t] = f;
    Callback_set[t] = C_CALLBACK;
    Listen_to[t] = true;
}

void listener::connect_py_function(int t,
                                   PyObject * f)
{
    Py_callbacks[t] = f;
    Callback_set[t] = PY_CALLBACK;
    Listen_to[t] = true;
}

void listener::disconnect_function(event::input_type t)
{
    Callback_set[t] = NO_CALLBACK;
    Listen_to[t] = false;
}

int listener::raise_event (event * ev)
{
    if (is_listening_to(ev->type()))
    {
        switch (Callback_set[ev->type()])
        {
            case C_CALLBACK:
                return Callbacks[ev->type()](ev);
            case PY_CALLBACK:
            {
                PyObject * args = PyTuple_New (1);
                PyObject * arg = python::pass_instance (ev, "input", "keyboard_event");
                PyTuple_SetItem (args, 0, arg);
                PyObject * val = PyObject_CallObject (Py_callbacks[ev->type()], args);
                Py_DECREF (args);
                Py_DECREF (val);
                return 0;
            }
            default:
                break;
        }   
    }                                                
    return 0; 
}
