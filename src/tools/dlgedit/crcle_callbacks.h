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

void on_text_entry_changed (GtkEditable * editable, gpointer user_data);
void on_player_button_pressed (GtkButton * button, gpointer user_data);
void on_npc_button_pressed (GtkButton * button, gpointer user_data);
void on_condition_entry_changed (GtkEditable * editable, gpointer user_data);
void on_variable_entry_changed (GtkEditable * editable, gpointer user_data);
void on_annotation_entry_changed (GtkEditable * editable, gpointer user_data);
void on_circle_ok_button_pressed (GtkButton * button, gpointer user_data);
void on_circle_cancel_button_pressed (GtkButton * button, gpointer user_data);
void on_switch_page (GtkNotebook*, GtkNotebookPage*, gint, gpointer user_data);