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
#include <vector>

// This is the lowlevel dialogue class 
class dialog
{
public:
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
    void scan_string (u_int32);     // Look for enclosed code and execute it
};

/*
#ifndef _DLGEDIT_                   // Don't need this for compiling dlgedit
class dialog_engine
{
public:
    dialog_engine ();
    void update_keyboard ();
    void update (window&);

private:
    void run (window&);
    void insert_plugin ();          // 'Merges' a dialogue with the loaded one

    dialog *dlg;
};
#endif // _DLGEDIT_
*/
#endif // __DIALOG_H__

