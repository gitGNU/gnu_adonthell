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

/* Callback Data for editing a node */
typedef struct
{
    MainFrame *wnd;
    DlgNode *node;
    GtkEditable *edit_box;
    u_int8 retval;
}
NodeData;

void create_mainframe (MainFrame *);
GtkWidget *create_text_dialog (NodeData *);
GtkWidget *create_tooltip (gchar *, s_int32, s_int32);
GtkWidget *create_fileselection (GString *, u_int8);
GtkWidget *create_run_dialogue (RunData *);
GtkWidget *create_list_item (MainFrame *, DlgNode *, int);
