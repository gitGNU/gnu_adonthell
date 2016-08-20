/*
   Copyright (C) 2001 by Kai Sterker <kai.sterker@gmail.com>
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
 * @file   data_screen.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the data_screen class.
 * 
 * 
 */



#ifndef DATA_SCREEN_H__
#define DATA_SCREEN_H__

#include "win_select.h"
#include "win_theme.h"
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

    /**
     * Returns the player's action
     *
     * @return
     *      @li true if a game has been loaded/saved
     *      @li false if the action has been cancelled
     */
    bool get_result ()
    {
        return !aborted;
    }
    
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

    win_font *font;                 // the font
    win_theme *theme;               // the theme
    win_write *entry;               // for entering a description of the game
    win_select *image_list;         // list of thumbnails

    vector<win_write*> entry_list;  // list of game descriptions
    int mode;                       // Whether we're saving or loading
    bool aborted;                   // Indicates whether action has been cancelled
    char gametime[20];              // time when saving the game
};

#endif // DATA_SCREEN_H__
