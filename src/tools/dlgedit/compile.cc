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

// Private functions 
void get_start_nodes (DlgCompiler *, ptr_list *);
void init_lookup_tables (DlgCompiler *, ptr_list *);

void compile_nodes (DlgCompiler *);
void connect_blocks (DlgCompiler *);

DlgNode *get_next_circle (DlgNode * node, u_int32);

void write_dialogue (DlgCompiler *, gchar *);
void write_text (DlgCompiler *, ptr_list *, gchar *);

void undo_changes (ptr_list *);

void add_CLEAR (DlgCompiler *, GPtrArray *);
void add_RETURN (DlgCompiler *, GPtrArray *, u_int32);
void add_NPCTEXT (DlgCompiler *, GPtrArray *, s_int32);
void add_IMPORT (DlgCompiler *, GPtrArray *, s_int32, s_int32);
void add_PTEXT (DlgCompiler *, GPtrArray *, s_int32);
void add_SNPCTEXT (DlgCompiler *, GPtrArray *, s_int32);

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

// Get all Start-nodes of Dialogue 
void 
get_start_nodes (DlgCompiler * comp, ptr_list * nodes)
{
/*
    DlgNode *node;
    s_int32 i;

    for (i = 0; i < nodes->size; i++)
    {
        node = (DlgNode *) get_ptr_list_element (nodes, i);

        if (node->prev->len == 0)
            g_ptr_array_add (comp->next_circles, node);
    }
*/
}

void 
undo_changes (ptr_list * nodes)
{
/*
    DlgNode *node;
    s_int32 i;
    u_int32 j;

    for (i = 0; i < nodes->size; i++)
    {
        node = (DlgNode *) get_ptr_list_element (nodes, i);

        for (j = 0; j < node->link->len; j++)
            g_ptr_array_remove (node->next, g_ptr_array_index (node->link, j));
    }
*/
}


void 
init_lookup_tables (DlgCompiler * comp, ptr_list * nodes)
{
/*
    DlgNode *node;
    s_int32 i;
    u_int32 j;

    // Inde of text in compiled dialogue 
    comp->text_number = (u_int32 *) malloc (sizeof (u_int32) * nodes->size);
    comp->pc_lookup = (s_int32 *) malloc (sizeof (s_int32) * nodes->size);
    comp->link_table = (s_int32 *) malloc (sizeof (s_int32) * nodes->size);
    comp->pos_table = (s_int32 *) malloc (sizeof (s_int32) * nodes->size);

    comp->dlg_length = 0;
    comp->nodes = nodes;
    comp->next_circles = g_ptr_array_new ();
    comp->compiled_blocks = g_ptr_array_new ();

    for (i = 0; i < nodes->size; i++)
    {
        comp->link_table[i] = -1;
        comp->pos_table[i] = -1;

        node = (DlgNode *) get_ptr_list_element (nodes, i);

        for (j = 0; j < node->link->len; j++)
            g_ptr_array_add (node->next, g_ptr_array_index (node->link, j));
    }
*/
}

// add IMPORT commands to start of Dialogue 
void 
write_text (DlgCompiler * comp, ptr_list * nodes, gchar * file_name)
{
/*
    GPtrArray *imports = g_ptr_array_new ();
    DlgNode *node;
    FILE *out;
    u_int32 *offset;
    u_int32 *length;
    u_int32 cur_offset;
    u_int32 index = 0;
    s_int32 i;

    // Count number of Circles 
    for (i = 0; i < nodes->size; i++)
    {
        node = (DlgNode *) get_ptr_list_element (nodes, i);
        if (node->type != LINK)
            index++;
    }

    cur_offset = 4 + 8 * index;
    offset = (u_int32 *) malloc (sizeof (u_int32) * index);
    length = (u_int32 *) malloc (sizeof (u_int32) * index);
    index = 0;

    // Add IMPORT commands; init offset & length 
    for (i = 0; i < nodes->size; i++)
    {
        node = (DlgNode *) get_ptr_list_element (nodes, i);
        if (node->type != LINK)
        {
            add_IMPORT (comp, imports, index, node->number);
            comp->text_number[node->number] = index;

            offset[index] = cur_offset;
            length[index] = strlen (node->text);

            cur_offset += length[index];
            index++;
        }
    }

    // Make Import -> Block first part of compiled blocks 
    g_ptr_array_add (comp->compiled_blocks, imports);
    comp->pc_lookup[0] = index;

    // write string - file 
    out = fopen (g_strjoin (NULL, file_name, ".str", NULL), "wb");

    fwrite (&index, sizeof (index), 1, out);
    fwrite (offset, sizeof (offset[0]), index, out);
    fwrite (length, sizeof (length[0]), index, out);

    for (i = 0; i < nodes->size; i++)
    {
        node = (DlgNode *) get_ptr_list_element (nodes, i);
        if (node->type != LINK)
            fputs (node->text, out);
    }

    fclose (out);
    free (offset);
    free (length);
*/
}

