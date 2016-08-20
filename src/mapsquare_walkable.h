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
 * @file   mapsquare_walkable.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapsquare_walkable and mapsquare_walkable_area classes.
 * 
 * 
 */


#ifndef MAPSQUARE_WALKABLE_H_
#define MAPSQUARE_WALKABLE_H_ 

#include "fileops.h"
#include "drawable.h"
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
    s_int8 put (ogzstream& file) const;

    /** 
     * Returns whether a mapsquare is walkable from west.
     * 
     * 
     * @return true if the mapsquare is walkable from west, false otherwise.
     */
    bool is_walkable_west () const
    {
        return walkable & WALKABLE_WEST;
    }

    /** 
     * Returns whether a mapsquare is walkable from east.
     * 
     * 
     * @return true if the mapsquare is walkable from east, false otherwise.
     */
    bool is_walkable_east () const
    {
        return walkable & WALKABLE_EAST;
    }
    
    /** 
     * Returns whether a mapsquare is walkable from north.
     * 
     * 
     * @return true if the mapsquare is walkable from north, false otherwise.
     */
    bool is_walkable_north () const
    {
        return walkable & WALKABLE_NORTH;
    }
    
    /** 
     * Returns whether a mapsquare is walkable from south.
     * 
     * 
     * @return true if the mapsquare is walkable from south, false otherwise.
     */
    bool is_walkable_south () const
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
    u_int8 get_walkable () const
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


/** 
 * Area of mapsquare_walkables, for use with mapcharacter and mapobject classes.
 * 
 */
class mapsquare_walkable_area : public drawable
{
public:
    /** 
     * Default constructor.
     * 
     */
    mapsquare_walkable_area ();
    
    /** 
     * Destructor.
     * 
     */
    ~mapsquare_walkable_area (); 

    /** 
     * Totally clears the area.
     * 
     */
    void clear (); 

    virtual void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                       surface * target = NULL) const = 0;

    /**
     * @name Area settings.
     * 
     */ 
    //@{ 

    /**
     * Returns the length of the area.
     *
     * @return length (in number of squares) of the area.
     *
     */ 
    u_int16 area_length () const
    {
        return area.size (); 
    }

    /**
     * Returns the height of the area.
     *
     * @return height (in number of squares) of the area.
     *
     */ 
    u_int16 area_height () const
    {
        if (area.size ()) return area[0].size (); 
        else return 0; 
    }

    /** 
     * Returns a pointer to a desired square.
     * 
     * @param x X position of the square to get.
     * @param y Y position of the square to get.
     * 
     * @return pointer to the (x,y) square.
     */
    mapsquare_walkable * get_square (u_int16 x, u_int16 y) const
    {
        return &(area[x][y]); 
    }
    
    /** 
     * Resize the area.
     * 
     * @param nl new length (in number of squares) of the area.
     * @param nh new height (in number of squares) of the area.
     */
    void resize_area (u_int16 nl, u_int16 nh); 

    //@}
    
      
    /**
     * @name Base square settings.
     * 
     */ 
    //@{ 
    
    /** 
     * Returns the X offset of the base square of this object.
     * 
     * 
     * @return X offset of the base square.
     */
    u_int16 base_x () const 
    {
        return basex; 
    }

    /** 
     * Returns the Y offset of the base square of this object.
     * 
     * 
     * @return Y offset of the base square.
     */
    u_int16 base_y () const
    {
        return basey; 
    }

    /** 
     * Sets the base square of this object.
     * 
     * @param nx X offset of the new base square.
     * @param ny Y offset of the new base square.
     */
    void set_base (u_int16 nx, u_int16 ny);

    //@}

    /**
     * Loads an area from an opened file.
     * @param file the opened file from which to load.
     * @return 0 in case of success, error code otherwise.
     *
     */ 
    s_int8 get (igzstream & file);

    /**
     * Saves an area into an opened file.
     * @param file the opened file where to write.
     * @return 0 in case of success, error code otherwise.
     *
     */ 
    s_int8 put (ogzstream & file) const;

#ifndef SWIG
    /**
     * Area copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */ 
    mapsquare_walkable_area & operator = (const mapsquare_walkable_area & mo);
#endif
    
    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (const mapsquare_walkable_area& src) 
    {
        *this = src; 
    }

private:
    /**
     * Forbids value passing.
     * 
     */ 
    mapsquare_walkable_area (const mapsquare_walkable_area & src); 

    mutable vector <vector<mapsquare_walkable> > area;
    
    u_int16 basex;
    u_int16 basey;  
}; 

#endif
