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

#ifndef GUI_MANAGER_
#define GUI_MANAGER_

#include <list> 
#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include "theme.h"
#include "font.h"
#include "base.h"
#include "../str_hash.h"

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

namespace gui {

  class manager
    {
    public:
      /**
       * Standard constructor
       */
      manager ();
      
      /**
       * Destructor
       */
      ~manager ();
      
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
      void add (base *wnd);
      
      // static bool exist (base *);
      
      /**
       * Remove a window from the window manager. The
       * window is erased from the window list, but not deleted.
       * If it had the %input focus, it is passed on to the topmost
       * window, i.e. the last one in the window list (if such a 
       * window exists).
       *
       * @param wnd The window to be removed
       */
      void remove (base *wnd);
      
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
      void set_focus (base *wnd);
      
      /**
       * Closes and deletes all windows of the current level.
       */
      void destroy ();
      
#ifndef SWIG
      /**
       * Pointer to the active, i.e. topmost window manager.
       */
      static manager *active;
#endif
      
      /**
       * Use this method to get the active manger from Python
       */
      static manager *get_active ()
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
      static void add_theme (std::string name); 
      
      /**
       * Delete a theme.
       *
       * @param name The name of the theme to delete.
       * @return 
       *      @li true in case of success.
       *      @li false in case of error.
       */
      static bool remove_theme (std::string name); 
      
      /**
       * Returns a pointer to a theme. Loads the theme from disk
       * if it isn't in memory yet.
       *
       * @param name The name of the theme to get.
       * @return Pointer to the theme.
       */
      static theme *get_theme (std::string name); 

      /**
       * Load a font from disk.
       *
       * @param name The name of the font to load.
       */
      static void add_font (std::string name); 

      /**
       * Delete a font.
       *
       * @param name The name of the font to delete.
       * @return 
       *      @li true in case of success.
       *      @li false in case of error.
       */
      static bool remove_font (std::string name); 
      
      /**
       * Returns a pointer to a font. Loads the font from disk
       * if it isn't in memory yet.
       *
       * @param name The name of the font to get.
       * @return Pointer to the font.
       */
      static font *get_font (std::string name); 
    
    //@}

    private:
#ifndef SWIG
      static std::hash_map<std::string, theme *> my_theme; 
      static std::hash_map<std::string, font *> my_font; 
      
      std::list<base *> wnd_list;
      std::list<base *>::iterator current;
      base *wnd_focus;
      manager *prev;
#endif // SWIG
    };
}
#endif
 
