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
#include "callbacks.h"

GtkWidget *
create_debug_wnd (debug_dlg * dlg)
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
    gtk_clist_set_column_width (GTK_CLIST (character_tree), 0, 200);
    gtk_clist_set_column_width (GTK_CLIST (character_tree), 1, 100);
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
    gtk_clist_set_column_width (GTK_CLIST (quest_tree), 0, 200);
    gtk_clist_set_column_width (GTK_CLIST (quest_tree), 1, 100);
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
    gtk_signal_connect (GTK_OBJECT (character_tree), "select_row", GTK_SIGNAL_FUNC (on_character_tree_select), dlg);
    gtk_signal_connect (GTK_OBJECT (quest_tree), "tree_expand", GTK_SIGNAL_FUNC (on_quest_tree_expand), NULL);
    gtk_signal_connect (GTK_OBJECT (update_debug), "clicked", GTK_SIGNAL_FUNC (on_update_debug_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (close_debug), "clicked", GTK_SIGNAL_FUNC (on_close_debug_clicked), dlg);
    gtk_signal_connect (GTK_OBJECT (debug_wnd), "destroy", GTK_SIGNAL_FUNC (on_debug_destroy), dlg);

    gtk_widget_show (debug_wnd);

    return debug_wnd;
}

GtkWidget *
create_dbg_edit_wnd (debug_dlg * dlg, char *title, char *attribute, char *value)
{
    GtkWidget *dbg_edit_wnd;
    GtkWidget *vbox1;
    GtkWidget *table1;
    GtkWidget *dbg_val_entry;
    GtkWidget *dbg_attrib_entry;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *dbg_edit_ok;
    GtkWidget *dbg_edit_cancel;

    dbg_edit_wnd = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (dbg_edit_wnd), "dbg_edit_wnd", dbg_edit_wnd);
    gtk_window_set_title (GTK_WINDOW (dbg_edit_wnd), title);
    gtk_window_set_position (GTK_WINDOW (dbg_edit_wnd), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (dbg_edit_wnd), TRUE);
    gtk_window_set_policy (GTK_WINDOW (dbg_edit_wnd), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (dbg_edit_wnd), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

    table1 = gtk_table_new (2, 2, FALSE);
    gtk_widget_ref (table1);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "table1", table1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (table1);
    gtk_box_pack_start (GTK_BOX (vbox1), table1, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (table1), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table1), 4);

    dbg_val_entry = gtk_entry_new ();
    gtk_widget_ref (dbg_val_entry);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "dbg_val_entry", dbg_val_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dbg_val_entry);
    gtk_table_attach (GTK_TABLE (table1), dbg_val_entry, 1, 2, 1, 2, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    if (value != NULL)
    {
        gtk_entry_set_text (GTK_ENTRY (dbg_val_entry), value);        
    }
    dlg->val_entry = dbg_val_entry;
    
    dbg_attrib_entry = gtk_entry_new ();
    gtk_widget_ref (dbg_attrib_entry);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "dbg_attrib_entry", dbg_attrib_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dbg_attrib_entry);
    gtk_table_attach (GTK_TABLE (table1), dbg_attrib_entry, 1, 2, 0, 1, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    if (attribute != NULL)
    {
        gtk_entry_set_text (GTK_ENTRY (dbg_attrib_entry), attribute);
        gtk_widget_set_sensitive (dbg_attrib_entry, FALSE);
    }
    dlg->attrib_entry = dbg_attrib_entry;

    label2 = gtk_label_new ("Attribute");
    gtk_widget_ref (label2);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "label2", label2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label2);
    gtk_table_attach (GTK_TABLE (table1), label2, 0, 1, 0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 4, 0);
    gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

    label3 = gtk_label_new ("Value");
    gtk_widget_ref (label3);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "label3", label3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label3);
    gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 1, 2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), (GtkAttachOptions) (0), 4, 0);
    gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 0);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    dbg_edit_ok = gtk_button_new_with_label ("OK");
    gtk_widget_ref (dbg_edit_ok);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "dbg_edit_ok", dbg_edit_ok, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dbg_edit_ok);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), dbg_edit_ok);
    GTK_WIDGET_SET_FLAGS (dbg_edit_ok, GTK_CAN_DEFAULT);

    dbg_edit_cancel = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (dbg_edit_cancel);
    gtk_object_set_data_full (GTK_OBJECT (dbg_edit_wnd), "dbg_edit_cancel", dbg_edit_cancel, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dbg_edit_cancel);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), dbg_edit_cancel);
    GTK_WIDGET_SET_FLAGS (dbg_edit_cancel, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (dbg_edit_ok), "clicked", GTK_SIGNAL_FUNC (on_dbg_edit_ok_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (dbg_edit_cancel), "clicked", GTK_SIGNAL_FUNC (on_dbg_edit_cancel_clicked), dbg_edit_wnd);
    gtk_signal_connect (GTK_OBJECT (dbg_edit_wnd), "destroy", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    gtk_widget_show (dbg_edit_wnd);
    
    return dbg_edit_wnd;
}
