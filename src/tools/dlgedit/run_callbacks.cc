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
#include "../../dialog.h"


void
on_dialogue_list_select_child (GtkList * list, GtkWidget * widget, gpointer user_data)
{
    GList *new_items = NULL;
    GList *items = gtk_container_children ((GtkContainer *) list);
    int pos = gtk_list_child_position (list, widget);
    dialog *dlg = ((run_dlg *) user_data)->dat;
    GtkWidget *npc_text; 
    char *str;
        
    // Set the answer the player has chosen
    dlg->answer = GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (widget)));
    pos -= dlg->answer;
  
    npc_text = (GtkWidget *) g_list_nth_data (items, pos-1);

    // Player text available!?
    if (GPOINTER_TO_INT (gtk_object_get_user_data (GTK_OBJECT (npc_text))) == -1)
    {
        gtk_label_get ((GtkLabel *) ((GtkBin *) npc_text)->child, &str);
        new_items = g_list_append (new_items, create_dlg_list_item (str, 2, -3));
        gtk_label_get ((GtkLabel *) ((GtkBin *) widget)->child, &str);
        new_items = g_list_append (new_items, create_dlg_list_item (str, 1, -3));
        gtk_list_clear_items (list, pos-2, -1);
    }
    else
    {
        gtk_label_get ((GtkLabel *) ((GtkBin *) widget)->child, &str);
        new_items = g_list_append (new_items, create_dlg_list_item (str, 2, -3));
        gtk_list_clear_items (list, pos-1, -1);
    }

    gtk_list_append_items (list, new_items);

    // continue the dialogue
    ((run_dlg *) user_data)->run ();    
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
