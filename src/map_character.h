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
 * @file   map_character.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the map_character class.
 * 
 * 
 */

#ifndef MAP_CHARACTER_H
#define MAP_CHARACTER_H

#include "map_moving.h"

/**
 * Map representation of a character.
 *
 * This class implements all that you need to represent a character
 * on the map. It mostly defines some additionnal functions on map_moving
 * to set the correct state according to the map_character move, and limits 
 * it's speed.
 * 
 */
class map_character : public map_moving
{
public:
    typedef enum { NONE = 0, WEST = 1, EAST = 2, NORTH = 4, SOUTH = 8 } direction;

protected:
    float Speed; 
    bool Is_running;
    int Current_dir;

public:
    
    map_character (landmap & mymap);

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

    void put(ogzstream & file) const;
    void get(igzstream & file);

    s_int8 save(string fname) const;
    s_int8 load(string fname);
};

#endif
