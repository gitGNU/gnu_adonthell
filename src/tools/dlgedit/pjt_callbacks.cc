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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "pjt_callbacks.h"
#include "pjt_interface.h"


void
on_project_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}


void
on_project_load_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}


void
on_project_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}


void
on_project_make_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}

// Close the Project Window
void on_project_close_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    // gtk_widget_destroy (GtkWidget*);
}

// Other tree item selected
void on_project_tree_selection_changed (GtkTree * tree, gpointer user_data)
{
    // Save changes made to this one

    // Display content of newly selected one 
}


void
on_import_npc_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_add_npc_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_import_dlg_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_add_dlg_clicked (GtkButton * button, gpointer user_data)
{

}
