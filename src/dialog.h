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


using namespace std; 


// This is the lowlevel dialogue class 
class dialog
{
public:
    dialog ();                      // Init stuff to a sane state
    ~dialog ();                     // Clean up stuff

    bool init (char*, char*);       // Load & instantiate the dialogue object
    void run (u_int32);             // Run the dialogue 
    PyObject *get_instance ();      // Get the Python dialogue instance

    u_int32 npc_color () { return _npc_color; }
    u_int32 text_size () { return _text_size; }
    char** text () { return _text; }
    
private:
	PyObject *instance;             // Points to the instantiated dialogue class
    char **strings;                 // The dialogue text
    char **_text;                   // NPC's speech and according Player responses
    u_int32 _npc_color;             // The color of the NPC's text
    u_int32 _text_size;             // Number of strings in text

    vector<s_int32> answers;        // The indices with which to call instance.run () 
    vector<s_int32> choices;        // Strings player can chose from
    vector<s_int32> used;           // Dialogue parts that are already spoken

    void extract_strings ();        // Gets the dialogues text from 'instance'
    char* scan_string (const char*);// Look for enclosed code and execute it
    char* get_substr (const char*, char*, char*);
};

#endif // __DIALOG_H__

