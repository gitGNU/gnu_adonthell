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

#ifndef MAP_CHARACTER_H
#define MAP_CHARACTER_H

#include "map_moving.h"

class map_character : public map_moving
{
public:
    typedef enum { NONE = 0, WEST = 1, EAST = 2, NORTH = 4, SOUTH = 8 } direction;

protected:
    float Speed; 
    bool Is_running;
    int Current_dir;

public:
    
    map_character () 
    {
        Type = CHARACTER;
        Speed = 2;
        Is_running = false;
        Current_dir = NONE;
    }

    float speed () 
    {
        return Speed; 
    }

    void set_speed (float s) 
    {
        Speed = s; 
    }

    bool is_running()
    {
        return Is_running;
    }

    void walk()
    {
        Is_running = false;
        set_direction(current_dir());
    }

    void run()
    {
        Is_running = true;
        set_direction(current_dir());
    }

    void stop()
    {
        set_velocity(0.0, 0.0);
    }

    int current_dir()
    {
        return Current_dir;
    }

    void set_direction(int ndir);

    void add_direction(direction ndir);

    void remove_direction(direction ndir)
    {
        set_direction(current_dir() & ~ndir);
    }

    void update_state ();
};

#endif
