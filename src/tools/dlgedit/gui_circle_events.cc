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
 * @file gui_circle_events.cc
 *
 * @author Kai Sterker
 * @brief Callbacks of the Edit Circle dialog
 */

#include <gtk/gtk.h>
#include "gui_circle_events.h"
#include "gui_circle.h"

// When switching pages of the notebook, give Focus to first widget on the new page
void on_switch_page (GtkNotebook *notebook, GtkNotebookPage *page, gint page_num, gpointer user_data)
{
    if (GTK_IS_CONTAINER (page->child))
    {
        GList *children = gtk_container_children ((GtkContainer *) page->child);

        while (children != NULL)
        {
            GtkWidget *widget = (GtkWidget*) g_list_first (children)->data;

            if (GTK_IS_CONTAINER (widget))
            {
                children = gtk_container_children ((GtkContainer *) widget);
                continue;
            }

            if (GTK_IS_WIDGET (widget))
            {
                gtk_widget_grab_focus (widget);
                return;
            }

            children = (GList *) g_list_next (children);
        }
    }
}

// Colorize the dialogue text according to the character it is assigned to
void on_radio_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkWidget *entry = (GtkWidget *) user_data;
    int type = GPOINTER_TO_INT (gtk_object_get_data (GTK_OBJECT (button), "type"));
    GdkColor color = { 0, 0, 0, 0 };

    switch (type)
    {
        case PLAYER:
            color.blue = 35000;
            break;
        case NARRATOR:
            color.green = 27300;
            color.blue = 15600;
            break;
    }

    // Change the color of the text entry
    gtk_text_freeze ((GtkText *) entry);
    gchar *text = gtk_editable_get_chars ((GtkEditable *) entry, 0, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, -1);

    // workaround in case the textbox is empty
    gtk_text_insert ((GtkText *) entry, entry->style->font, &color, &entry->style->white, " ", -1);
    gtk_text_insert ((GtkText *) entry, entry->style->font, &color, &entry->style->white, text, -1);
    gtk_editable_delete_text ((GtkEditable *) entry, 0, 1);

    // cleanup
    g_free (text);
    gtk_text_thaw ((GtkText *) entry);

    // remember the type of the text
    gtk_object_set_data (GTK_OBJECT (entry), "type", GINT_TO_POINTER (type));
}

// close the dialog and keep all changes
void on_circle_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    GuiCircle *dialog = (GuiCircle *) user_data;

    dialog->applyChanges ();
    dialog->okButtonPressed (true);

    // clean up
    gtk_main_quit ();
}

// close dialog and dismiss all changes
void on_circle_cancel_button_pressed (GtkButton * button, gpointer user_data)
{
    // clean up
    gtk_main_quit ();
}

