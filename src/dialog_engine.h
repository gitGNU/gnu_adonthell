/*
   $Id$

   (C) Copyright 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   dialog_engine.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the dialog_engine class.
 * 
 * 
 */ 


#ifndef DLG_ENGINE_H__
#define DLG_ENGINE_H__

#include "dialog.h"
#include "character_base.h"
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
class dialog_engine : public win_container
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
    dialog_engine (character_base * mynpc, char * dlg_file, u_int8 size=1);

    /**
     * Destructor.
     * 
     */ 
    ~dialog_engine ();

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
    void set_name (char* new_name);

    /** 
     * Changes the whole NPC.
     * 
     * @param char* the name of the new npc to use.
     */
    void set_npc (char* new_npc);

    /** 
     * React to (keyboard) input.
     * 
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
     * Dialogue Engine Python wrapper class.
     * 
     */
    PyObject* instance;

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
#endif // SWIG
};

#endif // DLG_ENGINE_H__
