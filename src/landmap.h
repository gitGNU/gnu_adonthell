/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/



/**
 * @file landmap.h
 * Declares mapsquare_tile, mapsquare_char, mapsquare, landsubmap and landmap
 * classes.
 */



#ifndef _LANDMAP_H
#define _LANDMAP_H

#include "fileops.h"
#include "mapobject.h"
#include "mapcharacter.h"
#ifndef _EDIT_
#include "event.h"
#endif
#include <string>
#include <vector>
#include <list>

/// Subdirectory where maps are saved.
#define MAPS_DIR "maps/"


#ifdef _EDIT_
/// Size of the mini object in mapedit
#define OBJSMPLSIZE 40
#endif // _EDIT_

class mapview;
class mapcharacter;
class mapobject;

#ifndef SWIG


/**
 * Contains informations about the position of an object on a map.  
 */
class mapsquare_tile
{ public:
#ifdef _DEBUG_
    static u_int16 a_d_diff;
#endif 
    /**
     * Default constructor.
     */ 
    mapsquare_tile ();

    /**
     * Destructor.
     */
    ~mapsquare_tile ();

    /**
     * Compare the location on the landsubmap of two mapsquare_tiles.
     * A mapsquare_tile is < to another if it's Y position is < to the other
     * one's or if it's Y position == the other one's and it's X position is
     * < to the other one's.
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
     * @sa operator < ()
     * @sa operator <= () 
     */
    bool operator == (const mapsquare_tile & mt)
    {
        return (mt.y == y && mt.x == x);
    }

    /** 
     * Draw the mapsquare_tile on a given mapview.
     * 
     * @param mv mapview where to draw to.
     * @param x X screen offset (typically the mapview::draw () x parameter). 
     * @param y Y screen offset (typically the mapview::draw () y parameter). 
     * @todo This is a mess! Clean this.
    */
    void draw (mapview * mv, u_int16 x, u_int16 y);

#ifdef _EDIT_
    void draw_border (mapview * mv);
    void draw_base_tile (mapview * mv);
#endif
    
private:
    /// Index of the landmap object on this mapsquare_tile.
    u_int16 objnbr;

    /// Is this mapsquare_tile a base square?
    bool is_base;

    /// Iterator to the base tile of this mapsquare_tile.
    list <mapsquare_tile>::iterator base_tile;

    /// x and y positions.
    u_int16 x, y;
 
    friend class mapsquare;
    friend class landsubmap;
    friend class landmap;
    friend class mapview;
};

/**
 * Contains informations about the position of an object on a map.  
 */
class mapsquare_char
{ 
public:
#ifdef _DEBUG_
    static u_int16 a_d_diff;
#endif
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
    

    /** 
     * Draw the mapsquare_char on a given mapview.
     * 
     * @param mv mapview where to draw to.
     * @param x X screen offset (typically the mapview::draw () x parameter). 
     * @param y Y screen offset (typically the mapview::draw () y parameter). 
     * @todo This is a mess! Clean this.
     */
    void draw (mapview * mv, u_int16 x, u_int16 y);
    
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
    
    friend class mapsquare;
    friend class landsubmap;
    friend class landmap;
    friend class mapview;
};

#endif

/**
 * Base unit of a landsubmap, where you can place mapobjects or mapcharacters.
 * A landsubmap is a 2 dimensionnal array of mapsquares. When a mapobject is
 * placed on a landsubmap, it belongs to one or several mapsquares. A mapsquare
 * is made of a list of mapobjects that are on it, and a list of mapcharacters
 * that are currently on it. This make it possible to have several mapobjects
 * and mapcharacters on the same mapsquare.
 */ 
class mapsquare
{
public:
#if defined _DEBUG_ || defined _EDIT_
    static u_int16 a_d_diff;
#endif	 // _DEBUG_     
    /** 
     * Default constructor.
     * 
     */
    mapsquare ();
    
    /** 
     * Destructor.
     * 
     */
    ~mapsquare ();
    
    /** 
     * Returns whether the mapsquare is free for a character to go on or not.
     * @return
     *         - false if the mapsquare isn't free.
     *         - true if the mapsquare is free.
     */ 
    bool is_free ()
    {
        list <mapsquare_char>::iterator i;
        for (i = mapchars.begin (); i != mapchars.end (); i++)
            if (i->is_base)
                return false;
        return true;
    }

