/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gtk/gtk.h>


void
 on_project_activate (GtkMenuItem * menuitem,
    gpointer user_data);

void
 on_project_load_activate (GtkMenuItem * menuitem,
    gpointer user_data);

void
 on_project_save_activate (GtkMenuItem * menuitem,
    gpointer user_data);

void
 on_project_make_activate (GtkMenuItem * menuitem,
    gpointer user_data);

void
 on_project_close_activate (GtkMenuItem * menuitem,
    gpointer user_data);

void
 on_project_tree_selection_changed (GtkTree * tree,
    gpointer user_data);

void
 on_import_npc_clicked (GtkButton * button,
    gpointer user_data);

void
 on_add_npc_clicked (GtkButton * button,
    gpointer user_data);

void
 on_import_dlg_clicked (GtkButton * button,
    gpointer user_data);

void
 on_add_dlg_clicked (GtkButton * button,
    gpointer user_data);
