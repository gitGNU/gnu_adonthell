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
#include "mapsquare.h"

#ifndef SWIG
using namespace std; 
#endif

class landmap; 

/**
 * A* pathfinding algorithm implementation class.
 *
 * This class calculates the shortest way from a begin point
 * to a goal point on a landmap using the A* algorithm. It
 * stores a list of directions that when followed lead from
 * the start to the goal.
 *
 * This class is particularly well designed for mapcharacters,
 * who will often need to walk from one point to another.
 * 
 */ 
class path
{
private:
    struct compare_squarecost
    {
        bool operator() (const mapsquare * s1, const mapsquare * s2)
        {
            return (s1->f > s2->f); 
        } 
    }; 
    
public:

    /** 
     * (x, y) coordinates of a point on a submap.
     * 
     */ 
    struct area_coord
    {
        /**
         * X position.
         * 
         */ 
        u_int16 x;

        /**
         * Y position.
         * 
         */ 
        u_int16 y; 
    }; 

    /**
     * Landmap where the pathfinding will occur.
     * 
     */ 
    landmap * refmap; 

    /**
     * Submap where the pathfinding will occur.
     * 
     */ 
    u_int16 submap;

    /**
     * Direction to face once the goal is reached.
     * 
     */ 
    u_int16 dir;

    /** 
     * Start point.
     *  
     */ 
    area_coord start;

    /**
     * Goal point.
     * 
     */ 
    area_coord goal;
  
    /** 
     * Tries to find the shortest path possible between the
     * \e start point and the \e goal point. 
     * 
     * @return \e true if a path was found, \e false otherwise.
     */
    bool calculate (); 

    /** 
     * Returns the number of moves between \e start and \e goal.
     * 
     * 
     * @return Number of moves between \e start and \e goal.
     */
    u_int16 nbr_moves () 
    {
        return moves_to_goal.size ();
    }

    /** 
     * Returns the move to perform when at position \e nbr.
     * 
     * @param nbr Index of the move to get.
     * 
     * @return Direction (move) at index \e nbr.
     */
    u_int16 get_move (u_int16 nbr) 
    {
        return moves_to_goal[nbr_moves () - (nbr + 1)]; 
    }
    
private: 
    vector <u_int16> moves_to_goal;
    u_int16 goal_estimate (u_int16 x, u_int16 y); 
}; 

#endif
