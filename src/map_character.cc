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


#include "map_character.h"

void map_character::update_state()
{
    string state;
    float xvel = vx () > 0 ? vx () : -vx (); 
    float yvel = vy () > 0 ? vy () : -vy ();
    
    if (xvel || yvel) 
    {
        if (xvel > yvel) 
        {
            if (vx () > 0) state = "e";
            else if (vx () < 0) state = "w"; 
        }
        else if (yvel > xvel) 
        {
            if (vy () > 0) state = "s";
            else if (vy () < 0) state = "n"; 
        }
        else 
        {
            if ((vx() > 0) && (current_state_name()[0] == 'w'))
                state = 'e';
            else if ((vx() < 0) && (current_state_name()[0] == 'e'))
                state = 'w';
            else if ((vy() > 0) && (current_state_name()[0] == 'n'))
                state = 's';
            else if ((vy() < 0) && (current_state_name()[0] == 's'))
                state = 'n';
            else state = current_state_name()[0];
        }
        state += is_running() ? "_run" : "_walk";
    }
    else if (!xvel)
    {
        state = current_state_name()[0];
        state += "_stand";
    }
    set_state(state);

}