    /** 
     * Return a pointer to the mapcharacter that occupies this mapsquare.
     *
     * @return pointer to the mapcharacter that occupies the mapsquare, NULL if none.
     */
    mapcharacter *whoshere ()
    {
        list <mapsquare_char>::iterator i;
        for (i = mapchars.begin (); i != mapchars.end (); i++)
            if (i->is_base)
                return i->mchar;
        return NULL;
    }
    
    /**
     * Return whether this mapsquare is walkable from west.
     * @return
     *         - false if the mapsquare isn't walkable from west.
     *         - true if the mapsquare is walkable from west.
     */ 
    bool is_walkable_west ()
    {
        return (walkable & WALKABLE_WEST);
    }
    
    /**
     * Return whether this mapsquare is walkable from east.
     * @return
     *         - false if the mapsquare isn't walkable from east.
     *         - true if the mapsquare is walkable from east.
     */ 
    bool is_walkable_east ()
    {
        return (walkable & WALKABLE_EAST);
    }
    
    /**
     * Return whether this mapsquare is walkable from north.
     * @return
     *         - false if the mapsquare isn't walkable from north.
     *         - true if the mapsquare is walkable from north.
     */ 
    bool is_walkable_north ()
    {
        return (walkable & WALKABLE_NORTH);
    }
    
    /**
     * Return whether this mapsquare is walkable from south.
     * @return
     *         - false if the mapsquare isn't walkable from south.
     *         - true if the mapsquare is walkable from south.
     */ 
    bool is_walkable_south ()
    {
        return (walkable & WALKABLE_SOUTH);
    }

    /** 
     * Set whether a mapsquare is walkable from west.
     * @param w @li true: the mapsquare is walkable from west.
     *          @li false: the mapsquare isn't walkable from west.
     */ 
    void set_walkable_west (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_WEST);
        else
            walkable |= WALKABLE_WEST;
    }
    
    /** 
     * Set whether a mapsquare is walkable from east.
     * @param w @li true: the mapsquare is walkable from west.
     *          @li false: the mapsquare isn't walkable from west.
     */ 
    void set_walkable_east (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_EAST);
        else
            walkable |= WALKABLE_EAST;
    }
    
    /** 
     * Set whether a mapsquare is walkable from north.
     * @param w @li true: the mapsquare is walkable from west.
     *          @li false: the mapsquare isn't walkable from west.
     */ 
    void set_walkable_north (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_NORTH);
        else
            walkable |= WALKABLE_NORTH;
    }
    
    /** 
     * Set whether a mapsquare is walkable from south.
     * @param w @li true: the mapsquare is walkable from west.
     *          @li false: the mapsquare isn't walkable from west.
     */ 
    void set_walkable_south (bool w)
    {
        if (!w)
            walkable &= (ALL_WALKABLE - WALKABLE_SOUTH);
        else
            walkable |= WALKABLE_SOUTH;
    }

private:
    /// Terrain type ; need to be defined later.
    u_int16 type;
    /// Walkable flags.
    u_int8 walkable;
    
#ifndef SWIG
    /// List of mapsquare_tiles.
    list <mapsquare_tile> tiles;

    /// Iterator to where the base tiles begin.
    list <mapsquare_tile>::iterator base_begin;

    /// List of mapsquare_chars.
    list <mapsquare_char> mapchars;
#endif
    friend class mapsquare_tile;
    friend class landsubmap;
    friend class landmap;
    friend class mapview;
};	  // mapsquare

class landmap;

#ifndef SWIG

/**
 * Contains a 2 dimensionnal array of mapsquares for the world to take
 * place on.
 * This class allows you to handle an actual land, where characters can walk
 * and mapobjects can be placed on. However, it just contains informations ABOUT
 * the mapcharacters and mapobjects, and NOT the mapcharacters and mapobjects
 * themselves. As a consequence, it's totally useless and unusuable without
 * a landmap, to which is should always belong.
 */ 
class landsubmap
{ 
public:
#if defined _DEBUG_ || defined _EDIT_
    static u_int16 a_d_diff;
#endif 	  // _DEBUG_
    /** 
     * Default constructor.
     * 
     */
    landsubmap ();

