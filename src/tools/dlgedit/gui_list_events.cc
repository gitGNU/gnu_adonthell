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
 * @file gui_list_events.cc
 *
 * @author Kai Sterker
 * @brief Event-callbacks for the instant dialogue preview
 */
 
#include <gtk/gtk.h>
#include "gui_list_events.h"
#include "gui_dlgedit.h"

// Node selected in preview
void on_list_select (GtkList *list, GtkWidget *widget, gpointer user_data)
{
    DlgCircle* circle = (DlgCircle *) gtk_object_get_user_data (GTK_OBJECT (widget));
    GuiGraph *graph = GuiDlgedit::window->graph ();
    
    // change selection in the graph view
    graph->deselectNode ();
    graph->selectNode (circle);
    graph->centerNode ();
}
