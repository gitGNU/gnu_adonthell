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

#include "preset.h"
#include "pset_callbacks.h"
#include "pset_interface.h"


void
on_preset_ok_pressed (GtkButton * button, gpointer user_data)
{
    preset_dlg *dlg = (preset_dlg *) user_data;
    if (!dlg->on_ok (gtk_editable_get_chars ((GtkEditable *) dlg->entry, 0, -1)))
        return;

    // clean up
    gtk_main_quit ();
    gtk_widget_destroy (dlg->dlg);
}


void
on_preset_cancel_pressed (GtkButton * button, gpointer user_data)
{
    // clean up
    gtk_main_quit ();
    gtk_widget_destroy ((GtkWidget *) user_data);
}
