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

#include "dlgnode.h"
#include "circle.h"
#include "crcle_callbacks.h"
#include "crcle_interface.h"

// When switching pages of the notebook, give Focus to first widget on the new page
void 
on_switch_page (GtkNotebook *notebook, GtkNotebookPage *page, gint page_num, gpointer user_data)
{
    if (GTK_IS_CONTAINER(page->child))
    {
        GList *children = gtk_container_children ((GtkContainer *) page->child);

        while (children != NULL)
        {
            if (GTK_IS_CONTAINER(g_list_first (children)->data))
            {
                children = gtk_container_children ((GtkContainer *) g_list_first (children)->data);
                continue;
            }
            
            if (GTK_IS_WIDGET(g_list_first (children)->data)) 
            {
                gtk_widget_grab_focus ((GtkWidget *) g_list_first (children)->data);
                return;
            }

            children = g_list_next (children);
        }
    }
}

void
on_text_entry_changed (GtkEditable * editable, gpointer user_data)
{
    ((crcle_dlg *) user_data)->on_text (g_strstrip (gtk_editable_get_chars (editable, 0, -1)));
}

void
on_player_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkWidget *entry = ((crcle_dlg *) user_data)->text_entry;
    GdkColor dark_blue;

    dark_blue.red = 0;
    dark_blue.green = 0;
    dark_blue.blue = 35000;

    // Set Text-color to blue
    gtk_text_freeze ((GtkText *) entry);
    gchar *text = gtk_editable_get_chars ((GtkEditable *) entry, 0, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, -1);
    gtk_text_insert ((GtkText *) entry, entry->style->font, &dark_blue, &entry->style->white, " ", -1);
    gtk_text_insert ((GtkText *) entry, entry->style->font, &dark_blue, &entry->style->white, text, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, 1);
    g_free (text);
    gtk_text_thaw ((GtkText *) entry);

    ((crcle_dlg *) user_data)->on_type (PLAYER);
}


void
on_npc_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkWidget *entry = ((crcle_dlg *) user_data)->text_entry;

    // Set Text-color to black
    gtk_text_freeze ((GtkText *) entry);
    gchar *text = gtk_editable_get_chars ((GtkEditable *) entry, 0, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, -1);
    gtk_text_insert ((GtkText *) entry, entry->style->font, &entry->style->black, &entry->style->white, " ", -1);
    gtk_text_insert ((GtkText *) entry, entry->style->font, &entry->style->black, &entry->style->white, text, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, 1);
    g_free (text);
    gtk_text_thaw ((GtkText *) entry);

    ((crcle_dlg *) user_data)->on_type (NPC);
}


void
on_condition_entry_changed (GtkEditable * editable, gpointer user_data)
{
    ((crcle_dlg *) user_data)->on_condition (gtk_editable_get_chars (editable, 0, -1));
}


void
on_variable_entry_changed (GtkEditable * editable, gpointer user_data)
{
    ((crcle_dlg *) user_data)->on_variable (gtk_editable_get_chars (editable, 0, -1));
}


void
on_annotation_entry_changed (GtkEditable * editable, gpointer user_data)
{
    ((crcle_dlg *) user_data)->on_annotation (gtk_editable_get_chars (editable, 0, -1));
}


void
on_circle_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    ((crcle_dlg *) user_data)->on_ok ();

    // clean up
    gtk_main_quit ();
    gtk_widget_destroy (((crcle_dlg *) user_data)->dlg);
}

void
on_circle_cancel_button_pressed (GtkButton * button, gpointer user_data)
{
    // clean up
    gtk_main_quit ();
    gtk_widget_destroy ((GtkWidget *) user_data);
}
