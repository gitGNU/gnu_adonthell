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
#include <fstream.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "interface.h"

int
main (int argc, char *argv[])
{
    main_wnd wnd;

    gtk_init (&argc, &argv);

    GtkWidget *app = create_main_wnd (wnd);

    wnd.load_list_defaults (GTK_CLIST (wnd.attribute_list), "attributes.txt", "0");

    gtk_widget_show (app);
    gtk_main ();

    return 0;
}

gchar *main_wnd::events[5] = { "Enter", "Leave", "Pickup", "Drop", "Kill" };
gchar *main_wnd::races[4] = { "Dwarf", "Elf", "Half-Elf", "Human" };
gchar *main_wnd::gender[2] = { "Female", "Male" };

main_wnd::main_wnd ()
{
    attribute_list_sel = -1;
    event_list_sel = -1;
    last_dir = "";
    char_dir = g_get_current_dir ();
    cur_dir = NULL;
    posx = "0";
    posy = "0";
}

// load a few default attributes from a file
void
main_wnd::load_list_defaults (GtkCList * list, gchar * file, gchar * _default)
{
    FILE *f = fopen (file, "r");

    if (!f)
        return;

    char *text[2] = { 0, _default };
    char str[256];

    while (!feof (f))
    {
        // read a line and strip all leading/trailing whitespace 
        fgets (str, 255, f);
        g_strstrip (str);

        // add line as long as it isn't empty or a comment
        if (str != NULL && str[0] != '#' && str[0] != '\0')
        {
            text[0] = str;
            gtk_clist_append (list, text);
        }
    }

    colorify_list (list);
    fclose (f);
}

// set every 2nd row to another color to make looking at stuff easier
void
main_wnd::colorify_list (GtkCList * list)
{
    int i = 0;
    gchar *str;

    // A Light blue color
    GdkColor color;

    color.red = 55000;
    color.blue = 65000;
    color.green = 55000;

    // can't think of a better way to iterate through the CList :P
    while (gtk_clist_get_text (list, i++, 0, &str))
        if (!(i % 2))
            gtk_clist_set_background (list, i - 1, &color);
        else
            gtk_clist_set_background (list, i - 1, &GTK_WIDGET (list)->style->white);
}

// returns selected option
gchar *
main_wnd::get_option (GtkOptionMenu * o)
{
    GtkMenu *m = (GtkMenu *) gtk_option_menu_get_menu (o);
    GtkMenuItem *i = (GtkMenuItem *) gtk_menu_get_active (m);

    return (gchar *) gtk_object_get_user_data (GTK_OBJECT (i));
}

void
main_wnd::set_option (GtkOptionMenu * o, gchar * label)
{
    GtkMenu *m = (GtkMenu *) gtk_option_menu_get_menu (o);
    GList *l = gtk_container_children (GTK_CONTAINER (m));
    gchar *c;
    int j = 0;

    while (l)
    { 
        GtkMenuItem *i = (GtkMenuItem *) l->data;
        c = (gchar *) gtk_object_get_user_data (GTK_OBJECT (i));

        if (strcmp (c, label) == 0)
        {
            gtk_option_menu_set_history (o, j);
            break;
        }
        
        j++;
        l = g_list_next (l);
    }
}

// Write all info to a file
void 
main_wnd::write_character_source ()
{
    if (cur_dir == NULL) return;

    gchar *name, *dialogue, *schedule, *tmp, *fname;
    gchar *str;
    int i = 0;
    ofstream file;

    // Make chosen directory default directory 
    g_free (char_dir);
    char_dir = g_strdup (cur_dir);

    // Get some data
    name = g_strstrip (gtk_entry_get_text (GTK_ENTRY (name_entry)));
    schedule = g_strstrip (gtk_entry_get_text (GTK_ENTRY (scl_entry)));
    dialogue = g_strstrip (gtk_entry_get_text (GTK_ENTRY (dlg_entry)));

    // create filename from chosen directory and the character's name
    fname = g_strdup (name);
    g_strdown (fname);
    g_strdelimit (fname, " ", '_');
    tmp = g_strconcat (cur_dir, fname, ".character", NULL);

    // try to open file
    file.open (tmp);
    if (!file)
    {
        create_warning ("    Save failed!");
        gtk_main ();
        g_free (tmp);
        g_free (fname);
        return;
    }

    // write stuff to file
    file << "# Adonthell character source file\n\n";

    file << "basic:\n";
    file << "name = " << name << "\n";
    file << "race = " << get_option (GTK_OPTION_MENU (race_choice)) << "\n";
    file << "gender = " << get_option (GTK_OPTION_MENU (gender_choice)) << "\n";
    file << "posx = " << posx << "\n";
    file << "posy = " << posy << "\n";

    file << "\nattributes:\n";
    while (gtk_clist_get_text (GTK_CLIST (attribute_list), i, 0, &str))
    {
        file << str << " = ";
        gtk_clist_get_text (GTK_CLIST (attribute_list), i++, 1, &str);
        file << str << "\n";
    }

    i = 0;
    file << "\nevents:\n";
    while (gtk_clist_get_text (GTK_CLIST (event_list), i, 0, &str))
    {
        file << "type = " << str << "\n";
        gtk_clist_get_text (GTK_CLIST (event_list), i, 1, &str);
        file << "script = " << str << "\n";
        gtk_clist_get_text (GTK_CLIST (event_list), i++, 2, &str);
        if (str != NULL && str[0] != '\0') file << "parameters = " << str << "\n";
    }

    file << "\nactions:\n";
    file << "schedule = " << schedule << "\n";
    file << "dialogue = " << dialogue << "\n";

    // clean up
    file.close ();
    g_free (tmp);
    g_free (fname);
}

