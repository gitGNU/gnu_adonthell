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
 * @file   mapsquares.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapsquare_walkable class.
 * 
 * 
 */


#ifndef MAPSQUARE_H_
#define MAPSQUARE_H_ 

#include "fileops.h"
#include <vector>


/**
 * Size of a mapsquare (in pixels).
 * 
 */ 
const u_int16 MAPSQUARE_SIZE = 20; 

/** 
 * Walkable from every side.
 * 
 */
#define ALL_WALKABLE 15

/**
 * Walkable from South.
 * 
 */ 
#define WALKABLE_SOUTH 1

/**
 * Walkable from North.
 * 
 */
#define WALKABLE_NORTH 2

/**
 * Walkable from East.
 * 
 */
#define WALKABLE_EAST 4

/**
 * Walkable from West.
 * 
 */
#define WALKABLE_WEST 8

/**
 * Unreachable.
 * 
 */
#define NONE_WALKABLE 0



/** 
 * Contains information about the walkability of a mapsquare.
 * 
 */
class mapsquare_walkable
{
public:

    /** 
     * Default constructor.
     * 
     */
    mapsquare_walkable();

    /** 
     * Loads a mapsquare_walkable from an opened file.
     * 
     * @param file the file to load from.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 get (igzstream& file);

    /**
     * Puts a mapsquare_walkable into an opened file.
     * 
     * @param file the file where to save.
     * 
     * @return 0 in case of success, error code otherwise.
     */ 
    s_int8 put (ogzstream& file);

    /** 
     * Returns whether a mapsquare is walkable from west.
     * 
     * 
     * @return true if the mapsquare is walkable from west, false otherwise.
     */
    bool is_walkable_west ()
    {
        return walkable & WALKABLE_WEST;
    }

    /** 
     * Returns whether a mapsquare is walkable from east.
     * 
     * 
     * @return true if the mapsquare is walkable from east, false otherwise.
     */
    bool is_walkable_east ()
    {
        return walkable & WALKABLE_EAST;
    }
    
    /** 
     * Returns whether a mapsquare is walkable from north.
     * 
     * 
     * @return true if the mapsquare is walkable from north, false otherwise.
     */
    bool is_walkable_north ()
    {
        return walkable & WALKABLE_NORTH;
    }
    
    /** 
     * Returns whether a mapsquare is walkable from south.
     * 
     * 
     * @return true if the mapsquare is walkable from south, false otherwise.
     */
    bool is_walkable_south ()
    {
        return walkable & WALKABLE_SOUTH;
    }

    /** 
     * Sets the reachability from west of a mapsquare.
     * 
     * @param w true if the mapsquare should be reachable from west, false otherwise.
     */
    void set_walkable_west (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_WEST);
        else
            walkable |= WALKABLE_WEST;
    }
    
    /** 
     * Sets the reachability from east of a mapsquare.
     * 
     * @param w true if the mapsquare should be reachable from east, false otherwise.
     */
    void set_walkable_east (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_EAST);
        else
            walkable |= WALKABLE_EAST;
    }
    
    /** 
     * Sets the reachability from north of a mapsquare.
     * 
     * @param w true if the mapsquare should be reachable from north, false otherwise.
     */
    void set_walkable_north (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_NORTH);
        else
            walkable |= WALKABLE_NORTH;
    }
    
    /** 
     * Sets the reachability from south of a mapsquare.
     * 
     * @param w true if the mapsquare should be reachable from south, false otherwise.
     */
    void set_walkable_south (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_SOUTH);
        else
            walkable |= WALKABLE_SOUTH;
    }

    /**
     * Gets the raw walkable parameter of a mapsquare.
     *
     * @return walkable parameter of this mapsquare.
     */ 
    u_int8 get_walkable () 
    {
        return walkable; 
    }

    /** 
     * Sets the walkable parameter of a mapsquare.
     * 
     * @param w new walkable status.
     */
    void set_walkable (u_int8 w) 
    {
        walkable = w; 
    }
    
private:
    u_int8 walkable; 
}; 


#endif
