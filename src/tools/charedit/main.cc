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
#include <stdio.h>

#include "main.h"
#include "interface.h"

int
main (int argc, char *argv[])
{
    main_wnd wnd;

    gtk_init (&argc, &argv);

    GtkWidget *app = create_main_wnd (wnd);
    wnd.load_default_attributes ();
    wnd.colorify_list (GTK_CLIST (wnd.attribute_list));

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
}

// load a few default attributes from a file
void main_wnd::load_default_attributes ()
{
    FILE* f = fopen ("attributes.txt", "r");
    if (!f) return;

    char* text[2] = { 0, "0" };
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
            gtk_clist_append (GTK_CLIST (attribute_list), text);
        }
    }

    fclose (f);
}

// set every 2nd row to another color to make looking at stuff easier
void main_wnd::colorify_list (GtkCList *list)
{
    int i = 0;
    gchar *str;

    // A Light blue color
    GdkColor color;
    color.red = 55000;
    color.blue = 65000;
    color.green = 55000;

    // can't think of a better way to find end of list :P
    while (gtk_clist_get_text (list, i++, 0, &str))
        if (!(i % 2)) gtk_clist_set_background (list, i-1, &color);
        else gtk_clist_set_background (list, i-1, &GTK_WIDGET (list)->style->white);
}

// returns selected option
gchar* main_wnd::get_option (GtkOptionMenu *o)
{
    GtkMenu *m = (GtkMenu *) gtk_option_menu_get_menu (o);
    GtkMenuItem *i = (GtkMenuItem *) gtk_menu_get_active (m);
    return (gchar *) gtk_object_get_user_data (GTK_OBJECT (i));
}