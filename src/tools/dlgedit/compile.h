/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* Dialogue Compiler Data */
typedef struct
{
    GPtrArray *next_circles;    /* Contains circles following node */
    ptr_list *nodes;            /* Contains all dialogue nodes */
    GPtrArray *compiled_blocks; /* Contains the compiled blocks */

    s_int32 *link_table;        /* Contains link to already compiled nodes */
    u_int32 *text_number;       /* contains the every strings position */
    s_int32 *pc_lookup;         /* contains the Program Counter values */
    s_int32 *pos_table;

    u_int32 dlg_length;         /* Number of 32bit values of compiled dialogue */
}
DlgCompiler;

typedef struct
{
    s_int32 type;               /* Command´s type */
    s_int32 text;               /* ID of string to load */
    s_int32 new_pc;             /* New value of Program Counter */
}
dialog_cmd;

void make_dialogue (MainFrame *);
