/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
#include "mapsquares.h"


/// Where the mapobjects resides in the data tree.
#define MAPOBJECTS_DIR "gfx/mapobjects/"



/**
 * Objects that can be placed on a map.
 *
 * A mapobject is basically a set of animations. Each animation can be freely
 * placed on a resizeable grid which represents the actual land where the
 * object will be placed. This grid also has information about the walkability
 * of it's squares, which will be repercuted on the landmap as soon as the
 * object is placed.
 * 
 */ 
class mapobject : public drawable
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
     * @name Area settings.
     * 
     */ 
    //@{ 

    /**
     * Returns the length of the mapobject's area.
     *
     * @return length (in number of squares) of the mapobject's area.
     *
     */ 
    u_int16 area_length () const
    {
        return area.size (); 
    }

    /**
     * Returns the height of the mapobject's area.
     *
     * @return height (in number of squares) of the mapobject's area.
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
    mapsquare_walkable * get_square (u_int16 x, u_int16 y)
    {
        return &(area[x][y]); 
    }
    
    /** 
     * Resize the mapobject's area.
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
     * @name State updating.
     * 
     */ 
    //@{ 
         
    /** 
     * Updates the mapobject's state.
     * 
     */
    void update ();

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
    u_int16 nbr_of_animations ()
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
    void copy (mapobject& src) 
    {
        *this = src; 
    }

private:
    /** 
     * Calculate the mapobject's length and height.
     * 
     */
    void calculate_dimensions ();

    /**
     * Forbid value passing.
     * 
     */ 
    mapobject (mapobject &src); 
    mutable vector <animation *> anim;
    vector <vector<mapsquare_walkable> > area;

    u_int16 basex;
    u_int16 basey; 
};

#endif
