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

#include "../../types.h"
#include "../../commands.h"
#include "../../dialog_cmds.h"
#include "../../generic_cmds.h"
#include "dlgnode.h"
#include "compile.h"

dlg_compiler::dlg_compiler (vector<DlgNode*> &d, string f)
{
    dlg = d;
    filename = f;

    text_lookup = new u_int32[dlg.size ()];
}

// Compile the Dialogue
void dlg_compiler::run ()
{
    // Write the Dialogue's text
    write_import ();

    get_cur_nodes ();

    // Write the rest
    while (!cur_nodes.empty ())
    {
       write_npc ();
       get_cur_nodes ();
    }

    // output everything
    output_script ();
}

// Write the Dialogue's text
void dlg_compiler::write_import ()
{
    // We use a 'FILE*' here instead of a 'ofstream', as it makes writing 
    // binary data a lot easier (at least I think so ;) )
    FILE* str = NULL;
    Circle *node;
    string sf = filename + ".str";

    command *cmd;               // The Import-command
    
    u_int32 *offset;            // Those two make the Table of
    u_int32 *length;            // Contents of the string file
    u_int32 *text;              // Tells the Import-Command which strings to load

    u_int32 cur_offset;         // Those three are 
    u_int32 index = 0;          // just some temporary
    u_int32 i;                  // variables

    // Count number of Circles 
    for (i = 0; i < dlg.size (); i++)
        if (dlg[i]->type != LINK)
            index++;

    cur_offset = 4 + 8 * index;
    offset = new u_int32[index];
    length = new u_int32[index];
    text = new u_int32[dlg.size ()];
    index = 0;

    // Add IMPORT command; init offset & length 
    for (i = 0; i < dlg.size (); i++)
    {
        node = (Circle *) dlg[i];
        text[i] = i;

        if (node->type != LINK)
        {
            text_lookup[node->number] = index;
            offset[index] = cur_offset;
            length[index] = node->text.length ();

            cur_offset += length[index];
            index++;
        }
    }

    // Make Import command and add it to script
    cmd = new import_cmd (text, index);
    code.push_back (cmd);
    
    // write string - file 
    str = fopen (sf.c_str (), "wb");
    if (!str) 
    {
        printf ("\n\nError opening file %s!\n", sf.c_str ());
        return;
    }
    
    fwrite (&index, sizeof (index), 1, str);
    fwrite (offset, sizeof (offset[0]), index, str);
    fwrite (length, sizeof (length[0]), index, str);

    for (i = 0; i < dlg.size (); i++)
    {
        node = (Circle *) dlg[i];
        if (node->type != LINK)
            fputs (node->text.c_str (), str);
    }

    // Clean up (DON'T delete 'text'! It's still needed!)
    fclose (str);
    delete[] offset;
    delete[] length;    
}

