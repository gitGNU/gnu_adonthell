/*
   $Id$

   (C) Copyright 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#include <gtk/gtk.h>

#include "ps_dlg.h"
#include "ps_callbacks.h"
#include "ps_interface.h"
#include "../../python_class.h"

extern gchar *get_option (GtkOptionMenu*);

void
on_ps_ok_pressed (GtkButton * button, gpointer user_data)
{
    int i, race = 0, gender = 0;
    char *the_npc;
    ps_dlg* dlg = (ps_dlg *) user_data;
    GSList* group = gtk_radio_button_group (dlg->race);

    for (i = 0; i < 4; i++)
        if (gtk_toggle_button_get_active ((GtkToggleButton *) g_slist_nth_data (group, i)))
            race = i;

    group = gtk_radio_button_group (dlg->gender);
    for (i = 0; i < 2; i++)
        if (gtk_toggle_button_get_active ((GtkToggleButton *) g_slist_nth_data (group, i)))
            gender = i;     

    the_npc = get_option (GTK_OPTION_MENU (dlg->npc_menu));
    PyDict_SetItemString (data::globals, "the_npc", 
        python::pass_instance (data::characters[the_npc], "character"));

    dlg->on_ok (gtk_entry_get_text (dlg->name), race, gender, the_npc);

    gtk_widget_destroy (dlg->wnd);
    gtk_main_quit ();
}


void
on_ps_cancel_pressed (GtkButton * button, gpointer user_data)
{
    // clean up
    gtk_main_quit ();
    gtk_widget_destroy ((GtkWidget *) user_data);
}
