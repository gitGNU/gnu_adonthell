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

class input_listener
{
private:
    bool Listen_to[input_event::NBR_INPUT_TYPES];
    
public:
    input_listener ();
    ~input_listener ();

    bool is_listening_to (input_event::input_type t) 
    {
        return Listen_to[t]; 
    }

    void listen_to (input_event::input_type t, bool val = true) 
    {
        Listen_to[t] = val; 
    }

    int raise_event (input_event * ev)
    {
        return 1; 
    }
};

#endif
