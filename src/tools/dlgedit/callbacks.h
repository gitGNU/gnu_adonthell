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

void on_widget_destroy (GtkWidget *, gpointer);

// Text Dialog Callbacks
void on_button_player_toggled (GtkToggleButton *, gpointer);
void on_button_npc_toggled (GtkToggleButton *, gpointer);
void on_cancel_button_pressed (GtkButton *, gpointer);
void on_ok_button_pressed (GtkButton *, gpointer);

// Fileselection Dialog Callbacks
void on_fs_ok_button_pressed (GtkButton *, gpointer);
void on_fs_cancel_button_pressed (GtkButton *, gpointer);

// Menu Callbacks
void on_file_project_activate (GtkMenuItem *, gpointer);
void on_file_new_activate (GtkMenuItem *, gpointer);
void on_file_load_activate (GtkMenuItem *, gpointer);
void on_file_save_activate (GtkMenuItem *, gpointer);
void on_dialogue_compile_activate (GtkMenuItem *, gpointer);
void on_dialogue_run_activate (GtkMenuItem *, gpointer);

// Run Dialog Callbacks
void on_player_txt_select_row (GtkCList *, gint, gint, GdkEvent *, gpointer);
void on_list_select (GtkList *, GtkWidget *, gpointer);

// Function Dialog Callbacks
void on_function_released (GtkButton *, gpointer);
void on_operation_released (GtkButton *, gpointer);
void on_add_button_clicked (GtkButton *, gpointer);
void on_remove_button_clicked (GtkButton *, gpointer);
void on_up_button_clicked (GtkButton *, gpointer);
void on_down_button_clicked (GtkButton *, gpointer);
void on_fct_ok_buttpn_clicked (GtkButton *, gpointer);
void on_fct_select_row (GtkWidget *, gint , gint , GdkEventButton *, gpointer);
void on_fct_unselect_row (GtkWidget *, gint , gint , GdkEventButton *, gpointer);
