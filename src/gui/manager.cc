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
 * @file win_theme.cc
 *
 * @author Joel Vennin
 * @brief Implements the win_theme class.
 */

#include "../types.h"
#include "../gfx/image.h"
#include "types.h"
#include "manager.h"

using namespace gui;

// Pointer to the active window(s)
manager * manager::active = NULL;
// List of loaded themes
std::hash_map<std::string, theme *> manager::my_theme;
// List of loaded fonts
std::hash_map<std::string, font *> manager::my_font; 

using namespace std; 


manager::manager ()
{
    // save a pointer to the parent window(s)
    prev = active;
    // make the current window(s) active
    active = this;
    // no window in focus at that point
    wnd_focus = NULL;
    // we're not iterating over the window_list
    current = wnd_list.end ();
}

manager::~manager ()
{
  // restore parent window(s)
  active = prev;
}

// Close and delete all windows
void manager::destroy()
{
  std::list<base *>::iterator i;
  for (i = wnd_list.begin(); i != wnd_list.end(); i++) delete *i;
  
  wnd_list.clear ();
  wnd_focus = NULL;
}

void manager::init () 
{
}

// Delete all fonts and themes
void manager::cleanup () 
{
  // Cleaning up themes
  for (std::hash_map <std::string, theme *>::iterator it = my_theme.begin ();
       it != my_theme.end (); it++)
    delete it->second;
  my_theme.clear (); 
  
  // Cleaning up fonts
  for (std::hash_map <std::string, font *>::iterator ifo = my_font.begin ();
       ifo != my_font.end (); ifo++)
    delete ifo->second;
  my_font.clear (); 
}

// add a window 
void manager::add (base *tmp)
{
  wnd_list.push_back (tmp);
}
        
// remove a window 
void manager::remove (base *tmp)
{
  // if the window has focus take it away
  if (tmp->is_focus ()) 
    {
      tmp->set_focus (false);
      wnd_focus = NULL;
    }
  
  // be careful if the iterator points to the element 
  // we want to remove. This may happen if remove() is called
  // from a window's update() method or from manager::update().
  if (tmp == *current)
    {
      // make sure that the iterator remains valid
      current++;
    }
  
  // remove it from the window list
  wnd_list.remove (tmp);
  
  // if no window has the focus, give it to the topmost window
  if (!wnd_focus) set_focus (wnd_list.back ());
}

// draw all windows
void manager::draw ()
{
  // first descent recursively down the list of parents 
  if (prev != NULL) prev->draw ();
  
  // on the way up, draw every window
  for (current = wnd_list.begin (); current != wnd_list.end(); current++)
    (*current)->draw ();
}

// grab keyboard input
void manager::input_update ()
{  
  // only the window with the focus may recieve input
  if (wnd_focus) wnd_focus->input_update (); 
}

// update the state of the topmost window(s)
void manager::update ()
{
  for (current = wnd_list.begin (); current != wnd_list.end ();)
    // a window signals that it wants to be closed by returning 0 here
    if (!(*current)->update ())
      {
	// remove and delete it
	base *tmp = *current;
	
	remove (tmp);
	delete tmp;
      }
    else current++;
}

// give the focus to a window 
void manager::set_focus (base *tmp)
{
    // but only if there are any windows at all
  if (!wnd_list.empty ())
    {
      // remove focus from the old window
      if (wnd_focus) wnd_focus->set_focus (false);
      
      // and give it to the new one
      wnd_focus = tmp;
      wnd_focus->set_focus (true);
    }   
}

// load a theme from disk
void manager::add_theme (std::string name)
{
  my_theme[name] = new theme ((char *) name.c_str ()); 
}

// remove a theme
bool manager::remove_theme (std::string name)
{
  std::hash_map <std::string, theme *>::iterator it = my_theme.find (name);
  if (it == my_theme.end ()) return false;
  
  delete it->second;
  my_theme.erase (it);
  return true; 
}

// return a pointer to a theme
theme * manager::get_theme (std::string name)
{
  hash_map <std::string, theme *>::iterator it = my_theme.find (name); 
  
  // try to load it if it's not in memory yet 
  if (it == my_theme.end ())
    {
      add_theme (name);
      return get_theme (name);
    }
  else return it->second; 
}

// load a font from disk
void manager::add_font (std::string name)
{
  my_font[name] = new font ((char *) name.c_str ());
}

// remove a font
bool manager::remove_font (std::string name)
{
  std::hash_map <std::string, font *>::iterator it = my_font.find (name);
  if (it == my_font.end ()) return false;
  
  delete it->second;
  my_font.erase (it);
  return true; 
}

// return a pointer to a font
font * manager::get_font (std::string name)
{
  std::hash_map <std::string, font *>::iterator it = my_font.find (name);
  
  // try to load the font if it's not in memory yet
  if (it == my_font.end ())
    {
      add_font (name);
      return get_font (name);
    }
  else return it->second; 
}