// Actual "compiling": create a command block from each circle 
void 
compile_nodes (DlgCompiler * comp)
{
/*
    GPtrArray *command_block = g_ptr_array_new ();
    DlgNode *circle;
    dialog_cmd *cmd;
    u_int32 i, j, len, num, retval = 2;

    // Look if we reached a end 
    if (comp->next_circles->len == 0)
    {
        add_RETURN (comp, command_block, 0);

        g_ptr_array_add (comp->compiled_blocks, command_block);

        // Remember positions of block - starts for later use 
        comp->pc_lookup[comp->compiled_blocks->len - 1] =
            comp->pc_lookup[comp->compiled_blocks->len - 2] + command_block->len;
    }

    else
    {
        // add circles 
           There are 3 possibilities: Either PLAYER - nodes follow, further
           NPC - nodes follow or END follows 
        for (i = 0; i < comp->next_circles->len; i++)
        {
            circle = (DlgNode *) g_ptr_array_index (comp->next_circles, i);

            // get number of circles followers 
            len = circle->next->len;

            // 1. End follows 
            if (len == 0)
            {
                // // add this node to link table 
                comp->pos_table[circle->number] = comp->compiled_blocks->len - 1;

                add_SNPCTEXT (comp, command_block, circle->number);
            }
            else
            {
                // 2. PLAYER - nodes follow 
                if ((get_next_circle (circle, 0))->type == PLAYER)
                {
                    // add this node to link table 
                    comp->pos_table[circle->number] = comp->compiled_blocks->len - 1;

                    add_NPCTEXT (comp, command_block, circle->number);

                    for (j = 0; j < len; j++)
                        add_PTEXT (comp, command_block, (get_next_circle (circle, j))->number);

                    retval = 1;
                }

                // 3. NPC - nodes follow 
                if ((get_next_circle (circle, 0))->type == NPC)
                    add_SNPCTEXT (comp, command_block, circle->number);
            }
        }

        // add DISPLAY 
        add_RETURN (comp, command_block, retval);

        // add CLEAR 
        add_CLEAR (comp, command_block);

        g_ptr_array_add (comp->compiled_blocks, command_block);

        // Remember positions of block - starts for later use 
        comp->pc_lookup[comp->compiled_blocks->len - 1] =
            comp->pc_lookup[comp->compiled_blocks->len - 2] + command_block->len;

        // Lets check, if this node has been written once. In this case we are
           finished. Else we have to go through it again and call this function
           for every SNPCTEXT or PTEXT statement we find in the block. 

        for (i = 0; i < command_block->len; i++)
        {
            cmd = (dialog_cmd *) g_ptr_array_index (command_block, i);

            if (cmd->type == SNPCTEXT || cmd->type == PTEXT)
            {
                // get the node by the number we previously stored in cmd -> new_pc 
                circle = (DlgNode *) get_ptr_list_element (comp->nodes, cmd->new_pc);

                // was node already done? 
                if (comp->link_table[circle->number] != -1)
                {
                    cmd->new_pc = comp->link_table[circle->number];
                    continue;
                }

                if (circle->next->len > 0)
                {
                    num = (get_next_circle (circle, 0))->number;

                    if (comp->pos_table[num] != -1)
                    {
                        cmd->new_pc = comp->pos_table[num];
                        continue;
                    }
                }

                // re-init next_circles 
                g_ptr_array_free (comp->next_circles, FALSE);
                comp->next_circles = g_ptr_array_new ();

                for (j = 0; j < circle->next->len; j++)
                    g_ptr_array_add (comp->next_circles, get_next_circle (circle, j));

                // This is the number of the block, we later must link to 
                cmd->new_pc = comp->compiled_blocks->len - 1;
                comp->link_table[circle->number] = cmd->new_pc;

                // now call this function again 
                compile_nodes (comp);
            }
        }
    }

    return;
*/
}

// Now fill in the jumps between the blocks 
void 
connect_blocks (DlgCompiler * comp)
{
/*
    GPtrArray *block;
    dialog_cmd *cmd;
    u_int32 i, j;

    for (i = 1; i < comp->compiled_blocks->len; i++)
    {
        block = (GPtrArray *) g_ptr_array_index (comp->compiled_blocks, i);

        for (j = 0; j < block->len; j++)
        {
            cmd = (dialog_cmd *) g_ptr_array_index (block, j);

            if (cmd->type == SNPCTEXT || cmd->type == PTEXT)
                cmd->new_pc = comp->pc_lookup[cmd->new_pc];
        }
    }
*/
}

