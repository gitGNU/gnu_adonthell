/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef __DLG_ENGINE_H__
#define __DLG_ENGINE_H__

#include "dialog.h"
#include "character_base.h"
#include "window.h"
#include "win_select.h"
#include "win_theme.h"

#define MAX_COLOR 6

class dialog_engine : public win_container
{
public:
    void set_portrait (char*);      // Changes the displayed NPC portrait
    void set_name (char*);          // Changes the displayed NPC name
    void set_npc (char*);           // Changes the whole NPC

    dialog_engine (character_base * mynpc, char * dlg_file, u_int8 size=1); // Constructor
    void init(character_base *mynpc, char * dlg_file, u_int8 size=1);
    ~dialog_engine ();              // Destructor
    
    bool update ();                 // React to (keyboard) input
    void run ();                    // Execute one step of the dialogue

#ifndef SWIG
private:
    void insert_plugin ();          // 'Merges' a dialogue with the loaded one
    void on_select ();              // Callback when item is "activated"
    
    win_image *face;                // Widget holding NPC portrait
    win_label *name;                // Widget holding NPC name
    win_theme *theme;               // Window theme
    win_select *sel;                // Selection of possible answers

    win_font *fonts[MAX_COLOR];     // As long as realtime coloring does not work
    
    vector <win_label*> cur_answers;// Answers currently available for selection

    PyObject* instance;             // Dialogue Engine Python wrapper class
    dialog *dlg;                    // The Python/C interface for the dialogue

    s_int32 answer;                 // The selected dialogue option
    u_int32 sel_start;              // Index of first selectible dialogue item
    
    bool is_running;                // True as long as we don't want to quit
#endif // SWIG
};

#endif // __DLG_ENGINE_H__
