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

#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input_event.h"
#include "callback.h"
#include "types.h"

class input_listener
{
private:
    bool Listen_to[input_event::NBR_INPUT_TYPES];
    bool Callback_set[input_event::NBR_INPUT_TYPES];
    Functor1wRet<input_event *, int> Callbacks[input_event::NBR_INPUT_TYPES];
    
public:
    input_listener ()
    {
        for (u_int16 i = 0; i < input_event::NBR_INPUT_TYPES; i++)
        {
            Listen_to[i] = false;
            Callback_set[i] = false;
        }
    }

    ~input_listener ();    

    bool is_listening_to (input_event::input_type t) 
    {
        return Listen_to[t]; 
    }

    void connect_function(input_event::input_type t, Functor1wRet<input_event *, int> f)
    {
        Callbacks[t] = f;
        Callback_set[t] = true;
        Listen_to[t] = true;
    }

    void disconnect_function(input_event::input_type t)
    {
        Callback_set[t] = false;
        Listen_to[t] = false;
    }

    int raise_event (input_event * ev)
    {
        if (is_listening_to(ev->type()) && Callback_set[ev->type()]) 
            return Callbacks[ev->type()](ev);
        return 0; 
    }
};

#endif
