/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

gint configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
gint expose_event (GtkWidget *, GdkEventExpose *, gpointer);
gint button_press_event (GtkWidget *, GdkEventButton *, gpointer);
gint button_release_event (GtkWidget *, GdkEventButton *, gpointer);
gint motion_notify_event (GtkWidget *, GdkEventMotion *, gpointer);
guint key_press_notify_event (GtkWidget *, GdkEventKey *, gpointer);
