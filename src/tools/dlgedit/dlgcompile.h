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

// Dialogue Compiler
class dlg_compiler
{
public:
    dlg_compiler () { }
    dlg_compiler (vector<DlgNode*>&, string, string);

    void run ();                    // Start the compile-process

private:
    vector<DlgNode*> dlg;           // The input dialogue
    vector<DlgNode*> cur_nodes;     // Those dlg-nodes to compile right now
    vector<DlgNode*> todo_nodes;    // Those nodes to compile next
    vector<DlgNode*> done_nodes;    // Already compiled nodes
    
    string filename;                // The base dialogue filename
    string cust_func;               // The dialogue's custom functions
    string space;                   // For formatting the python output
    ofstream script;                // The script file

    u_int32 *text_lookup;           // tells String index when given node-number
    s_int32 *jump_lookup;           // tells Function index when given node-number
    
    void write_dialogue ();         // Write the array with the dialogue functions
    void write_npc (Circle*);       // Write NPC part of a script block
    void write_player (Circle*);    // Write Player part of a block
    void write_strings ();          // Write the string-file and creates import cmd
    void write_entry_func ();       // Write the class' entry function run()
    void write_custom_func ();      // Write the dialogue's user defined functions
    void get_cur_nodes ();          // Get the nodes to create the next block from
    void write_answer ();           // Write the answer function's NPC part
    void write_player_answer (DlgNode*);// Write the answer function's Player part
    void write_start ();            // Writes the first function

    u_int8 npc_follows (DlgNode*);   // TRUE if player node(s) follow Circle
    u_int8 player_follows (DlgNode*);// TRUE if player node(s) follow Circle
    u_int8 character_changed (Circle*);// TRUE if another NPC speaks

    void get_prev_npc_nodes (Circle*, vector<Circle*>&);
    void get_prev_npc_links (Circle*, vector<Circle*>&);
};

#endif // __COMPILE_H__
