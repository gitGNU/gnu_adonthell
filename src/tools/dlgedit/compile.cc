/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "../../types.h"
#include "dlgnode.h"
#include "compile.h"

dlg_compiler::dlg_compiler (vector<DlgNode*> &d, string f)
{
    dlg = d;
    filename = f;

    text_lookup = new u_int32[dlg.size ()];
    jump_lookup = new u_int32[dlg.size ()];
}

// Compile the Dialogue
void dlg_compiler::run ()
{
    script.open ((filename + ".py").c_str ());
    
    // write the strings used in the dialogue
    write_strings ();

    // write the dialogue functions
    write_dialogue ();
    
    // this creates the scripts "entry function"
    write_entry_func ();

    // write start
    write_start ();

    // write all the others
    while (!todo_nodes.empty ())
    {
        get_cur_nodes ();
        write_answer ();
    }
    
    script.close ();
}

// Write the strings used in the dialogue
void dlg_compiler::write_strings ()
{
    vector<DlgNode*>::iterator i;
    u_int32 j = 0;

    // write the class name
    script << "class " << strrchr (filename.c_str (), '/') + 1 << ":\n";
    script << "    strings = [";

    // write all strings and build lookup table to get index when given
    // the number of the node
    for (i = dlg.begin (); i != dlg.end (); i++)
        if ((*i)->type != LINK)
        {
            script << "\""<< ((Circle *)(*i))->text.c_str () << "\", ";
            text_lookup[(*i)->number] = j++;
        }

    // with a last, empty string we don't have to care about the final comma
    script << "\"\"]\n";

    return;
}

// Write the dialogue functions
void dlg_compiler::write_dialogue ()
{
    vector<DlgNode*>::iterator i;
    u_int32 j = 0;

    // write the dialogue functions
    script << "\n    def __init__(self):";
    script << "\n        self.dialogue = [self.start, ";

    // write answer-function-array and build lookup table to get index when given
    // the number of the node
    for (i = dlg.begin (); i != dlg.end (); i++)
        if ((*i)->type != LINK && npc_follows ((Circle*)*i))
        {
            script << "self.answer" << (*i)->number << ", ";
            jump_lookup[(*i)->number] = ++j;
        }
        else jump_lookup[(*i)->number] = 0;

    // with a last, empty string we don't have to care about the final comma
    script << "None]\n";

    return;
}

void dlg_compiler::write_entry_func ()
{
    // overwrite __getattr__, so that member variables need not be
    // defined befor using them
    script << "\n    def __getattr__ (self, var):"

#ifdef _DEBUG_
           << "\n        print \"*** Warning: \\\"\" + var + \"\\\" not defined!\""

#endif _DEBUG_
           << "\n        return 0\n"

    // Write the function to start/continue the dialogue
           << "\n    def run (self, answer):"
           << "\n        self.npc = []"
           << "\n        self.player = []"
           << "\n        self.cont = []"
           << "\n        self.dialogue[answer]()\n";
}

// Write a NPC part 
void dlg_compiler::write_npc (Circle *circle)
{
    u_int32 i = 0, j;
    char *space;

    script << "\n    def npc" << circle->number << " (self):";

    // write circle's condition (if any)
    if (circle->conditions != "") 
    {
        script << "\n        " << circle -> conditions;
        space = "            ";
    }
    else space = "        "; 

    // write circle's text and "jump target"
    script << "\n" << space << "self.npc.append (" << text_lookup[circle->number] << ")";
    script << "\n" << space << "self.cont.append (" << jump_lookup[circle->number] << ")";

    // write circle's additional code (if any)
    if (circle->variables != "")
    {
        circle->variables += '\n';
        
        while ((j = circle->variables.find ('\n', i)) < circle->variables.size ())
        {
            script << "\n" << space << circle->variables.substr (i,j);
            i = ++j;  
        }
    }

    script << "\n";
}

// Write a player part
void dlg_compiler::write_player (Circle *circle)
{
    char *space;

    script << "\n    def player" << circle->number << " (self):";

    // write circle's condition (if any)
    if (circle->conditions != "") 
    {
        script << "\n        " << circle -> conditions;
        space = "            ";
    }
    else space = "        "; 

    // write circle's text and "jump target"
    script << "\n" << space << "self.player.append (" << text_lookup[circle->number] << ")";
    script << "\n" << space << "self.cont.append (" << jump_lookup[circle->number] << ")";
    script << "\n";
}

void dlg_compiler::write_start ()
{
    vector<DlgNode*>::iterator i;

    for (i = dlg.begin(); i != dlg.end (); i++)
        if ((*i)->type != LINK)
        {
            if ((*i)->type == PLAYER) write_player ((Circle*)*i);
            else write_npc ((Circle*)*i);
        
            if ((*i)->prev.empty ())
                cur_nodes.push_back (*i);
        }

    script << "\n    def start (self):";
    write_answer ();

    return;
}