// Write the "NPC part" of the current block:  
// SPEAKER
// CONDITON
// TEXT
// VARIABLES
//   Player Part
// SPEAKER 
// ...
// DISPLAY
void dlg_compiler::write_npc ()
{
    u_int32 i;
    u_int32 pos;
    branch_cmd *cmd;
    cmp_data *data;

    // Look wether multiple NPC nodes with multiple parents exist
    // --> handle differently

    // All possible NPC-texts of this block are stored in cur_nodes
    for (i = 0; i < cur_nodes.size (); i++)
    {
        // cur_crcle is the Node all subsequent functions work on
        cur_crcle = (Circle *) cur_nodes[i];

        // Set the NPC that will speak
        write_speaker ();

        // Here's the line of the script that preceeding nodes must link to
        data = new cmp_data (cur_crcle, NULL, code.size ());

        // write the condition and memorize the command so we can set the
        // branch target later in this function
        if (cur_crcle->conditions != "") 
        {
            write_condition ();
            cmd = (branch_cmd *) code.back ();
            pos = code.size ();
        }

        // Write the text that will be spoken by the NPC
        write_text ();

        // Here is the command whose target we have to set later on
        data->cmd = code.back ();
        
        // Do any operation on (gamestate) variables before we display the
        // players text, because player-conditions may depend on those 
        // new values already
        if (cur_crcle->variables != "") write_variables (); 

        // If player-text follows, write that immediately
        if (ptext_follows ())
        {
            write_player ();

            // furtermore we're done with that NPC node
            done_nodes.push_back (data);

            // Have to reinit cur_crcle as it was changed in write_player ()
            cur_crcle = (Circle *) cur_nodes[i];
        }
        // Else if dialogue ends afterwards        
        else if (end_follows())
        {
            // Node is done already ...
            done_nodes.push_back (data);
            // ... but we also have to write the end
            todo_nodes.push_back (data);
        }
        // Else we add this one to todo_nodes to have it handled later
        else todo_nodes.push_back (data);

        // if there was a condition, this is the line we have to jump to is it
        // isn't met 
        if (cur_crcle->conditions != "") cmd->setjmp (code.size () - pos);
    }

    // That tells the dialoge engine to update the conversation with the
    // new text of NPC and player
    write_display ();

    // Reinit the dialogue engine for the next block
    write_clear ();
}

// Write the "Player Part" of the current block:
// SPEAKER
// CONDITION
// TEXT
// CONDITION
// ...
void dlg_compiler::write_player ()
{
    Circle *npc = cur_crcle;
    u_int32 i;
    branch_cmd *cmd;
    cmp_data *data;

    // Set the cur_circle to a PLAYER-node to write the correct speaker
    if (!npc->next.empty ()) cur_crcle = (Circle *) npc->next[0]->next[0];
    else cur_crcle = (Circle *) npc->link[0]->next[0];
    
    write_speaker ();

    // First handle all the direct followers
    for (i = 0; i < npc->next.size (); i++)
    {
        cur_crcle = (Circle *) npc->next[i]->next[0];

        // write the condition and branch after the text command
        if (cur_crcle->conditions != "") 
        {
            write_condition ();
            cmd = (branch_cmd *) code.back ();
            cmd->setjmp (1);
        }

        write_text ();

        // Here's the line of the script that preceeding nodes must link to
        data = new cmp_data (cur_crcle, code.back (), code.size ()-1);
        todo_nodes.push_back (data);
    }

    // Now the same with the links
    for (i = 0; i < npc->link.size (); i++)
    {
        cur_crcle = (Circle *) npc->link[i]->next[0];

        if (cur_crcle->conditions != "") 
        {
            write_condition ();
            cmd = (branch_cmd *) code.back ();
            cmd->setjmp (1);
        }

        write_text ();

        data = new cmp_data (cur_crcle, code.back (), code.size ()-1);
        todo_nodes.push_back (data);
    }
}

void dlg_compiler::write_speaker ()
{
    command *cmd = new speaker_cmd (cur_crcle->character, cur_crcle->mood);
    code.push_back (cmd);
}

void dlg_compiler::write_condition ()
{
    string error;

    // This compiles the condition-code and adds it to the end of script
    cond_compile (cur_crcle->conditions.c_str (), error, code);
}

void dlg_compiler::write_text ()
{
    text_cmd *cmd = new text_cmd (text_lookup[cur_crcle->number], 0);
    code.push_back (cmd);
}

void dlg_compiler::write_variables ()
{
    string error;

    // This compiles the condition-code and adds it to the end of script
    vars_compile (cur_crcle->variables.c_str (), error, code);
}

void dlg_compiler::write_display ()
{
    return_cmd *cmd = new return_cmd (1);
    code.push_back (cmd);    
}

void dlg_compiler::write_clear ()
{
    clear_cmd *cmd = new clear_cmd;
    code.push_back (cmd);
}

void dlg_compiler::write_end ()
{
    return_cmd *cmd = new return_cmd (0);
    code.push_back (cmd);    
}

