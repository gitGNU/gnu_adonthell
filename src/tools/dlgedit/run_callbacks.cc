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

#include "run.h"
#include "run_callbacks.h"
#include "run_interface.h"


void
on_dialogue_list_select_child (GtkList * list, GtkWidget * widget, gpointer user_data)
{

}


void
on_dialogue_export_pressed (GtkButton * button, gpointer user_data)
{

}


void
on_dialogue_close_pressed (GtkButton * button, gpointer user_data)
{
    gtk_widget_destroy ((GtkWidget *) user_data);
    gtk_main_quit ();
}
