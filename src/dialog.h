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

#include <vector>


class dlg_text                          // Contains a line of text & its attributes
{
public:
    dlg_text (u_int32 i, s_int32 o) : id (i), offset (o) { }
    dlg_text (dlg_text *d) : id (d->id), offset (d->offset) { }  
        
    u_int32 id;                         // Id of the string
    s_int32 offset;                     // offset to next command in dialogue
};


class dialog
{
  private:
	dialog();
public:
	PyObject *class_obj;                // Points to the instantiated dialogue class
	char *module;

    u_int32 speaker;                    // The current speaker 
    u_int32 answer;                     // The text chosen by the player
    char *text_file;                    // The file with the dialogues strings
    char *player_name;                  // The players name

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
    void insert_plugin ();              // 'Merges' a dialogue with the loaded one

    dialog *dlg;
};
#endif // _DLGEDIT_
#endif // __DIALOG_H__

