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

#include "main.h"
#include "callbacks.h"
#include "interface.h"

GtkWidget *
create_main_wnd (main_wnd & wnd)
{
    GtkWidget *main_wnd;
    GtkWidget *vbox1;
    GtkWidget *menubar1;
    GtkWidget *file;
    GtkWidget *file_menu;
    GtkAccelGroup *file_menu_accels;
    GtkWidget *open;
    GtkWidget *save;
    GtkWidget *merge;
    GtkWidget *separator1;
    GtkWidget *quit;
    GtkWidget *vbox3;
    GtkWidget *hbox2;
    GtkWidget *label6;
    GtkWidget *name_entry;
    GtkWidget *frame2;
    GtkWidget *vbox2;
    GtkWidget *hbox3;
    GtkWidget *label9;
    GtkWidget *attrib_entry;
    GtkWidget *label10;
    GtkWidget *val_entry;
    GtkWidget *hbuttonbox1;
    GtkWidget *attrib_update;
    GtkWidget *attrib_remove;
    GtkWidget *scrolledwindow1;
    GtkWidget *attribute_list;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkAccelGroup *accel_group;
    GtkTooltips *tooltips;

    tooltips = gtk_tooltips_new ();

    accel_group = gtk_accel_group_new ();

    main_wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_object_set_data (GTK_OBJECT (main_wnd), "main_wnd", main_wnd);
    gtk_widget_set_usize (main_wnd, 500, 360);
    gtk_window_set_title (GTK_WINDOW (main_wnd), "Adonthell Quest Editor");
    gtk_window_set_policy (GTK_WINDOW (main_wnd), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (main_wnd), vbox1);

    menubar1 = gtk_menu_bar_new ();
    gtk_widget_ref (menubar1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "menubar1", menubar1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (menubar1);
    gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

    file = gtk_menu_item_new_with_label ("File");
    gtk_widget_ref (file);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "file", file, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (file);
    gtk_container_add (GTK_CONTAINER (menubar1), file);

    file_menu = gtk_menu_new ();
    gtk_widget_ref (file_menu);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "file_menu", file_menu, (GtkDestroyNotify) gtk_widget_unref);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (file), file_menu);
    file_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (file_menu));

    open = gtk_menu_item_new_with_label ("Open");
    gtk_widget_ref (open);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "open", open, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (open);
    gtk_container_add (GTK_CONTAINER (file_menu), open);
    gtk_widget_add_accelerator (open, "activate", accel_group, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    save = gtk_menu_item_new_with_label ("Save");
    gtk_widget_ref (save);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "save", save, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (save);
    gtk_container_add (GTK_CONTAINER (file_menu), save);
    gtk_widget_add_accelerator (save, "activate", accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    merge = gtk_menu_item_new_with_label ("Merge");
    gtk_widget_ref (merge);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "merge", merge, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (merge);
    gtk_container_add (GTK_CONTAINER (file_menu), merge);
    gtk_widget_add_accelerator (merge, "activate", accel_group, GDK_m, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    separator1 = gtk_menu_item_new ();
    gtk_widget_ref (separator1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "separator1", separator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (separator1);
    gtk_container_add (GTK_CONTAINER (file_menu), separator1);
    gtk_widget_set_sensitive (separator1, FALSE);

    quit = gtk_menu_item_new_with_label ("Quit");
    gtk_widget_ref (quit);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "quit", quit, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (quit);
    gtk_container_add (GTK_CONTAINER (file_menu), quit);
    gtk_widget_add_accelerator (quit, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    vbox3 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox3);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox3", vbox3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox3);
    gtk_container_add (GTK_CONTAINER (vbox1), vbox3);

    hbox2 = gtk_hbox_new (FALSE, 8);
    gtk_widget_ref (hbox2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbox2", hbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_container_set_border_width (GTK_CONTAINER (hbox2), 8);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox3), hbox2, FALSE, TRUE, 0);
    // gtk_container_add (GTK_CONTAINER (vbox3), hbox2);

    label6 = gtk_label_new ("Quest ID");
    gtk_widget_ref (label6);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label6", label6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label6);
    gtk_box_pack_start (GTK_BOX (hbox2), label6, FALSE, FALSE, 0);
    gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);

    name_entry = gtk_entry_new ();
    wnd.name_entry = name_entry;
    gtk_widget_ref (name_entry);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "name_entry", name_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (name_entry);
    gtk_box_pack_start (GTK_BOX (hbox2), name_entry, FALSE, TRUE, 0);
    gtk_widget_set_usize (name_entry, 100, -2);
    gtk_tooltips_set_tip (tooltips, name_entry, "A unique ID that is used to access a Quest object from Python scripts via\nmyQuest = quests[\"<id>\"]", 0);

    frame2 = gtk_frame_new ("Define Variables & Flags");
    gtk_widget_ref (frame2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "frame2", frame2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame2);
    gtk_box_pack_start (GTK_BOX (vbox3), frame2, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame2), 4);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox2", vbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (frame2), vbox2);

    hbox3 = gtk_hbox_new (FALSE, 4);
    gtk_widget_ref (hbox3);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbox3", hbox3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox3);
    gtk_box_pack_start (GTK_BOX (vbox2), hbox3, FALSE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox3), 4);

    label9 = gtk_label_new ("Name");
    gtk_widget_ref (label9);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label9", label9, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label9);
    gtk_box_pack_start (GTK_BOX (hbox3), label9, FALSE, FALSE, 0);

    attrib_entry = gtk_entry_new ();
    wnd.attrib_entry = attrib_entry;
    gtk_widget_ref (attrib_entry);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attrib_entry", attrib_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attrib_entry);
    gtk_box_pack_start (GTK_BOX (hbox3), attrib_entry, TRUE, TRUE, 0);
    gtk_widget_set_usize (attrib_entry, 140, -2);
    gtk_tooltips_set_tip (tooltips, attrib_entry, "The Variable. In scripts variables can be accessed with quest.get(\"<variable>\")", 0);

    label10 = gtk_label_new ("Value");
    gtk_widget_ref (label10);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label10", label10, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label10);
    gtk_box_pack_start (GTK_BOX (hbox3), label10, FALSE, FALSE, 0);

    val_entry = gtk_entry_new ();
    wnd.val_entry = val_entry;
    gtk_widget_ref (val_entry);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "val_entry", val_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (val_entry);
    gtk_box_pack_start (GTK_BOX (hbox3), val_entry, TRUE, TRUE, 0);
    gtk_widget_set_usize (val_entry, 50, -2);
    gtk_tooltips_set_tip (tooltips, val_entry, "The variable's initial value, an integer", 0);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (hbox3), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    attrib_update = gtk_button_new_with_label ("Add / Update");
    gtk_widget_ref (attrib_update);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attrib_update", attrib_update, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attrib_update);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), attrib_update);
    GTK_WIDGET_SET_FLAGS (attrib_update, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, attrib_update, "Add a new Variable to the list or update a existing one", 0);

    attrib_remove = gtk_button_new_with_label ("Remove");
    gtk_widget_ref (attrib_remove);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attrib_remove", attrib_remove, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attrib_remove);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), attrib_remove);
    GTK_WIDGET_SET_FLAGS (attrib_remove, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, attrib_remove, "Remove selected Variable from the list", 0);

    scrolledwindow1 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    attribute_list = gtk_clist_new (2);
    wnd.attribute_list = attribute_list;
    gtk_widget_ref (attribute_list);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attribute_list", attribute_list, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attribute_list);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), attribute_list);
    gtk_clist_set_column_width (GTK_CLIST (attribute_list), 0, 221);
    gtk_clist_set_column_width (GTK_CLIST (attribute_list), 1, 80);
    gtk_clist_column_titles_show (GTK_CLIST (attribute_list));
    gtk_clist_column_titles_passive (GTK_CLIST (attribute_list));

    label4 = gtk_label_new ("Name");
    gtk_widget_ref (label4);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label4", label4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label4);
    gtk_clist_set_column_widget (GTK_CLIST (attribute_list), 0, label4);

    label5 = gtk_label_new ("Value");
    gtk_widget_ref (label5);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label5", label5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label5);
    gtk_clist_set_column_widget (GTK_CLIST (attribute_list), 1, label5);

    gtk_signal_connect (GTK_OBJECT (main_wnd), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (open), "activate", GTK_SIGNAL_FUNC (on_open_activate), &wnd);
    gtk_signal_connect (GTK_OBJECT (save), "activate", GTK_SIGNAL_FUNC (on_save_activate), &wnd);
    gtk_signal_connect (GTK_OBJECT (merge), "activate", GTK_SIGNAL_FUNC (on_merge_activate), &wnd);
    gtk_signal_connect (GTK_OBJECT (quit), "activate", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (attrib_update), "clicked", GTK_SIGNAL_FUNC (on_attrib_update_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (attrib_remove), "clicked", GTK_SIGNAL_FUNC (on_attrib_remove_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (attribute_list), "select_row", GTK_SIGNAL_FUNC (on_attribute_list_select_row), &wnd);
    gtk_signal_connect (GTK_OBJECT (attribute_list), "unselect_row", GTK_SIGNAL_FUNC (on_attribute_list_unselect_row), &wnd);

    gtk_object_set_data (GTK_OBJECT (main_wnd), "tooltips", tooltips);

    gtk_window_add_accel_group (GTK_WINDOW (main_wnd), accel_group);

    return main_wnd;
}

GtkWidget *
create_fileselection (GString * file, bool fileops)
{
    GtkWidget *fileselection1;
    GtkWidget *fs_cancel_button;
    GtkWidget *fs_ok_button;

    fileselection1 = gtk_file_selection_new ("Chose File");
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fileselection1", fileselection1);
    gtk_container_set_border_width (GTK_CONTAINER (fileselection1), 10);
    GTK_WINDOW (fileselection1)->type = GTK_WINDOW_DIALOG;
    gtk_window_set_modal (GTK_WINDOW (fileselection1), TRUE);
    gtk_window_set_policy (GTK_WINDOW (fileselection1), FALSE, FALSE, FALSE);

    if (!fileops)
        gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (fileselection1));

    fs_cancel_button = GTK_FILE_SELECTION (fileselection1)->cancel_button;
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fs_cancel_button", fs_cancel_button);
    gtk_widget_show (fs_cancel_button);

    fs_ok_button = GTK_FILE_SELECTION (fileselection1)->ok_button;
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fs_ok_button", fs_ok_button);
    gtk_widget_show (fs_ok_button);
    GTK_WIDGET_SET_FLAGS (fs_ok_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (fs_cancel_button), "clicked", GTK_SIGNAL_FUNC (on_fs_cancel_button_pressed), NULL);
    gtk_signal_connect (GTK_OBJECT (fs_ok_button), "clicked", GTK_SIGNAL_FUNC (on_fs_ok_button_pressed), file);

    gtk_widget_grab_focus (fs_ok_button);
    gtk_widget_grab_default (fs_ok_button);

    return fileselection1;
}

GtkWidget *
create_warning (gchar * message)
{
    GtkWidget *warning;
    GtkWidget *vbox1;
    GtkWidget *label;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *warning_close;

    warning = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (warning), "warning", warning);
    gtk_window_set_title (GTK_WINDOW (warning), "Warning");
    gtk_window_set_position (GTK_WINDOW (warning), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (warning), TRUE);
    gtk_window_set_policy (GTK_WINDOW (warning), FALSE, FALSE, TRUE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (warning), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (warning), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

    label = gtk_label_new (message);
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (warning), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (vbox1), label, TRUE, TRUE, 0);
    gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_misc_set_padding (GTK_MISC (label), 10, 10);

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (warning), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 4);
    gtk_widget_set_usize (hseparator1, 200, -2);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (warning), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, FALSE, 0);

    warning_close = gtk_button_new_with_label ("Close");
    gtk_widget_ref (warning_close);
    gtk_object_set_data_full (GTK_OBJECT (warning), "warning_close", warning_close, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (warning_close);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), warning_close);
    GTK_WIDGET_SET_FLAGS (warning_close, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (warning), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (warning_close), "clicked", GTK_SIGNAL_FUNC (on_warning_close_clicked), warning);

    gtk_widget_grab_focus (warning_close);
    gtk_widget_grab_default (warning_close);

    gtk_widget_show (warning);

    return warning;
}
