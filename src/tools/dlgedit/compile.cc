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
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../commands.h"
#include "dlgnode.h"
#include "main.h"
#include "compile.h"

void 
make_dialogue (MainFrame * wnd)
{
/*
    DlgCompiler *comp = (DlgCompiler *) malloc (sizeof (DlgCompiler));

    // some initialization 
    init_lookup_tables (comp, wnd->nodes);

    // All the nodes for the first block 
    get_start_nodes (comp, wnd->nodes);

    // output the dialogues text 
    write_text (comp, wnd->nodes, wnd->file_name);

    // The "compiling" consists of 2 steps:  
       1. Create the blocks described in compile.h 
    compile_nodes (comp);

    // 2. Make the links between the different blocks 
    connect_blocks (comp);

    // output the compiled dialogue 
    write_dialogue (comp, wnd->file_name);

    // recreate the original state of all nodes 
    undo_changes (wnd->nodes);

    // activate Run - Menuitem 
    gtk_widget_set_sensitive (wnd->dialogue_run, TRUE);

    free (comp);
*/
}

dlg_compiler::dlg_compiler (vector<DlgNode*> &d, string f)
{
    dlg (d);
    filename (f);

    text_lookup = new u_int32[dlg.size ()];
}

// Compile the Dialogue
void dlg_compiler::run ()
{
    // Write the Dialogue's text
    write_text ();
}

// Write the Dialogue's text
void dlg_compiler::write_text ()
{
    // We use a 'FILE*' here instead of a 'ofstream', as it makes writing 
    // binary data a lot easier (at least I think so ;) )
    FILE *out;
    Circle *node;

    command *cmd;               // The Import-command
    
    u_int32 *offset;            // Those two make the Table of
    u_int32 *length;            // Contents of the string file
    u_int32 *text;              // Tells the Import-Command which strings to load

    u_int32 cur_offset;         // Those three are 
    u_int32 index = 0;          // just some temporary
    s_int32 i;                  // variables

    // Count number of Circles 
    for (i = 0; i < dlg.size (); i++)
        if (dlg[i]->type != LINK)
            index++;

    cur_offset = 4 + 8 * index;
    offset = new u_int32[index];
    length = new u_int32[index];
    index = 0;

    // Add IMPORT command; init offset & length 
    for (i = 0; i < dlg.size (); i++)
    {
        node = (Circle *) dlg[i];
        if (node->type != LINK)
        {
            text_lookup[node->number] = index;
            text[index] = index;
            offset[index] = cur_offset;
            length[index] = strlen (node->text);

            cur_offset += length[index];
            index++;
        }
    }

    // Make Import command and add it to script
    cmd = new import_cmd (text, index);
    code.push_back (cmd);
    
    // write string - file 
    out = fopen (g_strjoin (NULL, filename.c_str (), ".str", NULL), "wb");

    fwrite (&index, sizeof (index), 1, out);
    fwrite (offset, sizeof (offset[0]), index, out);
    fwrite (length, sizeof (length[0]), index, out);

    for (i = 0; i < dlg.size (); i++)
    {
        node = (Circle *) dlg[i];
        if (node->type != LINK)
            fputs (node->text, out);
    }

    // Clean up (DON'T delete 'text'! It's still needed!)
    fclose (out);
    delete[] offset;
    delete[] length;    
}