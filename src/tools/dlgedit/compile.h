/*
   $Id$
   
   Copyright (C) 1999,2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __COMPILE_H__
#define __COMPILE_H__

#include <vector>
#include <string>

#include "../../interpreter.h" 

// Dialogue Compiler
class dlg_compiler
{
public:
    dlg_compiler () { }
    dlg_compiler (vector<DlgNode*>&, string);

    void run ();                    // Start the compile-process

private:
    vector<command*> code;          // The compiled script
    vector<DlgNode*> dlg;           // The input dialogue
    string filename;                // The base dialogue filename

    u_int32 *text_lookup;           // 

    void write_text ();             // Write the Dialogues text
};

void make_dialogue (MainFrame *);

#endif // __COMPILE_H__