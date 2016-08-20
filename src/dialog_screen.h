/*
   (C) Copyright 2000/2001 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   dialog_screen.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 *
 * @brief  Declares the dialog_screen class.
 *
 *
 */


#ifndef DLG_ENGINE_H__
#define DLG_ENGINE_H__

#include "dialog.h"
#include "window.h"
#include "win_select.h"
#include "win_theme.h"

/**
 * Maximum number of colors used during a dialog.
 *
 */
#define MAX_COLOR 6

/**
 * Allows the running of dialogues through a nice interface.
 *
 */
class dialog_screen : public win_container
{
public:

    /**
     * Constructor.
     *
     * @param mynpc npc the player is talking with.
     * @param dlg_file dialogue file to use.
     * @param size if 1, use a large window, else a small one.
     *
     */
    dialog_screen (character_base * mynpc, char * dlg_file, u_int8 size=1);

    /**
     * Destructor.
     *
     */
    ~dialog_screen ();

    /** 
     * Inits the dialogue engine (similar to a constructor call).
     * 
     * @param mynpc npc the player is talking with.
     * @param dlg_file dialogue file to use.
     * @param size if 1, use a large window, else a small one.
     * 
     */
    void init(character_base *mynpc, char * dlg_file, u_int8 size=1);

    /** 
     * Changes the displayed NPC portrait.
     * 
     * @param new_portrait file name of the new portrait image to set.
     */
    void set_portrait (const string & new_portrait);

    /** 
     * Changes the displayed NPC name.
     * 
     * @param char* new name of the npc.
     */
    void set_name (const string & new_name);

    /** 
     * Changes the whole NPC.
     * 
     * @param char* the name of the new npc to use.
     */
    void set_npc (const string & new_npc);

    /** 
     * React to (keyboard) input.
     *
     * @return true if the dialog is still running, false otherwise.
     */
    bool update ();

    /**
     * Execute one step of the dialogue. 
     * 
     */
    void run ();

#ifndef SWIG
private:
    /** 
     * 'Merges' a dialogue with the loaded one.
     * 
     */
    void insert_plugin ();

    /** 
     * Callback when item is "activated".
     * 
     */
    void on_select ();
    
    /**
     * Widget holding NPC portrait.
     * 
     */ 
    win_image *face;

    /**
     * Widget holding NPC name.
     * 
     */ 
    win_label *name;

    /**
     * Window theme.
     * 
     */ 
    win_theme *theme;
    
    /**
     * Selection of possible answers.
     * 
     */
    win_select *sel;
    
    /**
     * As long as realtime coloring does not work.
     * 
     */

    win_font *fonts[MAX_COLOR];
    
    /**
     * Answers currently available for selection.
     * 
     */
    vector <win_label*> cur_answers;

    /**
     * The Python/C interface for the dialogue.
     * 
     */ 
    dialog *dlg;

    /**
     * The selected dialogue option.
     * 
     */
    s_int32 answer; 

    /**
     * Index of first selectible dialogue item.
     * 
     */
    u_int32 sel_start;
    
    /**
     * True as long as we don't want to quit.
     * 
     */
    bool is_running;

    /**
     * Image to display next to the NPC text
     *
     */
    string portrait;
#endif // SWIG
};

#endif // DLG_ENGINE_H__
