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

#ifndef __DIALOG_H__
#define __DIALOG_H__

#ifndef SWIG

#include "Python.h"
#include "types.h"
#include "window.h"
#include "win_label.h"
#include "win_container.h"
#include "win_background.h"
#include "win_border.h"
#include "win_select.h"
#include "win_image.h"
#include "win_theme.h"
#include "game.h"
#include <vector>

// This is the lowlevel dialogue class 
class dialog
{
public:
    dialog ();                      // Init stuff to a sane state
    ~dialog ();                     // Clean up stuff

    bool init (char*, char*);       // Load & instantiate the dialogue object
    void run (u_int32);             // Run the dialogue 
    PyObject *get_instance ();      // Get the Python dialogue instance

    char **text;                    // NPC's speech and according Player responses
    u_int32 text_size;              // Number of strings in text

private:
	PyObject *instance;             // Points to the instantiated dialogue class

    char **strings;                 // The dialogue text

    vector<s_int32> answers;        // The indices with which to call instance.run () 
    vector<s_int32> choices;        // Strings player can chose from
    vector<s_int32> used;           // Dialogue parts that are already spoken

    void extract_strings ();        // Gets the dialogues text from 'instance'
    char* scan_string (const char*);// Look for enclosed code and execute it
    char* get_substr (const char*, char*, char*);
};

class dialog_engine : public game_engine
#else
class dialog_engine
#endif // SWIG
{
public:
    void set_portrait (char*);      // Changes the displayed NPC portrait
    void set_name (char*);          // Changes the diaplayed NPC name
    void set_color (u_int32);       // Changes the NPC's text-color
    void set_npc (char*);           // Changes the whole NPC

    dialog_engine (const char*);    // Constructor
#ifndef SWIG
    ~dialog_engine ();              // Destructor
    
    void realtime_tasks ();         // Stuff done really often
    void gametime_tasks ();         // Stuff done once per frame
    
    void update_keyboard ();        // React to (keyboard) input
    void run ();                    // Execute one step of the dialogue

private:
    void insert_plugin ();          // 'Merges' a dialogue with the loaded one

    win_font *font;                 // The font
    win_container *wnd;             // The dialogue window
    win_container *txt;             // Container with the dialogue text
    win_image *face;                // Widget holding NPC portrait
    win_label *name;                // Widget holding NPC name
    win_theme *th;                      // Window theme
    win_select *sel;                // Selection of possible answers
    //    win_cursor *cursor;             // Cursor for selection
    image *portrait;                // The NPC's portrait

    win_label * question;             // Label for the question
    vector <win_label*> cur_answers;// Answers currently available for selection

    PyObject* instance;             // Dialogue Engine Python wrapper class
    dialog *dlg;                    // The Python/C interface for the dialogue
    game_engine *engine;            // A backup of the previously active engine
    int answer;                     // The selected dialogue option
#endif // SWIG
};
#endif // __DIALOG_H__

