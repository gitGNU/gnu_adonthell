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
 * @file   mapsquare.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapsquare and mapsquare_area classes.
 * 
 * 
 */


#ifndef MAPSQUARE_H_
#define MAPSQUARE_H_ 


#include "fileops.h"
#include "mapsquare_walkable.h"

#include <string>
#include <vector>
#include <list>


class mapobject;
class mapcharacter;

/**
 * Contains informations about the position of an object on a map. 
 *
 * Objects of this class has no reason to exist outside of a mapsquare.
 * You'll NEVER want to manipulate this class directly - only mapsquare,
 * mapsquare_area and landmap will.
 */
class mapsquare_tile
{
public:
    /**
     * Default constructor.
     */ 
    mapsquare_tile ();
    
    /**
     * Destructor.
     */
    ~mapsquare_tile ();
     
#ifndef SWIG     
    /**
     * Compare the location on the landsubmap of two mapsquare_tiles.
     * A mapsquare_tile is < to another if it's Y position is < to the other
     * one's or if it's Y position == the other one's and it's X position is
     * < to the other one's.
     *
     * @attention Not available from Python.
     *
     * @sa operator <=  ()
     * @sa operator ==  () 
     */
    bool operator < (const mapsquare_tile & mt)
    {
        return (mt.y > y || (mt.y == y && mt.x > x));
    }

    /**
     * Compare the location on the landsubmap of two mapsquare_tiles.
     * A mapsquare_tile is <= to another if it's Y position is < to the other
     * one's or if it's Y position == the other one's and it's X position is
     * <=  to the other one's.
     *
     * @attention Not available from Python.
     *
     * @sa operator < ()
     * @sa operator == () 
     */
    bool operator <= (const mapsquare_tile & mt)
    {
        return (mt.y > y || (mt.y == y && mt.x >= x));
    }
 
    /**
     * Compare the location on the landsubmap of two mapsquare_tiles.
     * A mapsquare_tile is ==  to another if their X and Y position are
     * equal.
     *
     * @attention Not available from Python.
     *
     * @sa operator < ()
     * @sa operator <= () 
     */
    bool operator == (const mapsquare_tile & mt)
    {
        return (mt.y == y && mt.x == x);
    } 
#endif
    

private:     
    /// Pointer to the object here.
    mapobject * mapobj; 

    /// Is this mapsquare_tile a base square?
    bool is_base;

    /// Iterator to the base tile of this mapsquare_tile.
    list <mapsquare_tile>::iterator base_tile;

    /// x and y positions.
    u_int16 x, y;
 
    friend class mapsquare;
    friend class mapsquare_area; 
    friend class landmap;
    friend class mapview;
};


/**
 * Contains informations about the position of a character on a map.  
 *
 * Objects of this class has no reason to exist outside of a mapsquare.
 * You'll NEVER want to manipulate this class directly - only mapsquare,
 * mapsquare_area and landmap will.
 */
class mapsquare_char
{ 
public:
    /**
     * Default constructor.
     */ 
    mapsquare_char ();

    /**
     * Destructor.
     */ 
    ~mapsquare_char ();
    
    /**
     * Compare the location on the landsubmap of two mapsquare_chars.
     * A mapsquare_char is < to another if it's Y position is < to the other
     * one's or if it's Y position == the other one's and it's X position is
     * < to the other one's.
     * @sa operator <=  ()
     * @sa operator ==  () 
     */
    bool operator < (const mapsquare_char & mt)
    {
        return (mt.y > y || (mt.y == y && mt.x > x));
    }
    
    /**
     * Compare the location on the landsubmap of two mapsquare_chars.
     * A mapsquare_char is <= to another if it's Y position is < to the other
     * one's or if it's Y position == the other one's and it's X position is
     * <=  to the other one's.
     * @sa operator < ()
     * @sa operator == () 
     */
    bool operator <= (const mapsquare_char & mt)
    {
        return (mt.y > y || (mt.y == y && mt.x >= x));
    }
    
    /**
     * Compare the location on the landsubmap of two mapsquare_chars.
     * A mapsquare_char is ==  to another if their X and Y position are
     * equal.
     * @sa operator < ()
     * @sa operator <= () 
     */
    bool operator == (const mapsquare_char & mt)
    {
        return (mt.y == y && mt.x == x);
    }
    
private:
    /// Pointer to the mapcharacter concerned by this mapchar_square.
    mapcharacter *mchar;

