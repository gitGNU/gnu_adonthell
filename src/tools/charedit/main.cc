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

    gtk_widget_show (app);
    gtk_main ();

    return 0;
}

main_wnd::main_wnd ()
{
    attribute_list_sel = -1;
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
        fgets (str, 255, f);      
        g_strstrip (str);
        
        if (str != NULL && str[0] != '#' && str[0] != '\0')
        {
            text[0] = str;
            gtk_clist_append (GTK_CLIST (attribute_list), text);
        }
    }

    fclose (f);
}
