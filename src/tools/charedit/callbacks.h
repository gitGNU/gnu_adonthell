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

#include <gtk/gtk.h>

void on_open_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_save_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_merge_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_widget_destroy (GtkWidget * widget, gpointer user_data);
void on_attrib_update_clicked (GtkButton * button, gpointer user_data);
void on_attrib_remove_clicked (GtkButton * button, gpointer user_data);
void on_attribute_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_attribute_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_event_add_clicked (GtkButton * button, gpointer user_data);
void on_event_remove_clicked (GtkButton * button, gpointer user_data);
void on_event_update_clicked (GtkButton * button, gpointer user_data);
void on_event_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_event_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_chose_schedule_clicked (GtkButton * button, gpointer user_data);
void on_chose_dialogue_clicked (GtkButton * button, gpointer user_data);
void on_event_script_entry_changed (GtkEditable * editable, gpointer user_data);
void on_event_browse_clicked (GtkButton * button, gpointer user_data);
void on_event_ok_clicked (GtkButton * button, gpointer user_data);
void on_event_cancel_clicked (GtkButton * button, gpointer user_data);
void on_condition_update_clicked (GtkButton * button, gpointer user_data);
void on_condition_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_condition_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data);
void on_fs_ok_button_pressed (GtkButton * button, gpointer user_data);
void on_fs_cancel_button_pressed (GtkButton * button, gpointer user_data);
void on_warning_close_clicked (GtkButton * button, gpointer user_data);
