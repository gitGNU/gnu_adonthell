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
 * @file gui_graph_events.h
 *
 * @author Kai Sterker
 * @brief Event-callbacks for the dialogue graph view
 */

#ifndef GUI_GRAPH_EVENTS_H
#define GUI_GRAPH_EVENTS_H

#include <gtk/gtkstyle.h>
#include <gdk/gdktypes.h>

/**
 * Callback notifying that the widget's size has changed.
 */
gint configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
/**
 * Callback indicating that the widget needs to be redrawn.
 */
gint expose_event (GtkWidget *, GdkEventExpose *, gpointer);
/**
 * Callback indicating that a mouse button has been pressed.
 */
gint button_press_event (GtkWidget *, GdkEventButton *, gpointer);
/**
 * Callback indicating that a mouse button has been released.
 */
gint button_release_event (GtkWidget *, GdkEventButton *, gpointer);
/**
 * Callback indicating that the mouse has been moved.
 */
gint motion_notify_event (GtkWidget *, GdkEventMotion *, gpointer);
/**
 * Callback indicating that a key has been pushed.
 */
guint key_press_notify_event (GtkWidget *, GdkEventKey *, gpointer);

#endif // GUI_GRAPH_EVENTS_H
