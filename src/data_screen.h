/*
   $Id$

   Copyright (C) 2001 by Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   data_screen.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the data_screen class.
 * 
 * 
 */



#ifndef DATA_SCREEN_H__
#define DATA_SCREEN_H__

#include "win_types.h"
#include "win_font.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_container.h"
#include "win_write.h"

/**
 * Whether the data screen should load or save games.
 * 
 */ 
enum
{
    LOAD_SCREEN = 0,
    SAVE_SCREEN = 1
};

/**
 * The gui for loading/saving games.
 * 
 */
class data_screen : public win_container
{
public:
    /** 
     * Constructor.
     * 
     * @param m LOAD_SCREEN or SAVE_SCREEN, depending on whether
     * you want to save or load a game.
     * 
     */
    data_screen (int m);

    /**
     * Destructor.
     * 
     */ 
    ~data_screen ();

    /**
     * React to input.
     * 
     */ 
    bool update ();

private:
    /** 
     * GUI initialisation.
     * 
     */
    void init ();

    /**
     * Callback for selecting a game.
     * 
     */ 
    void on_select (); 

    /**
     * Callback for entering a description.
     * 
     */ 
    void on_save ();

    /**
     * Write the thumbnail to disk.
     * 
     */
    void save_preview (string);

    win_font *font;                         // the font
    win_theme *theme;                       // the theme
    win_write *entry;                       // for entering a description of the game
    win_select *image_list;                 // list of thumbnails

    vector<win_write*> entry_list;          // list of game descriptions
    int mode;                               // Whether we're saving or loading
    bool quit;                              // Tells the window to close
};

#endif // DATA_SCREEN_H__
