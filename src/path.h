/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   path.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the path class.
 * 
 * 
 */

#ifndef PATH_H__
#define PATH_H__

#include "types.h"
#include <vector>

#ifndef SWIG
using namespace std; 
#endif

class landmap; 

struct area_coord
{
    u_int16 x;
    u_int16 y; 
}; 

class path
{
public:
    landmap * refmap; 
    u_int16 submap;
    u_int16 dir;
    area_coord start;
    area_coord goal;
  
    bool calculate (); 

    u_int16 nbr_moves () 
    {
        return moves_to_goal.size ();
    }

    u_int16 get_move (u_int16 nbr) 
    {
        return moves_to_goal[nbr_moves () - (nbr + 1)]; 
    }
    
private: 
    vector <u_int16> moves_to_goal;
    u_int16 goal_estimate (u_int16 x, u_int16 y); 
}; 

#endif
