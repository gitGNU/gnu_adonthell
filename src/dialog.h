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


#include "python_class.h"
#include "py_object.h"
#include "types.h"
#include <vector>
 

using namespace std; 


/**
 * The lowlevel dialog class. It is the link between Python dialogue
 * scripts and the \link dialog_screen dialogue GUI \endlink . As such
 * it is responsible for loading dialogue scripts and for stepping through
 * the dialogue according to the player's %input and the current state
 * of the %game. After each step, the resulting dialogue %text is available
 * for display through the GUI.
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
     * @param fpath full path to the dialogue.
     * @param name name of the dialogue class.
     * @param args arguments to pass to the dialogue class
     *
     * @return \e true in case of success, \e false otherwise.
     * @sa reload()
     */ 
    bool init (string fpath, string name, PyObject *args);
    
    /**
     * This method is similar to init. But unlike init, it will
     * correctly handle dialogues that have changed on disk since
     * they were first imported. This function can safely be called
     * several times, although the dialogue will be reset each time.
     *
     * @param fpath full path to the dialogue.
     * @param name name of the dialogue class.
     * @param args arguments to pass to the dialogue class
     *
     * @return \e true in case of success, \e false otherwise.
     * @sa init()
     */
    bool reload (string fpath, string name, PyObject *args);
    
    /**
     * Run the dialogue. Executes one step of the conversation.
     * Afterwards the NPC's speech and possible reactions of the
     * player can be retrieved via the text() method.
     *
     * @param index the index of the chosen alternative from the 
     *        previous list of %text.
     */
    void run (u_int32 index);

    /** 
     * Returns the Python dialog instance.
     * 
     * 
     * @return the Python dialog instance.
     */
    PyObject *get_instance ()
    {
        return dialogue.get_instance ();
    }

    /** 
     * Returns the color to be used for displaying the NPC's speech.
     * 
     * 
     * @return the npc color.
     */
    u_int32 npc_color () { return _npc_color; }

    /** 
     * Returns the number of %text lines available at this point of 
     * the dialoge.
     * 
     * @return the number of available dialogue texts. 0 if the 
     *         dialogue is finished.
     *
     * @sa text()
     */
    u_int32 text_size () { return _text_size; }

    /** 
     * Returns the dialogue's %text. Depending on the current state
     * of the dialogue, there can be multiple alternatives. The first
     * string is always the NPC's speech. Any following strings are
     * the player's possible reactions. The value passed to the run()
     * method is the (zero-based) index of the alternative chosen by
     * the player.
     *
     * 
     * @return array of strings containing the %text of the dialog or
     *         \e NULL when the dialogue is finished.
     * @sa text_size()
     */
    char** text () { return _text; }
    
private:
    py_object dialogue;             // Points to the instantiated dialogue class
    char **strings;                 // The dialogue text
    char **_text;                   // NPC's speech and according Player responses
    u_int32 _npc_color;             // The color of the NPC's text
    u_int32 _text_size;             // Number of strings in text

    vector<s_int32> answers;        // The indices with which to call instance.run () 
    vector<s_int32> choices;        // Strings player can chose from
    vector<s_int32> used;           // Dialogue parts that are already spoken

    void clear ();                  // Cleanup
    bool setup ();                  // Further dialogue initialisation
    char* scan_string (const char*);// Look for enclosed code and execute it
    char* get_substr (const char*, char*, char*); 
};

#endif // DIALOG_H__