void dlg_compiler::output_script ()
{
    u_int32 i;
    string af = filename + ".txt";
    string sf = filename + ".dlg";
    
    ofstream ascii (af.c_str ());
    FILE* script = fopen (sf.c_str (), "wb");

    i = code.size ();
    fwrite (&i, sizeof (i), 1, script);

    for (i = 0; i < code.size (); i++)
    {
        if (code[i]->type == CLEAR) ascii << "\n";
        
        ascii.width (3);
        ascii << i << "  ";

        code[i]->write (script);
        code[i]->ascii (ascii);

        ascii << "\n";
    }

    fclose (script);
    ascii.close ();
}

// Fill the cur_nodes array with the NPC-nodes which will be used to create
// the next block
void dlg_compiler::get_cur_nodes ()
{
    u_int32 i;
    cmp_data *data;

    // Make sure all the old NPC nodes are removed
    cur_nodes.clear ();

    // On startup, we've got to find all nodes without a father   
    if (todo_nodes.empty () && done_nodes.empty ())
    {
        for (i = 0; i < dlg.size (); i++)
            if (dlg[i]->prev.empty ())
                cur_nodes.push_back (dlg[i]);

        return;
    }

    // In case only todo_nodes is empty, we're finished
    if (todo_nodes.empty ()) return;

    // Here we can take one of the todo_nodes and continue with it
    data = todo_nodes[0];

    // First, we have to check wether the childs of this (player-) node
    // have already been compiled. If so, we can update the command with
    // the proper jump target. (That's done in the isdone(...) function)
    // Else we can safely add the child to the cur_nodes 

    // The chosen command continues in the next line
    ((text_cmd *) data->cmd)->setjmp (code.size () - data->line);

    // For all following direct links (arrows) ...
    for (i = 0; i < data->node->next.size (); i++)
        // If following NPC-node (circle) wasn't already compiled ...
        if (!isdone (data->node->next[i]->next[0], data))
            // Add the NPC-node to the nodes to compile next
            cur_nodes.push_back (data->node->next[i]->next[0]);

    // Now the same with the indirect links
    for (i = 0; i < data->node->link.size (); i++)
        if (!isdone (data->node->link[i]->next[0], data))
            cur_nodes.push_back (data->node->link[i]->next[0]);

    // What followa here isn't too good -> correct
    // The End of dialogue follows:
    if (cur_nodes.empty ())
    {
        cur_crcle = (Circle *) data->node;
        if (end_follows ()) write_end ();
        
        done_nodes.push_back (data);
        if (todo_nodes.size () > 1) todo_nodes.erase (todo_nodes.begin ());
        else todo_nodes.clear ();

        get_cur_nodes ();
    }

    // Move the node from the todo- to the done_nodes    
    done_nodes.push_back (data);

    // If vector::size () == 1 then vector::erase(vector::begin()) will segfault
    if (todo_nodes.size () > 1) todo_nodes.erase (todo_nodes.begin ());
    else todo_nodes.clear ();
}

// Check wether node was already compiled and set the proper link
u_int8 dlg_compiler::isdone (DlgNode *node, cmp_data *data)
{
    u_int32 i;

    for (i = 0; i < done_nodes.size (); i++)
        if (done_nodes[i]->node == node)
        {
            ((text_cmd *) data->cmd)->setjmp (done_nodes[i]->line - data->line - 1);
            return 1;
        }

    return 0;
}

// Check wether player text follows after that NPC node
u_int8 dlg_compiler::ptext_follows ()
{
    // Check immediate followers
    if (cur_crcle->next.size () > 0)
        if (cur_crcle->next[0]->next[0]->type == PLAYER)
            return 1;

    // Check linked followers
    if (cur_crcle->link.size () > 0)
        if (cur_crcle->link[0]->next[0]->type == PLAYER)
            return 1;

    return 0;
}

u_int8 dlg_compiler::end_follows ()
{
    return (cur_crcle->next.empty () && cur_crcle->link.empty ());
}