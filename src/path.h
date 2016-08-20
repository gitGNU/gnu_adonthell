/*
   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
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
#ifndef SWIG
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
#endif // SWIG

    /** 
     * Totally clears the path.
     * 
     */
    void clear () 
    {
        moves_to_goal.clear (); 
    } 

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
    u_int16 nbr_moves () const
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
    u_int16 get_move (u_int16 nbr) const
    {
        return moves_to_goal[nbr_moves () - (nbr + 1)]; 
    }

    /** 
     * Restore the path's state from an opened file.
     * 
     * @param file the opened file from which to load the state.
     * 
     * @return 0 in case of success, error code otherwise.
     *
     * @bug the landmap this path belongs to must be restored manually!
     */
    s_int8 get_state (igzstream & file); 

    /** 
     * Saves the path's state into an opened file.
     * 
     * @param file the opened file where to the state.
     * 
     * @return 0 in case of success, error code otherwise.
     *
     * @bug the landmap this path belongs to can't be saved (as it's a pointer)! 
     */
    s_int8 put_state (ogzstream & file) const; 
  
private: 
    vector <u_int16> moves_to_goal;
    u_int16 goal_estimate (u_int16 x, u_int16 y); 
}; 

#endif
