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

#include "run.h"
#include "run_callbacks.h"
#include "run_interface.h"
#include "callbacks.h" 


GtkWidget *
create_run_dlg_wnd (run_dlg *dlg)
{
    GtkWidget *run_dlg_wnd;
    GtkWidget *vbox1;
    GtkWidget *scrolledwindow1;
    GtkWidget *dialogue_list;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *dialogue_export;
    GtkWidget *dialogue_close;
    GtkTooltips *tooltips;

    tooltips = gtk_tooltips_new ();

    run_dlg_wnd = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (run_dlg_wnd), "run_dlg_wnd", run_dlg_wnd);
    gtk_widget_set_usize (run_dlg_wnd, 400, 320);
    gtk_window_set_title (GTK_WINDOW (run_dlg_wnd), "Run Dialogue");
    gtk_window_set_position (GTK_WINDOW (run_dlg_wnd), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (run_dlg_wnd), TRUE);
    gtk_window_set_policy (GTK_WINDOW (run_dlg_wnd), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (run_dlg_wnd), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

    scrolledwindow1 = gtk_scrolled_window_new ((GtkAdjustment*)NULL, (GtkAdjustment*)NULL);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    dialogue_list = gtk_list_new ();
    gtk_widget_ref (dialogue_list);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "dialogue_list", dialogue_list, (GtkDestroyNotify) gtk_widget_unref);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolledwindow1), dialogue_list);
    gtk_widget_show (dialogue_list);
    dlg->list = dialogue_list;

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 2);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    dialogue_export = gtk_button_new_with_label ("Export");
    gtk_widget_ref (dialogue_export);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "dialogue_export", dialogue_export, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dialogue_export);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), dialogue_export);
    gtk_widget_set_sensitive (dialogue_export, FALSE);
    GTK_WIDGET_SET_FLAGS (dialogue_export, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, dialogue_export, "Save this Conversation as ASCII text", (char*)NULL);

    dialogue_close = gtk_button_new_with_label ("Close");
    gtk_widget_ref (dialogue_close);
    gtk_object_set_data_full (GTK_OBJECT (run_dlg_wnd), "dialogue_close", dialogue_close, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dialogue_close);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), dialogue_close);
    GTK_WIDGET_SET_FLAGS (dialogue_close, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, dialogue_close, "Close this window", (char*)NULL);

    gtk_signal_connect (GTK_OBJECT (dialogue_list), "select_child", GTK_SIGNAL_FUNC (on_dialogue_list_select_child), dlg);
    gtk_signal_connect (GTK_OBJECT (dialogue_export), "pressed", GTK_SIGNAL_FUNC (on_dialogue_export_pressed), NULL);
    gtk_signal_connect (GTK_OBJECT (dialogue_close), "pressed", GTK_SIGNAL_FUNC (on_dialogue_close_pressed), run_dlg_wnd);
    gtk_signal_connect (GTK_OBJECT (run_dlg_wnd), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    gtk_widget_grab_default (dialogue_close);
    gtk_object_set_data (GTK_OBJECT (run_dlg_wnd), "tooltips", tooltips);

    // set the window modal
    gtk_window_set_modal ((GTK_WINDOW (run_dlg_wnd)), TRUE);
    
    // Show window
    gtk_widget_show (run_dlg_wnd);

    return run_dlg_wnd;
}

GtkWidget* 
create_dlg_list_item (const char *text, int mode, int num)
{
    GtkWidget *label;
    GtkWidget *list_item;
    GdkColor color;
    GtkStyle *style = gtk_style_copy (gtk_widget_get_default_style ());

    // Modes:
    // 1 = Blue (Player) Text, insensitive
    // 2 = Black (NPC) Text, insensitive
    // 3 = Black (Player) Text, sensitive
    // 4 = Red (NPC) Text, insensitive
    // 5 = Red (NPC) Text, sensitive

    switch (mode)
    {
        // Player Text
        case 1:
        {
            color.red = 0;
            color.green = 0;
            color.blue = 35000;
            break;
        }
        // NPC Text
        case 2:
        // Active Player Text
        case 3:
        {
            color.red = 0;
            color.green = 0;
            color.blue = 0;
            break;
        }
        // Active NPC text
        case 4:
        case 5:
        {
            color.red = 65535;
            color.green = 0;
            color.blue = 0;
            break;
        }
    }

    /* Set List_item colors */
    style->fg[GTK_STATE_NORMAL] = color;
    style->bg[GTK_STATE_NORMAL] = style->white;
    style->fg[GTK_STATE_INSENSITIVE] = color;
    style->bg[GTK_STATE_INSENSITIVE] = style->white;
    
    /* create label */    
    label = gtk_label_new (text);
    gtk_widget_set_style (label, style);
    gtk_widget_set_usize (label, 380, 0);
    gtk_label_set_justify ((GtkLabel *) label, GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap ((GtkLabel *) label, TRUE);
    gtk_widget_show (label);

    /* add label to list_item */
    list_item = gtk_list_item_new ();
    gtk_widget_set_style (list_item, style);
    gtk_container_add (GTK_CONTAINER(list_item), label);
    gtk_object_set_user_data (GTK_OBJECT (list_item), GINT_TO_POINTER(num));
    GTK_WIDGET_UNSET_FLAGS (list_item, GTK_CAN_FOCUS);
    if (mode != 3 && mode != 5) gtk_widget_set_sensitive (list_item, FALSE);
    gtk_widget_show (list_item);
    
    return list_item;
}
