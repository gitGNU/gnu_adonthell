/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file gui_circle_events.h
 *
 * @author Kai Sterker
 * @brief Callbacks of the Edit Circle dialog
 */

#ifndef GUI_CIRCLE_EVENTS_H
#define GUI_CIRCLE_EVENTS_H

#include <gtk/gtkstyle.h>

void on_radio_button_pressed (GtkButton *button, gpointer user_data);
void on_circle_ok_button_pressed (GtkButton *button, gpointer user_data);
void on_switch_page (GtkNotebook*, GtkNotebookPage*, gint, gpointer user_data);

#endif // GUI_CIRCLE_EVENTS_H

