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

void create_mainframe (MainFrame *);
GtkWidget *create_tooltip (const gchar *, s_int32, s_int32);
GtkWidget *create_fileselection (GString *, u_int8);
GtkWidget *create_list_item (MainFrame *, DlgNode *, int);
