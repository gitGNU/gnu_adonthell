/*
   $Id$

   Copyright (C) 1999/2000/2001 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/



/**
 * @file landmap.h
 * Declares the landmap class.
 */



#ifndef LANDMAP_H_
#define LANDMAP_H_

#include "mapobject.h"
#include "mapcharacter.h"

/**
 * Subdirectory where maps are saved.
 */
#define MAPS_DIR "maps/"

class mapview;

/**
 * Map where the world takes place.
 * This class handles everything that is needed for map display. More
 * specifically, it includes:
 * @li submaps (i.e mapsquare_area), 
 * @li mapobjects.
 * @li pointers to mapcharacters,
 * It can make a map and the characters that are on it "live", but isn't
 * designed for display. See the mapview class for that.
 *
 * This class is responsible for the storage of it's submaps and mapobjects,
 * but NOT mapcharacters. So be sure to delete () yourself your mapcharacters
 * when you don't need them anymore.
 *
 */
class landmap : public event_list
{
 public: 
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
     * @name Map information
     * 
     */
    //@{
    
    /** 
     * Get the number of mapobjects that the map owns.
     * @return number of mapobjects the map contains.
     */
    u_int16 nbr_of_mapobjects () const
    {
        return mobj.size ();
    }
    
    /** 
     * Get the number of landsubmaps that the map owns.
     * @return number of landsubmaps the map contains.
     */
    u_int16 nbr_of_submaps () const
    {
        return submap.size ();
    }

    /** 
     * Get the number of mapcharacters that are on this map.
     * @return number of mapcharacters on this map.
     */
    u_int16 nbr_of_mapcharacters () const
    {
        return mapchar.size ();
    }

    /** 
     * Get the filename of the map, i.e the file from which
     * it has been loaded (if any).
     * 
     * 
     * @return filename of the map.
     */
    string filename () const
    {
        return filename_;
    }
    
    //@}

    
    /**
     * @name Individual map elements manipulation
     * Using these methods should be avoided as long as possible. They
     * are here for completeness, but their use should be exceptionnal.
     * 
     */

    //@{
    
    /** 
     * Returns a pointer to a mapcharacter on this landmap.
     * 
     * @param pos index of the mapcharacter to return.
     * 
     * @return pointer to the \e pos mapcharacter.
     */
    mapcharacter * get_mapcharacter (u_int16 pos) 
    {
        return mapchar[pos]; 
    }

    /** 
     * Returns a pointer to a mapobject belonging to this landmap.
     * 
     * @param pos index of the mapobject to return.
     * 
     * @return pointer to the \e pos mapobject.
     */
    mapobject * get_mapobject (u_int16 pos) 
    {
        return mobj[pos]; 
    }

    /** 
     * Returns a pointer to a submap belonging to this landmap.
     * 
     * @param pos index of the submap to return.
     * 
     * @return pointer to the \e pos submap.
     */
    mapsquare_area * get_submap (u_int16 pos) 
    {
        return submap[pos]; 
    }

    //@}


    /**
     * @name State updating
     * 
     */

    //@{

    /** 
     * Update the entire map (mapcharacters, mapobjects, etc... of 1 cycle.
     * 
     */
    void update ();

    //@}


    /**
     * @name Loading/Saving methods.
     * 
     */

    //@{

    /** 
     * Load a map from an opened file.
     * 
     * @param file the file from which to load.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
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

    /** 
     * Put a map into an opened file.
     * 
     * @param file the file where to save.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 put (ogzstream& file) const;

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

    //@}
    

    /**
     * @name State loading/saving methods.
     * 
     */

    //@{

    /** 
     * Restore the landmap's state from an opened file.
     * 
     * @param file the opened file from which to load the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 get_state (igzstream& file);

    /** 
     * Saves the landmap's state into an opened file.
     * 
     * @param file the opened file where to the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 put_state (ogzstream& file) const;

    //@}

    /**
     * @name Landmap modification
     * Although it should be very rare to modify a landmap
     * during gameplay, these methods are here to allow you
     * to safely to it. Be aware that they check if each element
     * is in a safe state, and modify them if necessary. Therefore,
     * they are quite slow and should be used in exceptionnal situations.
     *
     * Note however that put_mapobject () and remove_mapobject () should
     * be fast enough to allow real-time map modifications. But beware anyway.
     *
     */

    //@{
    

    /** 
     * Put a mapobject on the map.
     * 
     * @param smap index of the submap to put the object on.
     * @param px X position to put the mapobject on
     * @param py Y position to put the mapobject on.
     * @param mobjnbr index of the mapobject to put.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of failure.
     */
    s_int8 put_mapobject (u_int16 smap, u_int16 px, u_int16 py,
                          u_int16 mobjnbr);

    /** 
     * Remove a mapobject from the map.
     * 
     * @param smap index of the submap to remove the object on.
     * @param px X position of the mapobject.
     * @param py Y position of the mapobject.
     * @param mobjnbr index of the mapobject to remove.
     */
    void remove_mapobject (u_int16 smap, u_int16 px, u_int16 py, 
                           u_int16 mobjnbr);

    
    /** 
     * Inserts an empty landsubmap into the landmap.
     *
     * The landmap can then be accessed for resizing with
     * get_submap ()
     *
     * @param pos the position where to insert the submap.
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of error.
     *
     * @sa get_submap () 
     */
    s_int8 insert_submap (u_int16 pos);

    /** 
     * Remove a landsubmap from the landmap.
     * 
     * @param pos the index of the submap to remove
     * 
     * @return
     * @li 0 in case of success.
     * @li -1 in case of error.
     */
    s_int8 delete_submap (u_int16 pos);

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
                             string srcfile = "");

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

    //@} 
    
private: 
    /**
     * Forbids value passing.
     *
     */
    landmap (const landmap& src);

#ifndef SWIG
    /**
     * Forbids landmap copy.
     * 
     */ 
    landmap & operator = (const landmap & src);
#endif
    
    string filename_;
    vector <mapcharacter *> mapchar;
    vector <mapobject *> mobj;
    vector <string> mobjsrc;
    vector <mapsquare_area *> submap;

#ifndef SWIG
    friend class mapcharacter; 
    friend class mapview; 
#endif

};

#endif
