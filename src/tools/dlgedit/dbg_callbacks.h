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

#ifndef __DBG_CALLBACKS_H__
#define __DBG_CALLBACKS_H__

#include <gtk/gtk.h>

void on_debug_destroy (GtkWidget *, gpointer);
void on_character_tree_expand (GtkCTree *, GList *, gpointer);
void on_character_tree_select (GtkWidget *, gint, gint, GdkEventButton *, gpointer);
void on_quest_tree_expand (GtkCTree *, GList *, gpointer);
void on_update_debug_clicked (GtkButton *, gpointer);
void on_close_debug_clicked (GtkButton *, gpointer);
void on_debug_page_switched (GtkNotebook *, GtkNotebookPage *, gint, gpointer);
void on_dbg_edit_ok_clicked (GtkButton *, gpointer);
void on_dbg_edit_cancel_clicked (GtkButton *, gpointer);

#endif
