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

extern int cond_compile (const char*, string&, vector<command*>&);
extern int vars_compile (const char*, string&, vector<command*>&);

// Data used to link the different blocks together
class cmp_data
{
public:
    cmp_data () { }
    cmp_data (DlgNode *n, command *c, u_int32 l) : node(n), cmd(c), line(l) { }

    DlgNode *node;                  // The original dialogue node
    command *cmd;                   // The command representing this node
    u_int32 line;                   // The commands position in the script
};

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
    vector<DlgNode*> cur_nodes;     // Those dlg-nodes to compile right now
    vector<cmp_data*> todo_nodes;   // Those nodes to compile next
    vector<cmp_data*> done_nodes;   // Already compiled nodes
    
    string filename;                // The base dialogue filename

    u_int32 *text_lookup;           // tells Text ID when given node-number
    
    Circle *cur_crcle;              // Circle all the write... functions work on

    void write_npc ();              // Write NPC part of a script block
    void write_player ();           // Write Player part of a block
    void write_import ();           // Write the string-file and creates import cmd
    void write_speaker ();          // Set the current speaker
    void write_condition ();        // Writes a node's condition
    void write_variables ();        // Write a node's operations on gamestates
    void write_text ();             // Writes a node's text
    void write_display ();          // Tells the interpreter to show the new text
    void write_end ();              // Tells the interpreter to quit
    void write_clear ();            // Continues the interpreter after players choice
    
    void output_script ();          // Write the compiled script to disk
    void get_cur_nodes ();          // Get the nodes to create the next block from

    u_int8 isdone (DlgNode*, cmp_data*); // Was DlgNode already compiled?
    u_int8 ptext_follows ();        // TRUE if player nodes follow cur_crcle
    u_int8 end_follows ();          // TRUE if dialogue's end follows cur_crcle
};

#endif // __COMPILE_H__