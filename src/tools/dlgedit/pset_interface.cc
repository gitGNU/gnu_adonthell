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

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "preset.h"
#include "callbacks.h"
#include "pset_callbacks.h"
#include "pset_interface.h"

GtkWidget *
create_preset_dlg (preset_dlg *dlg)
{
    GtkWidget *preset_dlg;
    GtkWidget *vbox1;
    GtkWidget *scrolledwindow1;
    GtkWidget *preset_entry;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *preset_ok;
    GtkWidget *preset_cancel;
    GtkTooltips *tooltips;

    tooltips = gtk_tooltips_new ();

    preset_dlg = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (preset_dlg), "preset_dlg", preset_dlg);
    gtk_widget_set_usize (preset_dlg, 400, 320);
    gtk_window_set_modal (GTK_WINDOW (preset_dlg), TRUE);
    gtk_window_set_title (GTK_WINDOW (preset_dlg), "Preset Variables");
    gtk_window_set_position (GTK_WINDOW (preset_dlg), GTK_WIN_POS_MOUSE);
    gtk_window_set_policy (GTK_WINDOW (preset_dlg), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (preset_dlg), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    preset_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (preset_entry);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "preset_entry", preset_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (preset_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), preset_entry);
    gtk_tooltips_set_tip (tooltips, preset_entry, "Here you can assign values to variables that will be available when testing the Dialogue.", NULL);
    gtk_text_set_editable (GTK_TEXT (preset_entry), TRUE);
    gtk_text_insert (GTK_TEXT (preset_entry), preset_entry->style->font,
        &preset_entry->style->black, &preset_entry->style->white, dlg->vars.c_str (), -1);

    dlg->entry = preset_entry;

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 2);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    preset_ok = gtk_button_new_with_label ("OK");
    gtk_widget_ref (preset_ok);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "preset_ok", preset_ok, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (preset_ok);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), preset_ok);
    GTK_WIDGET_SET_FLAGS (preset_ok, GTK_CAN_DEFAULT);

    preset_cancel = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (preset_cancel);
    gtk_object_set_data_full (GTK_OBJECT (preset_dlg), "preset_cancel", preset_cancel, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (preset_cancel);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), preset_cancel);
    GTK_WIDGET_SET_FLAGS (preset_cancel, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (preset_ok), "pressed", GTK_SIGNAL_FUNC (on_preset_ok_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (preset_cancel), "pressed", GTK_SIGNAL_FUNC (on_preset_cancel_pressed), preset_dlg);
    gtk_signal_connect (GTK_OBJECT (preset_dlg), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    gtk_widget_grab_focus (preset_entry);
    gtk_widget_grab_default (preset_ok);
    gtk_object_set_data (GTK_OBJECT (preset_dlg), "tooltips", tooltips);
    gtk_widget_show (preset_dlg);

    return preset_dlg;
}
