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
    gchar *name, *dialogue, *schedule;
    GtkWidget *dir_browser;

    // check a few things:
    // has the character a name?
    name = g_strstrip (gtk_entry_get_text (GTK_ENTRY (wnd->name_entry)));
    if (name == NULL || !strcmp (name, ""))
    {
        gtk_notebook_set_page (GTK_NOTEBOOK (wnd->notebook), 0);
        gtk_widget_grab_focus (wnd->name_entry);
        create_warning ("The character has no name!\n\nThe name is not only a simple attribute of the character, it is also an ID needed by the game engine to access the character data.");
        gtk_main ();
        return;
    }
    
    // has the character a schedule?
    schedule = g_strstrip (gtk_entry_get_text (GTK_ENTRY (wnd->scl_entry)));
    if (schedule == NULL || !strcmp (schedule, ""))
    {
        gtk_notebook_set_page (GTK_NOTEBOOK (wnd->notebook), 3);
        gtk_widget_grab_focus (wnd->scl_entry);
        create_warning ("The character has no schedule!\n\nSchedules control the character's movements and behaviour. Without a schedule it will remain frozen.");
        gtk_main ();
        return;
    }

    // has the character a dialogue?
    dialogue = g_strstrip (gtk_entry_get_text (GTK_ENTRY (wnd->dlg_entry)));
    if (dialogue == NULL || !strcmp (dialogue, ""))
    {
        gtk_notebook_set_page (GTK_NOTEBOOK (wnd->notebook), 3);
        gtk_widget_grab_focus (wnd->dlg_entry);
        create_warning ("The character has no dialogue!\n\nWithout a dialogue, the character will not be able to respond to the player when contacted.");
        gtk_main ();
        return;
    }

    // chose directory to save to
    dir_browser = xmms_create_dir_browser ("Select character directory", wnd->char_dir, GTK_SELECTION_SINGLE, wnd);
    gtk_main ();

    wnd->write_character_source ();
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

// add a event to the event-list
void
on_event_add_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    GtkWidget *event_dlg = 0;
    gchar *text[3];

    // get the selected event type
    gchar *event = wnd->get_option (GTK_OPTION_MENU (wnd->event_choice));

    // and create the dialog
    event_wnd dlg (wnd, event);

    gtk_main ();

    if (dlg.ok)
    {
        text[0] = event;
        text[1] = dlg.script;
        text[2] = dlg.params;

        gtk_clist_append (GTK_CLIST (wnd->event_list), text);
        wnd->colorify_list (GTK_CLIST (wnd->event_list));
    }
}


void
on_event_remove_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;

    // is row selected?
    if (wnd->event_list_sel != -1)
    {
        // Delete row
        gtk_clist_remove (GTK_CLIST (wnd->event_list), wnd->event_list_sel);
        wnd->colorify_list (GTK_CLIST (wnd->event_list));
    }
}


void
on_event_update_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *params, **pairs, **vals;
    gchar *event, *condition, *script;
    int i = 0, j = 0;

    // return if no row selected
    if (wnd->event_list_sel < 0)
        return;

    // Get the event type of the selected row
    gtk_clist_get_text (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 0, &event);

    // and create the dialog
    event_wnd dlg (wnd, event);

    // Now set the parameters & script accordingly
    gtk_clist_get_text (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 1, &script);
    gtk_clist_get_text (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 2, &params);
    gtk_entry_set_text (GTK_ENTRY (dlg.script_entry), script);

    pairs = g_strsplit (params, ", ", 0);

    while (pairs[i] != NULL)
    {
        vals = g_strsplit (pairs[i++], "=", 0);

        while (gtk_clist_get_text (GTK_CLIST (dlg.condition_list), j, 0, &condition))
        {
            if (!strcmp (condition, vals[0]))
                gtk_clist_set_text (GTK_CLIST (dlg.condition_list), j, 1, vals[1]);
            j++;
        }

        j = 0;
        g_strfreev (vals);
    }

    g_strfreev (pairs);

    gtk_main ();

    if (dlg.ok)
    {
        // update the values
        gtk_clist_set_text (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 1, dlg.script);
        gtk_clist_set_text (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 2, dlg.params);

        // and deselect the entry
        gtk_clist_unselect_row (GTK_CLIST (wnd->event_list), wnd->event_list_sel, 0);
    }
}


