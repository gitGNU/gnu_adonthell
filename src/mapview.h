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
 * @file   mapview.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the mapview class.
 * 
 * 
 */

 

#ifndef MAPVIEW_H_
#define MAPVIEW_H_

#include "landmap.h"
#include "py_inc.h"
#include <list>


/**
 * Allows you to display a landmap on a specified area of a surface.
 *
 * This class just acts as a "camera" which takes snapshots of a landmap.
 * It's size can be specified, a schedule can be set to
 * update it's movment. Nothing prevents you from having several mapviews
 * on the same map that displays each a different (or similar) part of the
 * landmap.
 * 
 */ 
class mapview : public drawable
{

public:

    /**
     * Default constructor.
     * 
     */ 
    mapview ();

    /**
     * Destructor.
     * 
     */ 
    ~mapview ();
 
    /** 
     * Sets which map this mapview will display.
     * 
     * @param m pointer to the map that will be displayed by the mapview.
     */
    void attach_map (landmap * m);

    /** 
     * Stops displaying a map.
     * 
     */
    void detach_map ();

    /** 
     * Sets the position of the top-left corner of the mapview on the map.
     *
     * You'll probably don't want to use this method. To center the mapview
     * on a precise position, see center_on () instead.
     *
     * @param sm submap.
     * @param x X position.
     * @param y Y position.
     * @param ox X offset.
     * @param oy Y offset.
     *
     * @sa center_on () 
     * 
     */
    s_int8 set_pos (u_int16 sm, u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);

    /** 
     * Sets the position of the center of the mapview on the map.
     * 
     * @param sm submap.
     * @param x X position.
     * @param y Y position.
     * @param ox X offset.
     * @param oy Y offset.
     * 
     */ 
    s_int8 center_on (u_int16 sm, u_int16 x, u_int16 y, s_int16 ox = 0, s_int16 oy = 0);

    /** 
     * Returns whether it is possible to scroll to right. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a right scrolling is possible, false otherwise.
     */
    bool can_scroll_right ()
    {
        s_int32 tpx = posx () * MAPSQUARE_SIZE + offx () + length ();
        return (tpx < m_map->submap[currentsubmap_]->area_length () * MAPSQUARE_SIZE);
    }
    
    /** 
     * Returns whether it is possible to scroll to left. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a left scrolling is possible, false otherwise.
     */
    bool can_scroll_left ()
    {
        return (posx_ || offx_);
    }
    
    /** 
     * Returns whether it is possible to scroll to up. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a up scrolling is possible, false otherwise.
     */
    bool can_scroll_up ()
    {
        return (posy_ || offy_);
    }

    /** 
     * Returns whether it is possible to scroll to down. A scrolling is impossible
     * if the mapview is at the map limits.
     * 
     * 
     * @return true if a down scrolling is possible, false otherwise.
     */
    bool can_scroll_down ()
    {
        s_int32 tpy = posy () * MAPSQUARE_SIZE + offy () + height ();
        return (tpy < m_map->submap[currentsubmap_]->area_height () * MAPSQUARE_SIZE);
    }

    /** 
     * Scrolls 1 pixel to right, if possible.
     * 
     */
    void scroll_right ();

    /** 
     * Scrolls 1 pixel to left, if possible.
     * 
     */
    void scroll_left ();

    /** 
     * Scrolls 1 pixel to down, if possible.
     * 
     */
    void scroll_down ();

    /** 
     * Scrolls 1 pixel to up, if possible.
     * 
     */
    void scroll_up ();

    /** 
     * Returns the submap this mapview is displaying.
     * 
     * 
     * @return current submap this mapview is displaying.
     */
    u_int16 currentsubmap ()
    {
        return currentsubmap_;
    }
    
    /** 
     * Returns the X position of the mapview.
     * 
     * 
     * @return X position of the mapview.
     */
    u_int16 posx () const
    {
        return posx_;
    }
    
    /** 
     * Returns the Y position of the mapview.
     * 
     * 
     * @return Y position of the mapview.
     */
    u_int16 posy () const
    {
        return posy_;
    }

    /** 
     * Returns the X offset of the mapview.
     * 
     * 
     * @return X offset of the mapview.
     */
    u_int16 offx () const
    {
        return offx_;
    }
    
    /** 
     * Returns the Y offset of the mapview.
     * 
     * 
     * @return Y offset of the mapview.
     */
    u_int16 offy () const
    {
        return offy_;
    }

    /** 
     * Resize the mapview. The parameters are given in pixels.
     * 
     * @param l new length.
     * @param h new height.
     */
    void resize (u_int16 l, u_int16 h);

    /** 
     * Restore the mapview's state from an opened file.
     * 
     * @param file the opened file from which to load the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */
    s_int8 get_state (igzstream& file);

    /** 
     * Saves the mapview's state into an opened file.
     * 
     * @param file the opened file where to the state.
     * 
     * @return 0 in case of success, error code otherwise.
     */ 
    s_int8 put_state (ogzstream& file);

    
    /** 
     * Assign a schedule to the mapview.
     * 
     * The schedule's filename will be \e "scripts/schedules/<file>.py".
     * 
     * @param file name of the schedule to use.
     */
    void set_schedule (string file);

    /** 
     * Returns the name of the mapview's current schedule.
     * 
     * 
     * @return name of the mapview's current schedule.
     */
    string schedule_file () 
    {
        return schedule_file_; 
    }

    /** 
     * Updates the mapview's state and launchs his schedule.
     * 
     */
    void update ();

    void draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
               surface *target = NULL) const;
 

private:
    
#ifndef SWIG
    void draw_tile (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target,
                    list<mapsquare_tile>::iterator it) const;
    
    void draw_mapchar (s_int16 x, s_int16 y, const drawing_area * da_opt,
                       surface * target, list<mapsquare_char>::iterator itc) const;  
#endif
    
    u_int16 d_length, d_height;	// size of the view in map squares

    u_int16 currentsubmap_;
    u_int16 posx_, posy_;
    u_int16 offx_, offy_;
    
    landmap *m_map;

    mutable drawing_area da; 
    
    mutable list <mapsquare_tile> critical_draw;
    mutable list <mapsquare_char> characters_draw;

    PyObject *locals;			// Locals that belong to that mapview
    py_script schedule;
    string schedule_file_; 

};

#endif
