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

#ifndef DIALOG_H__
#define DIALOG_H__


/**
 * @file   dialog.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the dialog class.
 * 
 * 
 */


#include "Python.h"
#include "types.h"
#include <vector>
 

using namespace std; 


/**
 * Lowlevel dialog class.
 * 
 */
class dialog
{
public:
    /** 
     * Default constructor.
     * 
     */
    dialog ();

    /** 
     * Destructor.
     * 
     */
    ~dialog ();

    /** 
     * Load and instanciate the dialog object.
     *
     * @param fpath file name of the dialog.
     * @param name ??
     *
     * @return true in case of success, false otherwise.
     */ 
    bool init (char* fpath, char* name);
    
    /**
     * Run the dialogue.
     *
     * @param index the index of the last answer.
     *
     */

    void run (u_int32 index);

    /** 
     * Returns the Python dialog instance.
     * 
     * 
     * @return the Python dialog instance.
     */
    PyObject *get_instance ();
    
    /** 
     * Returns the npc color.
     * 
     * 
     * @return the npc color.
     */
    u_int32 npc_color () { return _npc_color; }

    /** 
     * Returns the size of the text.
     * 
     * 
     * @return size of the text.
     */
    u_int32 text_size () { return _text_size; }

    /** 
     * Returns the dialog's texts.
     * 
     * 
     * @return array of strings containing the texts of the dialog.
     */
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

#endif // DIALOG_H__