void
on_event_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;

    wnd->event_list_sel = row;
}


void
on_event_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;

    wnd->event_list_sel = -1;
}

void
on_chose_schedule_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *script = wnd->get_script ();

    gtk_entry_set_text (GTK_ENTRY (wnd->scl_entry), script);
}


void
on_chose_dialogue_clicked (GtkButton * button, gpointer user_data)
{
    main_wnd *wnd = (main_wnd *) user_data;
    gchar *script = wnd->get_script ();

    gtk_entry_set_text (GTK_ENTRY (wnd->dlg_entry), script);
}


void
on_event_script_entry_changed (GtkEditable * editable, gpointer user_data)
{

}


void
on_event_browse_clicked (GtkButton * button, gpointer user_data)
{
    event_wnd *wnd = (event_wnd *) user_data;
    gchar *script = wnd->main->get_script ();

    gtk_entry_set_text (GTK_ENTRY (wnd->script_entry), script);
}


void
on_event_ok_clicked (GtkButton * button, gpointer user_data)
{
    event_wnd *wnd = (event_wnd *) user_data;
    GString *params = g_string_new ("");
    gchar *condition, *entry;
    int i = 0;

    // get script
    wnd->script = g_strstrip (g_strdup (gtk_entry_get_text (GTK_ENTRY (wnd->script_entry))));
    if (wnd->script == NULL || !strcmp (wnd->script, ""))
    {
        gtk_widget_grab_focus (wnd->script_entry);
        create_warning ("The event has no script assigned!\n\nThe script defines what happens if the event occurs. Without it, the event remains useless.");
        gtk_main ();
        return;
    }

    // get conditions
    while (gtk_clist_get_text (GTK_CLIST (wnd->condition_list), i, 1, &entry))
    {
        if (strcmp (entry, ""))
        {
            gtk_clist_get_text (GTK_CLIST (wnd->condition_list), i, 0, &condition);

            if (strcmp (params->str, ""))
                g_string_append (params, ", ");

            g_string_append (params, condition);
            g_string_append (params, "=");
            g_string_append (params, entry);
        }

        i++;
    }

    wnd->params = params->str;
    wnd->ok = true;

    g_string_free (params, FALSE);
    on_widget_destroy (gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_WINDOW), NULL);
}


void
on_event_cancel_clicked (GtkButton * button, gpointer user_data)
{
    on_widget_destroy (GTK_WIDGET (user_data), NULL);
}


void
on_condition_update_clicked (GtkButton * button, gpointer user_data)
{
    event_wnd *wnd = (event_wnd *) user_data;
    gchar *condition;

    // No Condition selected
    if (wnd->condition_list_sel < 0)
        return;

    // Update the value
    condition = gtk_entry_get_text (GTK_ENTRY (wnd->condition_entry));
    gtk_clist_set_text (GTK_CLIST (wnd->condition_list), wnd->condition_list_sel, 1, condition);

    // and deselect the entry
    gtk_clist_unselect_row (GTK_CLIST (wnd->condition_list), wnd->condition_list_sel, 0);
}


void
on_condition_list_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    event_wnd *wnd = (event_wnd *) user_data;
    gchar *condition, *entry;

    wnd->condition_list_sel = row;

    // Update the label text
    gtk_clist_get_text (GTK_CLIST (wnd->condition_list), wnd->condition_list_sel, 0, &condition);
    gtk_label_set_text (GTK_LABEL (wnd->condition_label), condition);

    // Update the entry text
    gtk_clist_get_text (GTK_CLIST (wnd->condition_list), wnd->condition_list_sel, 1, &entry);
    gtk_entry_set_text (GTK_ENTRY (wnd->condition_entry), entry);
}


void
on_condition_list_unselect_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    event_wnd *wnd = (event_wnd *) user_data;

    gtk_label_set_text (GTK_LABEL (wnd->condition_label), "Value");
    gtk_entry_set_text (GTK_ENTRY (wnd->condition_entry), "");

    wnd->condition_list_sel = -1;
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
