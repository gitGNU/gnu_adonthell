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
 * @file   character.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the character class.
 * 
 * 
 */

#ifndef MAP_CHARACTER_H
#define MAP_CHARACTER_H

#include "moving.h"

namespace map
{

    /**
     * Map representation of a character.
     *
     * This class implements all that you need to represent a character
     * on the map. It mostly defines some additionnal functions on moving
     * to set the correct state according to the character move, and limits 
     * it's speed.
     * 
     */
    class character : public moving
    {
    public:
        typedef enum { NONE = 0, WEST = 1, EAST = 2, NORTH = 4, SOUTH = 8 } direction;

    protected:
        float Speed; 
        float VSpeed;

        bool Is_running;
        int Current_dir;

    public:
    
        character (landmap & mymap);

        void update();

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

        void jump();

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

        s_int8 save(const std::string fname) const;
        s_int8 load(const std::string fname);
    };
}

#endif
