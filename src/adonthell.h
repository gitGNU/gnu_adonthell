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
 * @file adonthell.h
 *
 * @author Alexandre Courbot
 * @author Kai Sterker
 * @brief Declares the adonthell class.
 */

#ifndef __ADONTHELL_H__
#define __ADONTHELL_H__

#include "win_mapview.h"

/**
 * This is the heart of the Adonthell engine. All activities,
 * be it checking for user input, calculating a new %game state
 * and finally rendering a scene are done in the 
 * \ref main_loop "main loop".
 *
 * This class works together closely with the 
 * \ref win_manager "window manager" which provides the basic
 * GUI control methods and the \ref gametime class which is
 * responsible for synchronising Adonthell to the machine it
 * is running on.
 */
class adonthell
{
  public:
    /**
     * Standard constructor
     */
    adonthell ();
   
    /**
     * @name The engine's main loop
     * 
     */ 
    //@{
    /**
     * Starts the main loop. Since having the mainloop running
     * without a window to display seems to make no sense, you'll
     * have to pass an inital window when starting the main loop.
     * This method can be called multiple times. Only those windows
     * belonging to the outermost main loop are updated and recieve
     * user input. However, all windows are drawn in correct order,
     * i.e. innermost first, outermost last. 
     *
     * See the \ref win_manager "window manager" for more details.
     *
     * @param wnd The window to display initially
     * @param name A name for that window (currently unused)
     */
    void main (win_base *wnd = NULL, const string name = "");
    
    /**
     * The actual main loop. First, any user %input is processed,
     * then the new %game state is calculated and finally all
     * open (and visible) windows are rendered and displayed on
     * screen. This (currently) happens up to 50 times per second.
     */
    void main_loop ();
    
    /**
     * Quit the main loop. This stops the outermost main loop
     * and closes all windows associated with that loop. It does
     * not delete them however. For that you'll have to call
     * win_manager::destroy ().
     */
    void main_quit ();
    //@}
    
    /**
     * @name Fading
     * 
     */ 
    //@{
    /**
     * Fades the screen to black
     */
    void fade_out ();

    /**
     * Fades in from a black screen
     */
    void fade_in ();
    //@}

    /**
     * @name Saving and Loading
     * 
     */ 
    //@{
    /**
     * Restore the engine's state. Loads the previously displayed map,
     * it's state and the state of the mapview from mapengine.data. 
     *
     * @param file The opened engine state file (mapengine.data).
     */
    s_int8 get_state (igzstream& file);

    /**
     * Save the engine's state. Writes the current map w/ it's state
     * and the state of the mapview to mapengine.data.
     *
     * @param file The opened engine state file (mapengine.data).
     */
    s_int8 put_state (ogzstream& file);
    //@}
    
    /**
     * @name Additional game control
     * 
     */ 
    //@{
    /**
     * Returns whether the control script is active or not.
     *
     * @return 
     *      @li true if that is the case.
     *      @li false otherwise.
     */
    bool control_active ()
    {
        return control_active_;
    }
    
    /**
     * Set whether the control script should be executed or
     * not. This script provides functionality that is not
     * directly related to contolling the main character,
     * like opening the main menu, the load or save screen,
     * etc.
     *
     * @param c Pass true to enable the control script, false
     *          to disable it.
     */
    void set_control_active (bool c)
    {
        control_active_ = c;
    }
    //@}

    /**
     * @todo move landmap handling somewhere else
     */
    landmap *get_landmap ()
    {
        return &lmap;
    }

    /**
     * @todo move landmap handling somewhere else
     */
    bool update_map ()
    {
        return update_map_;
    }
    
    /**
     * @todo move landmap handling somewhere else
     */
    void set_update_map (bool u)
    {
        update_map_ = u;
    }
    
    /**
     * @todo move mapview handling somewhere else
     */
    mapview *get_mapview ()
    {
        return (mapview*) &view;
    }

    /**
     * @todo move mapview handling somewhere else
     */
    void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL,
               surface * target = NULL)
    {
        view.mapview::draw (x, y, da_opt, target); 
    }
    
    /**
     * @todo move mapview handling somewhere else
     */
    void set_mapview_schedule (string s, PyObject * args = NULL)
    {
        view.mapview::set_schedule (s, args); 
    }

    /**
     * @todo move mapview handling somewhere else
     */
    void mapview_start ();
    
    /**
     * @todo move mapview handling somewhere else
     */
    void mapview_stop ();

  private:
    py_object control;
    // flag to indicate whether the control script is active or not
    bool control_active_;
    // flag to indicate whether to exit the main loop
    bool letsexit;
    // indicates whether the map should be updated or not
    bool update_map_;
    // the current map
    landmap lmap;
    // the current mapview
    win_mapview view;
};

#ifndef SWIG
namespace data
{
    /**
     * Engine used during the game.
     * 
     */ 
    extern adonthell *engine;
}
#endif // SWIG

#endif // __ADONTHELL_H__
