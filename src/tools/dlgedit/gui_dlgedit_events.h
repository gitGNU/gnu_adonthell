/*
   $Id$
   
   Copyright (C) 1999/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file gui_dlgedit_events.h
 *
 * @author Kai Sterker
 * @brief The Events triggered via the Dialogue Editor's main window
 */

#ifndef GUI_DLGEDIT_EVENTS_H
#define GUI_DLGEDIT_EVENTS_H

void on_widget_destroy (GtkWidget *, gpointer);

// Menu Callbacks
void on_file_new_activate (GtkMenuItem *, gpointer);
void on_file_load_activate (GtkMenuItem *, gpointer);
void on_file_load_recent_activate (GtkMenuItem *, gpointer);
void on_file_save_activate (GtkMenuItem *, gpointer);
void on_file_save_as_activate (GtkMenuItem *, gpointer);
void on_file_revert_activate (GtkMenuItem *, gpointer);
void on_file_close_activate (GtkMenuItem *, gpointer);
void on_dialogue_compile_activate (GtkMenuItem *, gpointer);
void on_dialogue_run_activate (GtkMenuItem *, gpointer);
void on_dialogue_preview_activate (GtkMenuItem *, gpointer);
void on_dialogue_functions_activate (GtkMenuItem *, gpointer);
void on_dialogue_player_activate (GtkMenuItem *, gpointer);
void on_window_activate (GtkMenuItem *, gpointer);

// Statusbar callbacks
void on_display_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);
void on_clear_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

#endif // GUI_DLGEDIT_EVENTS_H
