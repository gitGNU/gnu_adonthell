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
 * @file gui_list_events.h
 *
 * @author Kai Sterker
 * @brief Event-callbacks for the instant dialogue preview
 */

#ifndef GUI_LIST_EVENTS_H
#define GUI_LIST_EVENTS_H

#include <gtk/gtkstyle.h>
#include <gdk/gdktypes.h>

/**
 * Callback notifying that the user selected one of the list items.
 */
void on_list_select (GtkList *, GtkWidget *, gpointer);

#endif // GUI_LIST_EVENTS_H
