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

#include "dbg_callbacks.h"
#include "dbg_interface.h"

GtkWidget *
create_debug_wnd (debug_dlg *dlg)
{
    GtkWidget *debug_wnd;
    GtkWidget *vbox1;
    GtkWidget *notebook1;
    GtkWidget *scrolledwindow1;
    GtkWidget *character_tree;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label1;
    GtkWidget *scrolledwindow2;
    GtkWidget *quest_tree;
    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *label2;
    GtkWidget *hbuttonbox1;
    GtkWidget *update_debug;
    GtkWidget *close_debug;

    debug_wnd = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (debug_wnd), "debug_wnd", debug_wnd);
    gtk_widget_set_usize (debug_wnd, 400, 320);
    gtk_window_set_title (GTK_WINDOW (debug_wnd), "Debug");
    gtk_window_set_position (GTK_WINDOW (debug_wnd), GTK_WIN_POS_MOUSE);
    gtk_window_set_policy (GTK_WINDOW (debug_wnd), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (debug_wnd), vbox1);

    notebook1 = gtk_notebook_new ();
    gtk_widget_ref (notebook1);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "notebook1", notebook1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (notebook1);
    gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);

    scrolledwindow1 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow1);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    character_tree = gtk_ctree_new (2, 0);
    gtk_widget_ref (character_tree);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "character_tree", character_tree, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (character_tree);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), character_tree);
    gtk_clist_set_column_width (GTK_CLIST (character_tree), 0, 80);
    gtk_clist_set_column_width (GTK_CLIST (character_tree), 1, 80);
    gtk_clist_column_titles_hide (GTK_CLIST (character_tree));
    dlg->char_tree = character_tree;
    
    label3 = gtk_label_new ("");
    gtk_widget_ref (label3);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label3", label3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label3);
    gtk_clist_set_column_widget (GTK_CLIST (character_tree), 0, label3);

    label4 = gtk_label_new ("label4");
    gtk_widget_ref (label4);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label4", label4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label4);
    gtk_clist_set_column_widget (GTK_CLIST (character_tree), 1, label4);

    label1 = gtk_label_new ("Characters");
    gtk_widget_ref (label1);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label1", label1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label1);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label1);

    scrolledwindow2 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow2);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "scrolledwindow2", scrolledwindow2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow2);
    gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow2);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    quest_tree = gtk_ctree_new (2, 0);
    gtk_widget_ref (quest_tree);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "quest_tree", quest_tree, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (quest_tree);
    gtk_container_add (GTK_CONTAINER (scrolledwindow2), quest_tree);
    gtk_clist_set_column_width (GTK_CLIST (quest_tree), 0, 80);
    gtk_clist_set_column_width (GTK_CLIST (quest_tree), 1, 80);
    gtk_clist_column_titles_hide (GTK_CLIST (quest_tree));
    dlg->quest_tree = quest_tree;

    label5 = gtk_label_new ("");
    gtk_widget_ref (label5);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label5", label5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label5);
    gtk_clist_set_column_widget (GTK_CLIST (quest_tree), 0, label5);

    label6 = gtk_label_new ("label4");
    gtk_widget_ref (label6);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label6", label6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label6);
    gtk_clist_set_column_widget (GTK_CLIST (quest_tree), 1, label6);

    label2 = gtk_label_new ("Quests");
    gtk_widget_ref (label2);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "label2", label2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label2);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox1), 4);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    update_debug = gtk_button_new_with_label ("Update");
    gtk_widget_ref (update_debug);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "update_debug", update_debug, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (update_debug);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), update_debug);
    GTK_WIDGET_SET_FLAGS (update_debug, GTK_CAN_DEFAULT);

    close_debug = gtk_button_new_with_label ("Close");
    gtk_widget_ref (close_debug);
    gtk_object_set_data_full (GTK_OBJECT (debug_wnd), "close_debug", close_debug, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (close_debug);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), close_debug);
    GTK_WIDGET_SET_FLAGS (close_debug, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (character_tree), "tree_expand", GTK_SIGNAL_FUNC (on_character_tree_expand), NULL);
    gtk_signal_connect (GTK_OBJECT (quest_tree), "tree_expand", GTK_SIGNAL_FUNC (on_quest_tree_expand), NULL);
    gtk_signal_connect (GTK_OBJECT (update_debug), "clicked", GTK_SIGNAL_FUNC (on_update_debug_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (close_debug), "clicked", GTK_SIGNAL_FUNC (on_close_debug_clicked), dlg);

    gtk_widget_show (debug_wnd);
    
    return debug_wnd;
}
