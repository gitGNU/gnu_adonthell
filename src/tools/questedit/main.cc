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

main_wnd::main_wnd ()
{
    attribute_list_sel = -1;
    last_dir = "";
    char_dir = g_get_current_dir ();
    cur_dir = NULL;
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

    while (fgets (str, 255, f));
    {
        // strip all leading/trailing whitespace 
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

// Write all info to a file
void 
main_wnd::write_character_source ()
{
    if (cur_dir == NULL) return;

    gchar *name, *tmp, *fname;
    gchar *str;
    int i = 0;
    ofstream file;

    // Make chosen directory default directory 
    g_free (char_dir);
    char_dir = g_strdup (cur_dir);

    // Get some data
    name = g_strstrip (gtk_entry_get_text (GTK_ENTRY (name_entry)));

    // create filename from chosen directory and the character's name
    fname = g_strdup (name);
    g_strdown (fname);
    g_strdelimit (fname, " ", '_');
    tmp = g_strconcat (cur_dir, fname, ".quest", NULL);

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
    file << "# Adonthell Quest source file\n\n";

    file << "basic:\n";
    file << "id = " << name << "\n";

    file << "\nvariables:\n";
    while (gtk_clist_get_text (GTK_CLIST (attribute_list), i, 0, &str))
    {
        file << str << " = ";
        gtk_clist_get_text (GTK_CLIST (attribute_list), i++, 1, &str);
        file << str << "\n";
    }

    // clean up
    file.close ();
    g_free (tmp);
    g_free (fname);
}

void
main_wnd::read_character_source (gchar *fname)
{
    ifstream file;
    gchar **vals;
    char str[256];
    int mode = 0;

    file.open (fname);
    if (!file)
    {
        create_warning ("    Load failed!");
        gtk_main ();
        return;
    }

    gtk_clist_freeze (GTK_CLIST (attribute_list));
    gtk_clist_clear (GTK_CLIST (attribute_list));

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
            if (strcmp (str, "variables:") == 0)
            {
                mode = 2;
                continue;
            }

            vals = g_strsplit (str, "=", 1);
            g_strstrip (vals[0]);
            g_strstrip (vals[1]);

            switch (mode)
            {
                case 1:
                {
                    if (strcmp (vals[0], "id") == 0)
                        gtk_entry_set_text (GTK_ENTRY (name_entry), vals[1]);

                    break;
                }
                case 2:
                {
                    gtk_clist_append (GTK_CLIST (attribute_list), vals);

                    break;
                }
            }

            g_strfreev (vals);
        }
    }

    colorify_list (GTK_CLIST (attribute_list));
    gtk_clist_thaw (GTK_CLIST (attribute_list));
}