void
main_wnd::read_character_source (gchar *fname)
{
    ifstream file;
    gchar **vals, *dummy[3] = { "", "", "" };
    char str[256];
    int mode = 0, i = 0, j;

    file.open (fname);
    if (!file)
    {
        create_warning ("    Load failed!");
        gtk_main ();
        return;
    }

    gtk_clist_freeze (GTK_CLIST (attribute_list));
    gtk_clist_freeze (GTK_CLIST (event_list));
    gtk_clist_clear (GTK_CLIST (attribute_list));
    gtk_clist_clear (GTK_CLIST (event_list));

    // read file line by line
    while (!file.eof ())
    {
        file.getline (str, 255);
        g_strstrip (str);

        // ignore comments or blank lines
        if (str != NULL && str[0] != '#' && str[0] != '\0')
        {
            if (strcmp (str, "basic:") == 0)
            {
                mode = 1;
                continue;
            }
            if (strcmp (str, "attributes:") == 0)
            {
                mode = 2;
                continue;
            }
            if (strcmp (str, "events:") == 0)
            {
                mode = 3;
                continue;
            }
            if (strcmp (str, "actions:") == 0)
            {
                mode = 4;
                continue;
            }

            vals = g_strsplit (str, "=", 1);
            g_strstrip (vals[0]);
            g_strstrip (vals[1]);

            switch (mode)
            {
                case 1:
                {
                    if (strcmp (vals[0], "name") == 0)
                        gtk_entry_set_text (GTK_ENTRY (name_entry), vals[1]);
                    if (strcmp (vals[0], "race") == 0)
                        set_option (GTK_OPTION_MENU (race_choice), vals[1]);
                    if (strcmp (vals[0], "gender") == 0)
                        set_option (GTK_OPTION_MENU (gender_choice), vals[1]);
                    if (strcmp (vals[0], "posx") == 0)
                        posx = strdup (vals[1]);
                    if (strcmp (vals[0], "posy") == 0)
                        posy = strdup (vals[1]);

                    break;
                }
                case 2:
                {
                    gtk_clist_append (GTK_CLIST (attribute_list), vals);

                    break;
                }
                case 3:
                {
                    if (strcmp (vals[0], "type") == 0)
                    {
                        gtk_clist_append (GTK_CLIST (event_list), dummy);
                        gtk_clist_set_text (GTK_CLIST (event_list), i, 0, vals[1]);
                    }
                    if (strcmp (vals[0], "script") == 0)
                        gtk_clist_set_text (GTK_CLIST (event_list), i, 1, vals[1]);
                    if (strcmp (vals[0], "parameters") == 0)
                        gtk_clist_set_text (GTK_CLIST (event_list), i++, 2, vals[1]);

                    break;
                }
                case 4:
                {
                    if (strcmp (vals[0], "dialogue") == 0)
                        gtk_entry_set_text (GTK_ENTRY (dlg_entry), vals[1]);
                    if (strcmp (vals[0], "schedule") == 0)
                        gtk_entry_set_text (GTK_ENTRY (scl_entry), vals[1]);
                        
                    break;
                }
            }

            g_strfreev (vals);
        }
    }

    colorify_list (GTK_CLIST (attribute_list));
    colorify_list (GTK_CLIST (event_list));
    gtk_clist_thaw (GTK_CLIST (attribute_list));
    gtk_clist_thaw (GTK_CLIST (event_list));
}

gchar *
main_wnd::get_script ()
{
    GString *file = g_string_new ("");
    gchar *script;
    int i, j;

    GtkWidget *fs = create_fileselection (file, false);

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, last_dir);

    // chose file
    gtk_widget_show (fs);
    gtk_main ();

    // remember the directory we're in
    last_dir = g_dirname (file->str);

    // just return the plain filename without path and extension
    script = g_basename (file->str);

    for (j = i = strlen (script); i > 0; i--)
        if (script[i] == '.')
        {
            j = i;
            i = 0;
        }

    script = g_strndup (script, j);
    g_string_free (file, TRUE);

    return script;
}

event_wnd::event_wnd (main_wnd * wnd, gchar * event)
{
    main = wnd;
    ok = false;
    script = "";
    params = "";

    condition_list_sel = -1;

    GtkWidget *event_dlg = create_event_wnd (this, event);

    gtk_widget_show (event_dlg);
    gtk_window_set_modal (GTK_WINDOW (event_dlg), TRUE);
}
