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

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_manager.h"


// Pointer to the active window(s)
win_manager* win_manager::active = NULL;
// List of loaded themes
hash_map<string, win_theme *> win_manager::theme;
// List of loaded fonts
hash_map<string, win_font *> win_manager::font; 

using namespace std; 


win_manager::win_manager ()
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

win_manager::~win_manager ()
{
    // restore parent window(s)
    active = prev;
}

// Close and delete all windows
void win_manager::destroy()
{
    list<win_base *>::iterator i;
    
    for (i = wnd_list.begin(); i != wnd_list.end(); i++)
        delete *i;
  
    wnd_list.clear ();
    wnd_focus = NULL;
}

void win_manager::init () 
{
}

// Delete all fonts and themes
void win_manager::cleanup () 
{
    // Cleaning up themes
    for (hash_map <string, win_theme *>::iterator it = theme.begin ();
         it != theme.end (); it++)
        delete it->second;
    theme.clear (); 

    // Cleaning up fonts
    for (hash_map <string, win_font *>::iterator ifo = font.begin ();
         ifo != font.end (); ifo++)
        delete ifo->second;
    font.clear (); 
}

// add a window 
void win_manager::add (win_base *tmp)
{
    wnd_list.push_back (tmp);
}

/*
bool win_manager::exist(win_base *tmp)
{
    for (list<win_base *>::iterator i = wnd_list.begin ();
        i != wnd_list.end(); i++)
        if (*i == tmp) return true;

    return false;
}
*/
        
// remove a window 
void win_manager::remove (win_base *tmp)
{
    // if the window has focus take it away
    if (tmp->is_focus ()) 
    {
        tmp->set_focus (false);
        wnd_focus = NULL;
    }
    
    // be careful if the iterator points to the element 
    // we want to remove. This may happen if remove() is called
    // from a window's update() method or from win_manager::update().
    if (tmp == *current)
    {
        // make sure that the iterator remains valid
        current--;
    }
    
    // remove it from the window list
    wnd_list.remove (tmp);
    
    // if no window has the focus, give it to the topmost window
    if (!wnd_focus) set_focus (wnd_list.back ());
}

// draw all windows
void win_manager::draw ()
{
    // first descent recursively down the list of parents 
    if (prev != NULL) prev->draw ();
  
    // on the way up, draw every window
    for (list<win_base *>::iterator i = wnd_list.begin();
        i != wnd_list.end(); i++)
        (*i)->draw ();
}

// grab keyboard input
void win_manager::input_update ()
{  
    // only the window with the focus may recieve input
    if (wnd_focus) wnd_focus->input_update (); 
}

// update the state of the topmost window(s)
void win_manager::update ()
{
    for (current = wnd_list.begin (); current != wnd_list.end (); current++)
        // a window signals that it wants to be closed by returning 0 here
        if (!(*current)->update ())
        {
            // remove and delete it
            win_base *tmp = *current;
            
            remove (tmp);
            delete tmp;
        }
}

// give the focus to a window 
void win_manager::set_focus (win_base *tmp)
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
void win_manager::add_theme (string name)
{
    theme[name] = new win_theme ((char *) name.c_str ()); 
}

// remove a theme
bool win_manager::remove_theme (string name)
{
    hash_map <string, win_theme *>::iterator it = theme.find (name);
    if (it == theme.end ()) return false;

    delete it->second;
    theme.erase (it);
    return true; 
}

// return a pointer to a theme
win_theme * win_manager::get_theme (string name)
{
    hash_map <string, win_theme *>::iterator it = theme.find (name); 
    
    // try to load it if it's not in memory yet 
    if (it == theme.end ())
    {
        add_theme (name);
        return get_theme (name);
    }
    else return it->second; 
}

// load a font from disk
void win_manager::add_font (string name)
{
    font[name] = new win_font ((char *) name.c_str ());
}

// remove a font
bool win_manager::remove_font (string name)
{
    hash_map <string, win_font *>::iterator it = font.find (name);
    if (it == font.end ()) return false;

    delete it->second;
    font.erase (it);
    return true; 
}

// return a pointer to a font
win_font * win_manager::get_font (string name)
{
    hash_map <string, win_font *>::iterator it = font.find (name);
    
    // try to load the font if it's not in memory yet
    if (it == font.end ())
    {
        add_font (name);
        return get_font (name);
    }
    else return it->second; 
}
