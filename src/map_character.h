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

#ifndef MAP_CHARACTER_H
#define MAP_CHARACTER_H

#include "map_moving.h"

class map_character : public map_moving
{
protected:
    float Speed; 

public:
    
    map_character () 
    {
        Type = CHARACTER;
        Speed = 2;
    }

    float speed () 
    {
        return Speed; 
    }

    void set_speed (float s) 
    {
        Speed = s; 
    }

    void update_state () 
    {
        float xvel = vx () > 0 ? vx () : -vx (); 
        float yvel = vy () > 0 ? vy () : -vy ();

        if (xvel > yvel) 
        {
            if (vx () > 0) set_state ("standeast");
            else if (vx () < 0) set_state ("standwest"); 
        }
        else if (yvel > xvel) 
        {
            if (vy () > 0) set_state ("standsouth");
            else if (vy () < 0) set_state ("standnorth"); 
        }
    }
};

#endif
