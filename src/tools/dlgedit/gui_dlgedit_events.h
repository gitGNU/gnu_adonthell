/*
   Copyright (C) 1999/2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
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
gboolean on_display_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);
gboolean on_clear_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);

#endif // GUI_DLGEDIT_EVENTS_H
