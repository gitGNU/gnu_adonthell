/*
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/** 
 * @file gui_list_events.h
 *
 * @author Kai Sterker
 * @brief Event-callbacks for the instant dialogue preview
 */

#ifndef GUI_LIST_EVENTS_H
#define GUI_LIST_EVENTS_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>

/**
 * Callback notifying that the user selected one of the list items.
 */
void on_list_select (GtkTreeSelection *selection, gpointer user_data);

/**
 * Callback to resize the text to the new list size
 */
void on_list_resize (GtkWidget *widget, GdkRectangle *allocation, gpointer data);

#endif // GUI_LIST_EVENTS_H
