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

#include "dbg_callbacks.h"
#include "dbg_interface.h"
#include "debug.h"
#include "run.h"

void
on_debug_destroy (GtkWidget * widget, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;

    if (debug_dlg::destroy)
    {
        debug_dlg::destroy = 0;
        gtk_widget_destroy (widget);
        delete dlg;
    }
}

void
on_character_tree_select (GtkWidget * widget, gint row, gint column, GdkEventButton * bevent, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;
    char *title, *attribute, *value;
    dbg_node_data *data;
	GtkCTreeNode *node;

	if (bevent)
	{
		if (bevent->button == 1)
		{
            dlg->selected_row = row;
		
			node = gtk_ctree_node_nth (GTK_CTREE (widget), row);
			data = (dbg_node_data *) gtk_ctree_node_get_row_data (GTK_CTREE (widget), node);
            attribute = data->attribute;
            gtk_ctree_node_get_text (GTK_CTREE (widget), node, 1, &value);

            // Top-Level node:
            if (value[0] == 0)
            {
                title = attribute;
                attribute = 0;
                value = 0;
            }
            // Attribute node:
            else
            {
                GtkCTreeRow *tree_row = GTK_CTREE_ROW (node);
			    data = (dbg_node_data *) gtk_ctree_node_get_row_data (GTK_CTREE (widget), tree_row->parent);
                title = data->attribute;
            }

            create_dbg_edit_wnd (dlg, title, attribute, value);
            gtk_main ();

            dlg->selected_row = -1;
            gtk_clist_unselect_row (GTK_CLIST (widget), row, column);
		}
	}
}

void
on_run_debug_clicked (GtkButton * button, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;

    if (run_dlg::destroy == 0)
    {
        dlg->wnd->test_dlg = new run_dlg (dlg->wnd);
        dlg->wnd->test_dlg->run ();
    }
}


void
on_close_debug_clicked (GtkButton * button, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;

    on_debug_destroy (dlg->dlg, user_data);
}

void 
on_debug_page_switched (GtkNotebook *notebook, GtkNotebookPage *page, gint num, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;
    dlg->active_page = num;
}

void
on_dbg_edit_ok_clicked (GtkButton * button, gpointer user_data)
{
    debug_dlg *dlg = (debug_dlg *) user_data;
    char *key = strdup (gtk_entry_get_text (GTK_ENTRY (dlg->attrib_entry)));
    char *val = gtk_entry_get_text (GTK_ENTRY (dlg->val_entry));

    if (key[0] == 0 || val[0] == 0) return; 

    dlg->set (key, val);
    on_dbg_edit_cancel_clicked (button, gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_dbg_edit_cancel_clicked (GtkButton * button, gpointer user_data)
{
    gtk_widget_destroy (GTK_WIDGET (user_data));
    gtk_main_quit ();
}
