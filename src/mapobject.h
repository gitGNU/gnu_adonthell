/*
   Copyright (C) 1999/2000/2001   Alexandre Courbot
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
 * @file mapobject.h
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Declares the mapobject class.
 */


#ifndef _MAPOBJECT_H
#define _MAPOBJECT_H

#include "animation.h"
#include "mapsquare_walkable.h"


/// Where the mapobjects resides in the data tree.
#define MAPOBJECTS_DIR "gfx/mapobjects/"



/**
 * Objects that can be placed on a landmap.
 *
 * A mapobject is basically a set of animations. Each animation can be freely
 * placed on a resizeable grid which represents the actual land where the
 * object will be placed. This grid also has information about the walkability
 * of it's squares, which will be repercuted on the landmap as soon as the
 * object is placed.
 * 
 */ 
class mapobject : public mapsquare_walkable_area
{
public:

    /**
     * Default constructor.
     * 
     */
    mapobject ();

    /**
     * Destructor.
     * 
     */ 
    ~mapobject ();

    /**
     * Resets the mapobject to its post-constructor state.
     * 
     */ 
    void clear ();      
 
    /**
     * @name State updating.
     * 
     */ 
    //@{ 
         
    /** 
     * Updates the mapobject's state.
     * 
     */
    bool update ();

    //@}

      
    /**
     * @name Drawing methods.
     * 
     */ 
    //@{ 
    
    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL, surface * target = NULL) const;

    /** 
     * Similar to draw (), but assume the x and y parameters are where the base
     * square should appear.
     * 
     * @param x X position where to draw.
     * @param y Y position where to draw.
     * @param da_opt optional drawing_area to use during the drawing operation.
     * @param target pointer to the surface where to draw the drawable. If NULL, draw on the screen.
     */
    void draw_from_base (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                         surface * target = NULL) const; 


    //@}


 
    /**
     * @name Loading/saving methods.
     *
     * @note You can't save a mapobject with this class.
     * 
     */ 
    //@{ 
     
    /**
     * Loads a mapobject from an opened file.
     * @param file the opened file from which to load.
     * @return 0 in case of success, error code otherwise.
     *
     */ 
    s_int8 get (igzstream & file);

    /** 
     * Loads a mapobject from it's filename.
     * 
     * @param fname the name of the file to load.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 load (string fname);

    /** Saves an mapobject into an opened file, in %game format, with
     *  alpha and mask values. 
     *  @warning as the mapobject which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving mapobjects with full
     *           truecolor quality, use mapobject_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save ()
     */
    s_int8 put (ogzstream& file) const;

    /** Saves an mapobject into an file, in %game format, with
     *  alpha and mask values.
     *  @warning as the mapobject which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving mapobjects with full
     *           truecolor quality, use mapobject_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put ()
     */
    s_int8 save (string fname) const;

    //@}


     
    /**
     * @name Individual animations manipulation.
     * 
     */ 
    //@{ 
    
    /** 
     * Returns the number of animations of this mapobject.
     * 
     * 
     * @return the number of animations of this mapobject.
     */
    u_int16 nbr_of_animations () const
    {
        return anim.size ();
    }
    
    /** 
     * Returns a pointer to one of the mapobject's animations.
     * 
     * @param nbr index of the animation to get.
     * 
     * @return pointer to the nbr animation.
     */
    animation *get_animation (u_int16 nbr)
    {
        return anim[nbr];
    }
    
    /** 
     * Inserts an animation at a given position of the animations array.
     * 
     * The mapobject will be responsible for freeing the inserted animation.
     *
     * @param an pointer to the animation to add.
     * @param pos index where to add the animation.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 insert_animation (animation * an, u_int16 pos);

    
    /** 
     * Removes an animation at a given position.
     * The animation itself will also be deleted ().
     *
     * @param pos The index of the animation to remove.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 delete_animation (u_int16 pos);
    
    //@}
    
    
#ifndef SWIG
    /**
     * Mapobject copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */ 
    mapobject & operator = (const mapobject & mo);
#endif

    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (const mapobject& src) 
    {
        *this = src; 
    }

private:

    /**
     * Forbid value passing.
     * 
     */ 
    mapobject (mapobject &src); 

    mutable vector <animation *> anim;
};

#endif
