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
#include <stdlib.h>
#include <ctype.h>

#include "callbacks.h"
#include "interface.h"
#include "dirbrowser.h"
#include "main.h"

// File menu "Open"
void
on_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    GString *file = g_string_new ("");
    GtkWidget *fs = create_fileselection (file, false);

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->last_dir);
    
    // chose file
    gtk_widget_show (fs);
    gtk_main ();

    // try to load file
    wnd->read_character_source (file->str);

    g_string_free (file, TRUE);
}

// File menu "Save"
void
on_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    GtkWidget *dir_browser;
    gchar *name;

    // check a few things:
    // has the quest an ID?
    name = g_strstrip (gtk_entry_get_text (GTK_ENTRY (wnd->name_entry)));
    if (name == NULL || !strcmp (name, ""))
    {
        gtk_widget_grab_focus (wnd->name_entry);
        create_warning ("The quest has no ID!\n\nAn ID is needed by the game engine to access this quest object.");
        gtk_main ();
        return;
    }
    
    // chose directory to save to
    dir_browser = xmms_create_dir_browser ("Select character directory", wnd->char_dir, GTK_SELECTION_SINGLE, wnd);
    gtk_main ();

    wnd->write_character_source ();
}

// File menu "Merge"
void
on_merge_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *cmd;
    GString *file = g_string_new ("");
    GtkWidget *fs = create_fileselection (file, false);
    GtkWidget *dir_browser;

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->last_dir);
    gtk_file_selection_set_filename ((GtkFileSelection *) fs, "quest.data");
    
    // chose directory to save to
    dir_browser = xmms_create_dir_browser ("Select quest directory", wnd->char_dir, GTK_SELECTION_SINGLE, wnd);
    gtk_main ();

    // chose output file
    gtk_window_set_title (GTK_WINDOW (fs), "Select quest data file");
    gtk_widget_show (fs);
    gtk_main ();

    // create character data file
    cmd = g_strjoin (" ", "mergequests", wnd->cur_dir, file->str, NULL);
    system (cmd);

    g_string_free (file, TRUE);
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
    text[0] = g_strstrip (gtk_entry_get_text (GTK_ENTRY (wnd->attrib_entry)));
    text[1] = gtk_entry_get_text (GTK_ENTRY (wnd->val_entry));

    // check whether attribute and value are nonzero
    if (text[0] == NULL || !strcmp (text[0], ""))
        return;
    if (text[1] == NULL || !strcmp (text[1], ""))
        return;

    // check whether value is an integer
    if (text[1][0] != '-' && !isdigit (text[1][0]))
        return;

    // clear entries and selection
    gtk_entry_set_text (GTK_ENTRY (wnd->attrib_entry), "");
    gtk_entry_set_text (GTK_ENTRY (wnd->val_entry), "");
    gtk_clist_unselect_row (GTK_CLIST (wnd->attribute_list), wnd->attribute_list_sel, 0);

    // check whether attribute already exists
    while (gtk_clist_get_text (GTK_CLIST (wnd->attribute_list), i++, 0, &str))
        if (!strcmp (text[0], str))
        {
            // in case it does, only update the value
            gtk_clist_set_text (GTK_CLIST (wnd->attribute_list), i - 1, 1, text[1]);

            return;
        }

    // else add them to the list and clear the entries
    gtk_clist_append (GTK_CLIST (wnd->attribute_list), text);

    wnd->colorify_list (GTK_CLIST (wnd->attribute_list));
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
        wnd->colorify_list (GTK_CLIST (wnd->attribute_list));

        return;
    }

    // check whether attribute is nonzero
    if (attr == NULL || !strcmp (attr, ""))
        return;

    // find attr in list and remove it
    while (gtk_clist_get_text (GTK_CLIST (wnd->attribute_list), i++, 0, &str))
        if (!strcmp (attr, str))
        {
            gtk_clist_remove (GTK_CLIST (wnd->attribute_list), i - 1);
            wnd->colorify_list (GTK_CLIST (wnd->attribute_list));

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

// File-Selection: ok -> get file name
void
on_fs_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkFileSelection *fs = (GtkFileSelection *) gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_FILE_SELECTION);
    GString *file = (GString *) user_data;

    file = g_string_assign (file, gtk_file_selection_get_filename (fs));
    gtk_widget_destroy (GTK_WIDGET (fs));
    gtk_main_quit ();
}

// File-Selection: cancel -> no file name
void
on_fs_cancel_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkFileSelection *fs = (GtkFileSelection *) gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_FILE_SELECTION);

    gtk_widget_destroy (GTK_WIDGET (fs));
    gtk_main_quit ();
}

void
on_warning_close_clicked (GtkButton * button, gpointer user_data)
{
    on_widget_destroy (GTK_WIDGET (user_data), NULL);
}
