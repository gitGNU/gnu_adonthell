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
#include "debug.h"

void
on_run_destroy (GtkWidget * widget, gpointer user_data)
{
    run_dlg *dlg = (run_dlg *) user_data;

    if (run_dlg::destroy)
    {
        run_dlg::destroy = 0;
        gtk_widget_destroy (widget);
        delete dlg;
    }
}

void
on_dialogue_list_select_child (GtkList * list, GtkWidget * widget, gpointer user_data)
{
    GList *new_items = NULL;
    GList *items = gtk_container_children ((GtkContainer *) list);
    int pos = gtk_list_child_position (list, widget);
    run_dlg *dlg = (run_dlg *) user_data;
    GtkWidget *npc_text; 
    char *str;
        
    // Set the answer the player has chosen
    dlg->answer = GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (widget)));

    pos -= dlg->answer;

    npc_text = (GtkWidget *) g_list_nth_data (items, pos);

    // Player text available!?
    switch (GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (npc_text))))
    {
        // Only NPC Text is available
        case 0:
        {
            gtk_label_get ((GtkLabel *) ((GtkBin *) widget)->child, &str);
            new_items = g_list_append (new_items, create_dlg_list_item (str, 2, -3));

            break;
        }

        // Player text is available
        case -1:
        {
            gtk_label_get ((GtkLabel *) ((GtkBin *) npc_text)->child, &str);
            new_items = g_list_append (new_items, create_dlg_list_item (str, 2, -3));
            gtk_label_get ((GtkLabel *) ((GtkBin *) widget)->child, &str);
            new_items = g_list_append (new_items, create_dlg_list_item (str, 1, -3));

            break;
        }
    }
    
    gtk_list_clear_items (list, pos-1, -1);
    gtk_list_append_items (list, new_items);

    // continue the dialogue
    dlg->run ();    
}

void
on_dialogue_restart_pressed (GtkButton * button, gpointer user_data)
{
    run_dlg *dlg = (run_dlg *) user_data;
    gtk_list_clear_items (GTK_LIST (dlg->list), 0, -1);

    dlg->start ();
    dlg->run ();
}

void
on_dialogue_debug_pressed (GtkButton * button, gpointer user_data)
{
    run_dlg *dlg = (run_dlg *) user_data;

    if (debug_dlg::destroy == 0)
    {
        dlg->wnd->dbg_dlg = new debug_dlg (dlg->wnd);
        dlg->wnd->dbg_dlg->update ();
    }
}

void
on_dialogue_close_pressed (GtkButton * button, gpointer user_data)
{
    run_dlg *dlg = (run_dlg *) user_data;

    on_run_destroy (dlg->dlg, user_data);
}