// all the NPC->nodes to add to the function are stored in cur_nodes
// at that moment
void dlg_compiler::write_answer ()
{
    vector<DlgNode*>::iterator i;
    char *space = "        ";

    // write npc node & followers (if any)
    for (i = cur_nodes.begin (); i != cur_nodes.end (); i++)
    {
#ifdef _DEBUG_
        if ((*i)->type != NPC) cout << "\n*** Compile error: NPC node expected!"; 
#endif _DEBUG_

        script << "\n" << space << "self.npc" << (*i)->number << " ()";

        // write following player nodes (if any)
        if (player_follows (*i))
            write_player_answer (*i);
            
        // if npc node(s) follow instead, add node for further actions
        if (npc_follows (*i))
            if (find (todo_nodes.begin(), todo_nodes.end (), *i) == todo_nodes.end () &&
                find (done_nodes.begin(), done_nodes.end (), *i) == done_nodes.end ())
                todo_nodes.push_back (*i);

        // add a delimiter to distinguish between player text's following
        // different NPC answers
        if (i + 1 != cur_nodes.end ())
            script << "\n" << space << "self.cont.append (-1)";
    }

    if (cur_nodes.empty ())
        script << "\n" << space << "pass";
    
    script << "\n";

    return;
}

// write the player's text following a npc node
void dlg_compiler::write_player_answer (DlgNode *npc)
{
    vector<DlgNode*>::iterator i;
    char *space = "        ";
    DlgNode *player;

    // first look at the direct links
    for (i = npc->next.begin (); i != npc->next.end (); i++)
    {
        // get the player node the arrow (*j) is pointing to
        player = (*i)->next[0];

        // instead of writing the function call, we could also call
        // writer_player (player) to create inline code instead
        script << "\n" << space << "self.player" << player->number << " ()";

        // if node isn't already handled or queued for handling,
        // add it to todo_nodes to have it handled later
        if (find (todo_nodes.begin(), todo_nodes.end (), player) == todo_nodes.end () &&
            find (done_nodes.begin(), done_nodes.end (), player) == done_nodes.end ())
            todo_nodes.push_back (player);
    }

    // now the same with the indirect links
    for (i = npc->link.begin (); i != npc->link.end (); i++)
    {
        player = (*i)->next[0];

        script << "\n" << space << "self.player" << player->number << " ()";

        if (find (todo_nodes.begin(), todo_nodes.end (), player) == todo_nodes.end () &&
            find (done_nodes.begin(), done_nodes.end (), player) == done_nodes.end ())
        todo_nodes.push_back (player);               
    }

    return;
}

// Get the nodes that make the next answer
void dlg_compiler::get_cur_nodes ()
{
    vector<DlgNode*>::iterator i;
    Circle *circle = (Circle *) todo_nodes.back ();
    char *space = "        ";
    u_int32 j = 0, k;

    cur_nodes.clear ();
    todo_nodes.pop_back ();
    done_nodes.push_back (circle);

    // For all following direct links (arrows) ...
    for (i = circle->next.begin (); i != circle->next.end (); i++)
        // If following NPC-node (circle) wasn't already compiled ...
        if (find (done_nodes.begin (), done_nodes.end (), (*i)->next[0]) == done_nodes.end ())
            // Add the NPC-node to the nodes to compile next
            cur_nodes.push_back ((*i)->next[0]);
        
    // Now the same for the indirect followers
    for (i = circle->link.begin (); i != circle->link.end (); i++)
        if (find (done_nodes.begin (), done_nodes.end (), (*i)->next[0]) == done_nodes.end ())
            cur_nodes.push_back ((*i)->next[0]);

    script << "\n    def answer" << circle->number << " (self):";

    // write player's additional code
    if (circle->type == PLAYER && circle->variables != "")
    {
        circle->variables += '\n';
    
        while ((k = circle->variables.find ('\n', j)) < circle->variables.size ())
        {
            script << "\n" << space << circle->variables.substr (j, k);
            j = ++k;  
        }
    }

    return;
}

// Check wether player text follows after that NPC node
u_int8 dlg_compiler::player_follows (DlgNode *circle)
{
    // Check immediate followers
    if (circle->next.size () > 0)
        if (circle->next[0]->next[0]->type == PLAYER)
            return 1;

    // Check linked followers
    if (circle->link.size () > 0)
        if (circle->link[0]->next[0]->type == PLAYER)
            return 1;

    return 0;
}

u_int8 dlg_compiler::npc_follows (DlgNode *circle)
{
    // Check immediate followers
    if (circle->next.size () > 0)
        if (circle->next[0]->next[0]->type == NPC)
            return 1;

    // Check linked followers
    if (circle->link.size () > 0)
        if (circle->link[0]->next[0]->type == NPC)
            return 1;

    return 0;
}
