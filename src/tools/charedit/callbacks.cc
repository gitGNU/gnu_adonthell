/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gtk/gtk.h>
#include <ctype.h>

#include "callbacks.h"
#include "interface.h"
#include "main.h"

// File menu "Open"
void
on_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}

// File menu "Save"
void
on_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{

}

// File menu "Quit"
void 
on_widget_destroy (GtkWidget * widget, gpointer data)
{
    gtk_widget_destroy (widget);
    gtk_main_quit ();
}

// Adds a new (attribut, value) pair to the attrib_list
void
on_attrib_update_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *text[2] = { 0, 0 };
    gchar *str;
    int i = 0;

    // grab attribute and value
    text[0] = gtk_entry_get_text (GTK_ENTRY (wnd->attrib_entry));
    text[1] = gtk_entry_get_text (GTK_ENTRY (wnd->val_entry));

    // check whether attribute and value are nonzero
    if (text[0] == NULL || !strcmp (text[0], "")) return;
    if (text[1] == NULL || !strcmp (text[1], "")) return;

    // check whether value is an integer
    if (text[1][0] != '-' && !isdigit (text[1][0])) return;

    // check whether attribute already exists
    while (gtk_clist_get_text (GTK_CLIST (wnd->attribute_list), i++, 0, &str))
        if (!strcmp (text[0], str))
        {
            // in case it does, only update the value
            gtk_clist_set_text (GTK_CLIST (wnd->attribute_list), i-1, 1, text[1]);
            gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), "");
            gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), ""); 

            return;
        }
    
    // add them to the list and clear the entries
    gtk_clist_append (GTK_CLIST (wnd->attribute_list), text);
    gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), "");
    gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), ""); 
}

// Remove the selected row in the attribute-list
void
on_attrib_remove_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *attr = 0;
    gchar *str;
    int i = 0;

    // grab the attribute
    attr = gtk_entry_get_text (GTK_ENTRY (wnd->attrib_entry));

    // delete selected row
    if (wnd->attribute_list_sel != -1)
    {
        // Check whether selected row and attrib_entry match
        gtk_clist_get_text (GTK_CLIST (wnd->attribute_list), wnd->attribute_list_sel, 0, &str);
        if (!strcmp (attr, str))
        {
            gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), "");
            gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), ""); 
        }

        // Delete row
        gtk_clist_remove (GTK_CLIST (wnd->attribute_list), wnd->attribute_list_sel);
        
        return;
    }
    

    // check whether attribute is nonzero
    if (attr == NULL || !strcmp (attr, "")) return;

    // find attr in list and remove it
    while (gtk_clist_get_text (GTK_CLIST (wnd->attribute_list), i++, 0, &str))
        if (!strcmp (attr, str))
        {
            gtk_clist_remove (GTK_CLIST (wnd->attribute_list), i-1);
            gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), "");
            gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), ""); 

            return;
        }
}

// set attrib and value to the entries in the selected row
void
on_attribute_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *str;

    // set the attribute
    gtk_clist_get_text (clist, row, 0, &str);
    gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), str);

    // set the value
    gtk_clist_get_text (clist, row, 1, &str);
    gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), str);

    // remember selected row
    wnd->attribute_list_sel = row;
}

// unset attribute_list_sel
void
on_attribute_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    wnd->attribute_list_sel = -1;
}

void
on_event_add_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_event_remove_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_event_update_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_event_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{

}


void
on_dlg_add_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_dlg_remove_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_dlg_default_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_dlg_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{

}


void
on_enter_script_entry_changed (GtkEditable * editable, gpointer user_data)
{

}


void
on_enter_browse_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_enter_ok_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_enter_cancel_clicked (GtkButton * button, gpointer user_data)
{

}