    /** 
     * Makes a landsubmap of a given size.
     * 
     * @param l length of the landsubmap, in mapsquares.
     * @param h height of the landsubmap, in mapsquares.
     */ 
    landsubmap (u_int16 l, u_int16 h);

    /** 
     * Destructor.
     * 
     */
    ~landsubmap ();

    /** 
     * Resize the landsubmap.
     * 
     * @param l new length in mapsquares.
     * @param h new height in mapsquares.
     */
    void resize (u_int16 l, u_int16 h);

    /** 
     * Load a landsubmap from an opened file.
     * 
     * @param file opened file from which to load.
     * 
     * @return @li 0 in case of success.
     *         @li -1 in case of error.
     */
    s_int8 get (igzstream& file);

#ifdef _EDIT_
    /** 
     * Put a landsubmap to an opened file.
     * 
     * @param file opened file where to save.
     * 
     * @return @li 0 in case of success.
     *         @li -1 in case of error.
     */
    s_int8 put (ogzstream& file);
#endif							// _EDIT_

    /** 
     * Get the length of the landsubmap.
     * 
     * 
     * @return length of the landsubmap.
     */
    u_int16 get_length ();

    /** 
     * Get the height of the landsubmap.
     * 
     * 
     * @return height of the landsubmap.
     */ 
    u_int16 get_height ();

    /** 
     * landsubmap copy.
     * 
     */ 
     landsubmap & operator = (landsubmap & sm);

private:
    /// Reference to the parent map
    landmap * m_map;

    /// Area.
    mapsquare **land;
    u_int16 length, height;
    inline void allocmap (u_int16 l, u_int16 h);
    inline void destroymap ();
    s_int8 put_mapobject (u_int16 px, u_int16 py, u_int16 patnbr);

    friend class mapview;
    friend class mapcharacter;
    friend class landmap;
};								// landsubmap

#endif // SWIG

/**
 * Map where the world take place.
 * This class handles everything that is needed for map display. More
 * specifically, it includes:
 * @li landsubmaps, 
 * @li mapcharacters,
 * @li mapobjects.
 * It can make a map and the characters that are on it "live", but isn't
 * designed for display. See the mapview class for that.
 * @todo This needs cleanup!
 */
class landmap
#ifndef _EDIT_
    :public event_list
