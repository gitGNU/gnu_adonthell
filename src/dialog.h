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

#include "Python.h"
#include "types.h"
#include "win_label.h"
#include "win_container.h"
#include "win_border.h"
#include "win_select.h"
#include "win_image.h"
#include "mapcharacter.h"
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

    char **text;                    // NPC's speech and according Player responses
    u_int32 text_size;              // Number of strings in text

private:
	PyObject *instance;             // Points to the instantiated dialogue class

    char **strings;                 // The dialogue text

    vector<u_int32> answers;        // The indices with which to call instance.run () 
    vector<s_int32> choices;        // Strings player can chose from
    vector<s_int32> used;           // Dialogue parts that are already spoken

    void extract_strings ();        // Gets the dialogues text from 'instance'
    char* scan_string (const char*);// Look for enclosed code and execute it
    char* get_substr (const char*, char*, char*);
};

class dialog_engine : public game_engine
{
public:
    dialog_engine (mapcharacter *, game_engine *);
    ~dialog_engine ();
    
    void realtime_tasks ();
    void gametime_tasks ();
    
    void update_keyboard ();
    void update ();

private:
    void run ();
    void insert_plugin ();          // 'Merges' a dialogue with the loaded one

    win_font *font;
    win_container *wnd;
    win_container *txt;
    win_image *face;
    win_label *name;
    win_border *border;
    win_background *back;
    
    dialog *dlg;
    game_engine *engine;
    int answer;
};
#endif // __DIALOG_H__