// Write the compiled dialogue to a file 
void 
write_dialogue (DlgCompiler * comp, gchar * file_name)
{
/*
    GPtrArray *dlg = comp->compiled_blocks;
    GPtrArray *block;
    dialog_cmd *cmd;
    FILE *txt;
    FILE *dat;
    u_int32 i, j, k = 0;
    u_int32 num_cmds;

    txt = fopen (g_strjoin (NULL, file_name, ".txt", NULL), "w");
    dat = fopen (g_strjoin (NULL, file_name, ".dat", NULL), "wb");

    num_cmds = comp->pc_lookup[dlg->len - 1];

    // output length of dialogue 
    fwrite (&comp->dlg_length, sizeof(comp->dlg_length), 1, dat);

    // output number of dialogue commands 
    fwrite (&num_cmds, sizeof(num_cmds), 1, dat);
    fprintf (txt, "%i commands in Dialogue\n\n", (int)num_cmds);

    for (i = 0; i < dlg->len; i++)
    {
        block = (GPtrArray *) g_ptr_array_index (dlg, i);

        for (j = 0; j < block->len; j++)
        {
            cmd = (dialog_cmd *) g_ptr_array_index (block, j);

            switch (cmd->type)
            {
            case CLEAR:
                {
                    fprintf (txt, "%i CLEAR\n", (int)k++);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    break;
                }
            case RETURN:
                {
                    fprintf (txt, "%i RETURN %i\n", (int)k++, (int)cmd->text);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    fwrite (&cmd->text, sizeof(cmd->text), 1, dat);
                    break;
                }
           case IMPORT:
                {
                    fprintf (txt, "%i IMPORT \"%s\"\n", (int)k++, ((DlgNode *) get_ptr_list_element (comp->nodes, cmd->new_pc))->text);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    fwrite (&cmd->text, sizeof(cmd->text), 1, dat);
                    break;
                }
            case NPCTEXT:
                {
                    fprintf (txt, "\n%i NPCTEXT %i\n", (int)k++, (int)cmd->text);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    fwrite (&cmd->text, sizeof(cmd->text), 1, dat);
                    break;
                }
            case PTEXT:
                {
                    fprintf (txt, "%i PTEXT %i %i\n", (int)k++, (int)cmd->text, (int)cmd->new_pc);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    fwrite (&cmd->text, sizeof(cmd->text), 1, dat);
                    fwrite (&cmd->new_pc, sizeof(cmd->new_pc), 1, dat);
                    break;
                }
            case SNPCTEXT:
                {
                    fprintf (txt, "\n%i SNPCTEXT %i %i\n", (int)k++, (int)cmd->text, (int)cmd->new_pc);
                    fwrite (&cmd->type, sizeof(cmd->type), 1, dat);
                    fwrite (&cmd->text, sizeof(cmd->text), 1, dat);
                    fwrite (&cmd->new_pc, sizeof(cmd->new_pc), 1, dat);
                    break;
                }
            }
        }

        g_ptr_array_free (block, TRUE);
    }

    g_ptr_array_free (dlg, FALSE);

    fclose (txt);
    fclose (dat);
*/
}

DlgNode *
get_next_circle (DlgNode * node, u_int32 index)
{
/*
    DlgNode *arrow = (DlgNode *) g_ptr_array_index (node->next, index);

    return (DlgNode *) g_ptr_array_index (arrow->next, 0);
*/
    return NULL;
}

void 
add_CLEAR (DlgCompiler * comp, GPtrArray * array)
{
/*
    dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = CLEAR;

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 1;
*/
}

void 
add_RETURN (DlgCompiler * comp, GPtrArray * array, u_int32 retval)
{
/*
    dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = RETURN;
    cmd->text = retval;

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 2;
*/
}

void 
add_NPCTEXT (DlgCompiler * comp, GPtrArray * array, s_int32 index)
{
/*
   dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = NPCTEXT;
    cmd->text = comp->text_number[index];

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 2;
*/
}

void 
add_IMPORT (DlgCompiler * comp, GPtrArray * array, s_int32 text, s_int32 index)
{
/*
    dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = IMPORT;
    cmd->text = text;
    cmd->new_pc = index;

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 2;
*/
}

void 
add_PTEXT (DlgCompiler * comp, GPtrArray * array, s_int32 index)
{
/*
    dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = PTEXT;
    cmd->text = comp->text_number[index];
    cmd->new_pc = index;

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 3;
*/
}

void 
add_SNPCTEXT (DlgCompiler * comp, GPtrArray * array, s_int32 index)
{
/*
    dialog_cmd *cmd = (dialog_cmd *) malloc (sizeof (dialog_cmd));

    cmd->type = SNPCTEXT;
    cmd->text = comp->text_number[index];
    cmd->new_pc = index;

    g_ptr_array_add (array, cmd);
    comp->dlg_length += 3;
*/
}