#endif
{
public:
#if defined _DEBUG_ || defined _EDIT_
    static u_int16 a_d_diff;
#endif	 // _DEBUG_

    /**
     * Default constructor.
     * 
     */ 
    landmap ();

    /**
     * Destructor.
     * 
     */ 
    ~landmap ();

    /**
     * Cleanup the map.
     * Totally cleanup a map, that is deleting every
     * mapobject/mapcharacter/landsubmap it contains, and reset it to
     * a stable state (just like it has just been created).
     */
    void clear ();

    /** 
     * Get the number of mapobjects that the map owns.
     * @return Number of mapobjects the map contains.
     */
    u_int16 get_nbr_of_patterns ()
    {
        return nbr_of_patterns;
    }

    /** 
     * Get the number of landsubmaps that the map owns.
     * @return Number of landsubmaps the map contains.
     */
    u_int16 get_nbr_of_submaps ()
    {
        return nbr_of_submaps;
    }

    /** 
     * Get the filename of the map, i.e the file from which
     * it has been loaded (if any).
     * 
     * 
     * @return filename of the map.
     */
    string filename ()
    {
        return filename_;
    }

    /** 
     * Load a map from an opened file.
     * 
     * @param file the file from which to load.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     * @todo Replace the lame string loading function with a better one
     * (need format change)
     */
    s_int8 get (igzstream& file);

    /** 
     * Load a map from a filename.
     * 
     * @param fname the filename from which to load.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 load (string fname);
#ifdef _EDIT_

    /** 
     * Put a map into an opened file.
     * 
     * @param file the file where to save.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 put (ogzstream& file);

     /** 
     * Save a map into a file.
     * 
     * @param fname the filename where to save.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 save (string fname);
#endif							// _EDIT_

    /** 
     * Put a mapcharacter on a given submap, at a given place.
     * 
     * @param mchar the mapcharacter to place.
     * @param smap the submap where to place the mapcharacter.
     * @param px X position where to place the mapcharacter.
     * @param py Y position where to place the mapcharacter.
     */
    void put_mapchar (mapcharacter * mchar, u_int16 smap, u_int16 px,
                      u_int16 py);

    
    /** 
     * Remove a mapcharacter on a given submap, at a given place.
     * 
     * @param mchar the mapcharacter to remove.
     * @param smap the submap where to remove the mapcharacter.
     * @param px X position where to remove the mapcharacter.
     * @param py Y position where to remove the mapcharacter.
     */
    void remove_mapchar (mapcharacter * mchar, u_int16 smap, u_int16 px,
                         u_int16 py);
    
    /** 
     * Add a mapcharacter to the map. The map will then be responsible for
     * it, and the mapcharacter can safely be placed on the map.
     * 
     * @param m mapcharacter to add.
     */
    void add_mapcharacter (mapcharacter * m)
    {
        mapchar.push_back (m);
    }

    /** 
     * Get a mapcharacter from it's index.
     * 
     * @param nbr index of the mapcharacter.
     * 
     * @return pointer to the corresponding mapcharacter.
     */
    mapcharacter *get_mapchar (u_int16 nbr)
    {
        return mapchar[nbr];
    }
    
    /** 
     * Remove a landsubmap from the landmap.
     * 
     * @param nbr the index of the submap to remove
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of error.
     */
    s_int8 remove_submap (u_int16 nbr);

    /** 
     * Put a mapobject on the map.
     * 
     * @param smap index of the submap to put the object on.
     * @param px X position to put the mapobject on
     * @param py Y position to put the mapobject on.
     * @param patnbr index of the mapobject to put.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 put_mapobject (u_int16 smap, u_int16 px, u_int16 py,
                          u_int16 patnbr);

    /** 
     * Update the entire map (mapcharacters, mapobjects, etc... of 1 cycle.
     * 
     */
    void update ();

#ifndef SWIG
    /** 
     * Add an empty landsubmap to the landmap.
     * 
     * 
     * @return the index of the new submap.
     */
    s_int8 add_submap ();

    /** 
     * Add a new landsubmap of a given size to the landmap.
     * 
     * @param l submap's length.
     * @param h submap's height.
     * 
     * @return the index of the new submap.
     */
    s_int8 add_submap (u_int16 l, u_int16 h);

    /** 
     * Remove a mapobject from the map.
     * 
     * @param smap index of the submap to remove the object on.
     * @param obj iterator to the mapobject to remove.
     */
    void remove_mapobject (u_int16 smap,
                           list <mapsquare_tile>::iterator obj);

    /** 
     * Adds a mapobject to a landmap.
     * 
     * @param an the mapobject to insert.
     * @param pos the position where to insert the mapobject.
     * @param srcfile the name of the file where the mapobject come from.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of error.
     */
    s_int8 insert_mapobject (mapobject * an, u_int16 pos,
                             const char *srcfile = "");

    /** 
     * Delete a mapobject from a landmap.
     * 
     * @param pos the index of the mapobject to delete.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 delete_mapobject (u_int16 pos);

    /**
     * landmap copy.
     * 
     */

    landmap & operator = (const landmap & lm);
         
private: 
#ifndef SWIG
#ifdef _EDIT_
    mapobject **mini_pattern;

    void update_current_tile ();

    void increase_obj_here ();
    void decrease_obj_here ();
    void increase_currentobj ();
    void decrease_currentobj ();

    void reset_objs ();

#endif							// _EDIT_
#endif							// SWIG
#ifndef SWIG
    string filename_;
    vector <mapcharacter *>mapchar;
    mapobject **pattern;
    vector <string> objsrc;
    landsubmap **submap;
#endif							// SWIG
    
    u_int16 nbr_of_patterns;
    u_int16 nbr_of_submaps; 
#endif							// SWIG

public:
#ifdef _EDIT_
    void editor ();
#endif							// _EDIT_

    /// @todo remove all these lame friendship relationships! 
    friend class mapengine;
    friend class mapview;
    friend class landsubmap; 
    friend class mapcharacter;
    friend class mapsquare_tile; 
    friend class mapsquare_char;  
};								// landmap

#endif // _LANDMAP_H