    /// Is it the base tile?
    bool is_base;

    /// Is this mapsquare_tile walkable?
    bool walkable;

    /// Iterator to the base tile of this mapsquare_char.
    list <mapsquare_char>::iterator base_tile;

    /// x and y positions.
    u_int16 x, y;
    
    friend class mapcharacter; 
    friend class mapsquare;
    friend class landmap;
    friend class mapview;
};


/**
 * Base unit of a landsubmap, where you can place mapobjects or mapcharacters.
 * A landsubmap is a 2 dimensionnal array of mapsquares. When a mapobject is
 * placed on a landsubmap, it belongs to one or several mapsquares. A mapsquare
 * is made of a list of mapsquare_tiles, containing informations about the objects
 * that are on it, and a list of mapsquare_char, which informs about the mapcharacters
 * here. This make it possible to have several mapobjects
 * and mapcharacters on the same mapsquare.
 *
 * These two lists are sorted by the position of the object or mapcharacter's base square
 * on the map. This make it fast to iterate through the lists during drawing, as we always
 * want to iterate the list in this order.
 */ 
class mapsquare : public mapsquare_walkable
{
public:
    /** 
     * Default constructor.
     * 
     */
    mapsquare ();
    
    /**
     * Copy constructor.
     * 
     */ 
    mapsquare (const mapsquare& src);
    
    /** 
     * Destructor.
     * 
     */
    ~mapsquare ();
    
    /** 
     * Returns whether the mapsquare is free for a character to go on or not.
     * It only checks if a mapcharacter is already here. It doesn't deal with the
     * walkable problem.
     * @return
     *         - false if the mapsquare isn't free.
     *         - true if the mapsquare is free.
     */ 
    bool is_free (); 

    /** 
     * Return a pointer to the mapcharacter that occupies this mapsquare.
     *
     * @return pointer to the mapcharacter that occupies the mapsquare, NULL if none.
     */
    mapcharacter *whoshere (); 

private: 
#ifndef SWIG
    /// List of mapsquare_tiles.
    list <mapsquare_tile> tiles;
    
    /// Iterator to where the base tiles begin.
    /// This serves as an "accelerator" for mapview::draw () which
    /// can go directly to the base tiles of a squares with this
    /// iterator.
    list <mapsquare_tile>::iterator base_begin;
    
    /// List of mapsquare_chars.
    list <mapsquare_char> mapchars;
#endif

    friend class mapcharacter; 
    friend class mapsquare_area; 
    friend class landmap;
    friend class mapview;
};	  // mapsquare 

/** 
 * Area of mapsquares, for use with landmap.
 *
 * This class has no reason to exist is not belonging
 * to a landmap. You'll NEVER use this class directly -
 * anyway you can't do anything usefull with it alone.
 * 
 */
class mapsquare_area
{
public:
    /** 
     * Default constructor.
     * 
     */
    mapsquare_area ();
    
    /** 
     * Destructor.
     * 
     */
    ~mapsquare_area (); 

    /** 
     * Totally clears the area.
     * 
     */
    void clear (); 
    
    /**
     * @name Area settings
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
    mapsquare * get_square (u_int16 x, u_int16 y) const
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

    
private:
    /**
     * Forbids value passing.
     * 
     */
    mapsquare_area (const mapsquare_area& src); 
    
    /**
     * Forbids mapsquare_area copy.
     * 
     */ 
    mapsquare_area & operator = (const mapsquare_area & mo);

    /** 
     * Place a mapobject on the submap.
     * 
     * @param px X position of the base square of the object.
     * @param py Y position of the base square of the object.
     * @param mobj pointer to the mapobject to remove.
     */ 
     s_int8 put_mapobject (u_int16 px, u_int16 py, mapobject * mobj);

    /** 
     * Remove a mapobject from the submap.
     * 
     * @param px X position of the base square of the object.
     * @param py Y position of the base square of the object.
     * @param mobj pointer to the mapobject to remove.
     */
    void remove_mapobject (u_int16 px, u_int16 py, mapobject * mobj);

    /**
     * 2 dimensionnal array of mapsquares - the actual map
     * 
     */ 
    mutable vector <vector<mapsquare> > area;
    
    friend class mapcharacter; 
    friend class mapview; 
    friend class landmap; 
}; 

#endif
