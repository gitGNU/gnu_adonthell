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

#include <vector>

class dialog
{
public:

    static s_int32 *offset;
    static s_int32 *length;

    struct text                         // Contains a line of text & its attributes
    {
        u_int32 id;                     // Id of the string
        s_int32 offset;                 // offset to next command in dialogue
    };
    
    u_int32 answer;                     // The text chosen by the player
    char *text_file;                    // The file with the dialogues strings

    vector<text*> npc_text;             // NPC´s part
    vector<text*> player_text;          // Player´s part
    vector<u_int32> used_text;          // Text already spoken -> don´t display again

    vector<char*> strings;              // Text data
};

#ifndef _DLGEDIT_                       // Don't need this for compiling dlgedit
class dialog_engine
{
public:
    dialog_engine ();
    void update_keyboard ();
    void update (window&);

private:
    void run (window&);

    dialog *dlg;
    interpreter *engine;
};
#endif // _DLGEDIT_
#endif // __DIALOG_H__

