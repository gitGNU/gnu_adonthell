/*
   $Id$
   
   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

/** 
 * @file win_manager.h
 *
 * @author Joel Vennin 
 * @brief Declares the win_manager class.
 */

#ifndef _WIN_MANAGER_
#define _WIN_MANAGER_

#include "str_hash.h"
#include "win_theme.h"
#include "win_font.h"

#ifndef SWIG
using namespace std;   
#endif

/** 
 * The window manager takes care of basic GUI functions, such as
 * %input focus, window state updates and displaying everything in
 * the right order.
 * It also provides centralised access to fonts and themes, so
 * that they can be used by different windows without having to
 * load them multiple times.
 * For something to appear on screen, it has to be passes to the
 * window manager.
 *
 * Before the window manager can be used, adonthell::main() has 
 * to be called. This instanciates a window manager object and 
 * makes it available to other classes via the static 
 * win_manager::active pointer. All windows added to that instance
 * have access to the input focus, although only one window can
 * own it at any given time.
 *
 * Another call to adonthell::main() will create a new window
 * manager instance that grabs the input focus. As long as it
 * is in existance, none of the parent windows are updated, nor may 
 * they recieve the focus. A call to adonthell::main_quit() will
 * delete the topmost window manager and return focus to the underlying
 * windows.
 *
 * That way it is possible to create a hierarchie of windows, where
 * where only windows on the same level may share the input focus,
 * but only those on the highest level receive input.
 */
class win_manager
{
public:
    /**
     * Standard constructor
     */
    win_manager ();

    /**
     * Destructor
     */
    ~win_manager ();
    
    /**
     * @name Window handling methods
     * 
     */ 
    //@{
    
    /**
     * Add a window to the window manager.
     *
     * @param wnd The window to be added
     */
    void add (win_base *wnd);
  
    // static bool exist (win_base *);
    
    /**
     * Remove a window from the window manager. The
     * window is erased from the window list, but not deleted.
     * If it had the %input focus, it is passed on to the topmost
     * window, i.e. the last one in the window list (if such a 
     * window exists).
     *
     * @param wnd The window to be removed
     */
    void remove (win_base *wnd);
    
    /**
     * Update the state of all top level windows. Calls the
     * %update() method of all windows in the window list. If
     * that method returns 0, it will be removed from the window
     * list and deleted.
     */
    void update ();
    
    /**
     * Checks for user input. Calls the %input_update() method of
     * the window that has the %input focus.
     *
     * @sa set_focus ()
     */
    void input_update ();
    
    /**
     * Draws <b>all</b> windows. If the window hierarchie consists
     * of multiple levels, the lowest windows are drawn first. Within
     * each level, windows are drawn in the order they appear in the
     * window list.
     */
    void draw ();
    
    /**
     * Gives the input focus to wnd. Only one window can have the
     * focus at a time, so focus will be removed from the window
     * that had it so far. Only the window with the focus will
     * receive user input.
     *
     * @sa input_update ()
     */
    void set_focus (win_base *wnd);
    
    /**
     * Closes and deletes all windows of the current level.
     */
    void destroy ();

#ifndef SWIG
    /**
     * Pointer to the active, i.e. topmost window manager.
     */
    static win_manager *active;
#endif
    
    /**
     * Use this method to get the active manger from Python
     */
    static win_manager *get_active ()
    {
        return active;
    }
    //@}

    /**
     * @name Theme and font related methods
     * 
     */ 
    //@{ 
             
    /**
     * Empty for now
     */
    static void init ();
    
    /**
     * Delete all themes and fonts currently loaded.
     */
    static void cleanup ();

    /**
     * Load a theme from disk.
     *
     * @param name The name of the theme to load.
     */
    static void add_theme (string name); 

    /**
     * Delete a theme.
     *
     * @param name The name of the theme to delete.
     * @return 
     *      @li true in case of success.
     *      @li false in case of error.
     */
    static bool remove_theme (string name); 
    
    /**
     * Returns a pointer to a theme. Loads the theme from disk
     * if it isn't in memory yet.
     *
     * @param name The name of the theme to get.
     * @return Pointer to the theme.
     */
    static win_theme *get_theme (string name); 

    /**
     * Load a font from disk.
     *
     * @param name The name of the font to load.
     */
    static void add_font (string name); 

    /**
     * Delete a font.
     *
     * @param name The name of the font to delete.
     * @return 
     *      @li true in case of success.
     *      @li false in case of error.
     */
    static bool remove_font (string name); 
    
    /**
     * Returns a pointer to a font. Loads the font from disk
     * if it isn't in memory yet.
     *
     * @param name The name of the font to get.
     * @return Pointer to the font.
     */
    static win_font *get_font (string name); 
    
    //@}

 private:
#ifndef SWIG
    static hash_map<string, win_theme *> theme; 
    static hash_map<string, win_font *> font; 
    
    list<win_base *> wnd_list;
    list<win_base *>::iterator current;
    win_base *wnd_focus;
    win_manager *prev;
#endif // SWIG
};

#endif
 
