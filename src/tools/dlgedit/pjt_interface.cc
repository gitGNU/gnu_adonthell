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

#include "pjt_callbacks.h"
#include "pjt_interface.h"

GtkWidget *
create_project_window ()
{
    GtkWidget *project_window;
    GtkWidget *vbox2;
    GtkWidget *menubar1;
    GtkWidget *project;
    GtkWidget *project_menu;
    GtkAccelGroup *project_menu_accels;
    GtkWidget *project_load;
    GtkWidget *project_save;
    GtkWidget *project_make;
    GtkWidget *separator1;
    GtkWidget *project_close;
    GtkWidget *hbox1;
    GtkWidget *scrolledwindow1;
    GtkWidget *viewport1;
    GtkWidget *project_tree;
    GtkWidget *notebook1;
    GtkWidget *scrolledwindow2;
    GtkWidget *project_item_txt;
    GtkWidget *label1;
    GtkWidget *vbox5;
    GtkWidget *import_npc;
    GtkWidget *npc_frame;
    GtkWidget *vbox6;
    GtkWidget *label7;
    GtkWidget *hseparator2;
    GtkWidget *add_npc;
    GtkWidget *label2;
    GtkWidget *vbox3;
    GtkWidget *import_dlg;
    GtkWidget *dlg_frame;
    GtkWidget *vbox4;
    GtkWidget *label6;
    GtkWidget *hbox2;
    GtkWidget *label5;
    GtkWidget *assign_dlg;
    GtkWidget *assign_dlg_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *hseparator1;
    GtkWidget *add_dlg;
    GtkWidget *label3;

    project_window = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (project_window), "project_window", project_window);
    gtk_window_set_title (GTK_WINDOW (project_window), "Adonthell Dialogue Project - [new_project]");
    gtk_window_set_position (GTK_WINDOW (project_window), GTK_WIN_POS_MOUSE);
    gtk_window_set_default_size (GTK_WINDOW (project_window), 500, 300);

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox2);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "vbox2", vbox2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (project_window), vbox2);

    menubar1 = gtk_menu_bar_new ();
    gtk_widget_ref (menubar1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "menubar1", menubar1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (menubar1);
    gtk_box_pack_start (GTK_BOX (vbox2), menubar1, FALSE, FALSE, 0);

    project = gtk_menu_item_new_with_label ("Project");
    gtk_widget_ref (project);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project", project,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project);
    gtk_container_add (GTK_CONTAINER (menubar1), project);

    project_menu = gtk_menu_new ();
    gtk_widget_ref (project_menu);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_menu", project_menu,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (project), project_menu);
    project_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (project_menu));

    project_load = gtk_menu_item_new_with_label ("Load");
    gtk_widget_ref (project_load);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_load", project_load,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_load);
    gtk_container_add (GTK_CONTAINER (project_menu), project_load);

    project_save = gtk_menu_item_new_with_label ("Save");
    gtk_widget_ref (project_save);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_save", project_save,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_save);
    gtk_container_add (GTK_CONTAINER (project_menu), project_save);

    separator1 = gtk_menu_item_new ();
    gtk_widget_ref (separator1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "separator1", separator1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (separator1);
    gtk_container_add (GTK_CONTAINER (project_menu), separator1);
    gtk_widget_set_sensitive (separator1, FALSE);

    project_make = gtk_menu_item_new_with_label ("Make");
    gtk_widget_ref (project_make);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_make", project_make,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_make);
    gtk_container_add (GTK_CONTAINER (project_menu), project_make);

    separator1 = gtk_menu_item_new ();
    gtk_widget_ref (separator1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "separator1", separator1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (separator1);
    gtk_container_add (GTK_CONTAINER (project_menu), separator1);
    gtk_widget_set_sensitive (separator1, FALSE);

    project_close = gtk_menu_item_new_with_label ("Close");
    gtk_widget_ref (project_close);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_close", project_close,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_close);
    gtk_container_add (GTK_CONTAINER (project_menu), project_close);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_ref (hbox1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "hbox1", hbox1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

    scrolledwindow1 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "scrolledwindow1", scrolledwindow1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (hbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_widget_set_usize (scrolledwindow1, 200, -2);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 4);

    viewport1 = gtk_viewport_new (0, 0);
    gtk_widget_ref (viewport1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "viewport1", viewport1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (viewport1);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);

    project_tree = gtk_tree_new ();
    gtk_widget_ref (project_tree);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_tree", project_tree,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_tree);
    gtk_container_add (GTK_CONTAINER (viewport1), project_tree);

    notebook1 = gtk_notebook_new ();
    gtk_widget_ref (notebook1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "notebook1", notebook1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (notebook1);
    gtk_box_pack_start (GTK_BOX (hbox1), notebook1, TRUE, TRUE, 0);

    scrolledwindow2 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow2);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "scrolledwindow2", scrolledwindow2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow2);
    gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow2);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow2), 5);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    project_item_txt = gtk_text_new (0, 0);
    gtk_widget_ref (project_item_txt);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "project_item_txt", project_item_txt,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project_item_txt);
    gtk_container_add (GTK_CONTAINER (scrolledwindow2), project_item_txt);
    gtk_text_set_editable (GTK_TEXT (project_item_txt), TRUE);

    label1 = gtk_label_new ("Personal Notes");
    gtk_widget_ref (label1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label1", label1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label1);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label1);

    vbox5 = gtk_vbox_new (FALSE, 2);
    gtk_widget_ref (vbox5);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "vbox5", vbox5,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox5);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox5), 4);

    import_npc = gtk_button_new_with_label ("Import Character ...");
    gtk_widget_ref (import_npc);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "import_npc", import_npc,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (import_npc);
    gtk_box_pack_start (GTK_BOX (vbox5), import_npc, FALSE, TRUE, 0);

    npc_frame = gtk_frame_new ("No Character selected");
    gtk_widget_ref (npc_frame);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "npc_frame", npc_frame,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_frame);
    gtk_box_pack_start (GTK_BOX (vbox5), npc_frame, TRUE, TRUE, 0);
    gtk_widget_set_sensitive (npc_frame, FALSE);

    vbox6 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox6);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "vbox6", vbox6,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox6);
    gtk_container_add (GTK_CONTAINER (npc_frame), vbox6);
    gtk_container_set_border_width (GTK_CONTAINER (vbox6), 4);

    label7 = gtk_label_new ("Further Character information\nwill be displayed here ...");
    gtk_widget_ref (label7);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label7", label7,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label7);
    gtk_box_pack_start (GTK_BOX (vbox6), label7, TRUE, TRUE, 0);
    gtk_label_set_line_wrap (GTK_LABEL (label7), TRUE);

    hseparator2 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator2);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "hseparator2", hseparator2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator2);
    gtk_box_pack_start (GTK_BOX (vbox6), hseparator2, FALSE, TRUE, 2);

    add_npc = gtk_button_new_with_label ("Add to Project");
    gtk_widget_ref (add_npc);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "add_npc", add_npc,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (add_npc);
    gtk_box_pack_start (GTK_BOX (vbox6), add_npc, FALSE, FALSE, 0);
    gtk_widget_set_sensitive (add_npc, FALSE);

    label2 = gtk_label_new ("Character");
    gtk_widget_ref (label2);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label2", label2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label2);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);

    vbox3 = gtk_vbox_new (FALSE, 2);
    gtk_widget_ref (vbox3);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "vbox3", vbox3,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox3);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox3);
    gtk_container_set_border_width (GTK_CONTAINER (vbox3), 4);

    import_dlg = gtk_button_new_with_label ("Import Dialogue ...");
    gtk_widget_ref (import_dlg);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "import_dlg", import_dlg,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (import_dlg);
    gtk_box_pack_start (GTK_BOX (vbox3), import_dlg, FALSE, TRUE, 0);

    dlg_frame = gtk_frame_new ("No Dialogue selected");
    gtk_widget_ref (dlg_frame);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "dlg_frame", dlg_frame,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dlg_frame);
    gtk_box_pack_start (GTK_BOX (vbox3), dlg_frame, TRUE, TRUE, 0);
    gtk_widget_set_sensitive (dlg_frame, FALSE);

    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox4);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "vbox4", vbox4,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (dlg_frame), vbox4);
    gtk_container_set_border_width (GTK_CONTAINER (vbox4), 4);

    label6 = gtk_label_new ("Further Dialogue customization\nfollows ...");
    gtk_widget_ref (label6);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label6", label6,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label6);
    gtk_box_pack_start (GTK_BOX (vbox4), label6, TRUE, TRUE, 0);
    gtk_label_set_line_wrap (GTK_LABEL (label6), TRUE);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_ref (hbox2);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "hbox2", hbox2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox4), hbox2, FALSE, TRUE, 0);

    label5 = gtk_label_new ("Assign to ");
    gtk_widget_ref (label5);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label5", label5,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label5);
    gtk_box_pack_start (GTK_BOX (hbox2), label5, FALSE, FALSE, 0);
    gtk_misc_set_padding (GTK_MISC (label5), 5, 0);

    assign_dlg = gtk_option_menu_new ();
    gtk_widget_ref (assign_dlg);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "assign_dlg", assign_dlg,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (assign_dlg);
    gtk_box_pack_start (GTK_BOX (hbox2), assign_dlg, TRUE, TRUE, 0);
    assign_dlg_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (assign_dlg_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (assign_dlg), assign_dlg_menu);

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "hseparator1", hseparator1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox4), hseparator1, FALSE, TRUE, 2);

    add_dlg = gtk_button_new_with_label ("Add to Project");
    gtk_widget_ref (add_dlg);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "add_dlg", add_dlg,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (add_dlg);
    gtk_box_pack_start (GTK_BOX (vbox4), add_dlg, FALSE, FALSE, 0);
    gtk_widget_set_sensitive (add_dlg, FALSE);

    label3 = gtk_label_new ("Dialogue");
    gtk_widget_ref (label3);
    gtk_object_set_data_full (GTK_OBJECT (project_window), "label3", label3,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label3);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label3);

    gtk_signal_connect (GTK_OBJECT (project), "activate",
        GTK_SIGNAL_FUNC (on_project_activate),
        NULL);
    gtk_signal_connect (GTK_OBJECT (project_load), "activate",
        GTK_SIGNAL_FUNC (on_project_load_activate),
        NULL);
    gtk_signal_connect (GTK_OBJECT (project_save), "activate",
        GTK_SIGNAL_FUNC (on_project_save_activate),
        NULL);
    gtk_signal_connect (GTK_OBJECT (project_make), "activate",
        GTK_SIGNAL_FUNC (on_project_make_activate),
        NULL);
    gtk_signal_connect (GTK_OBJECT (project_close), "activate",
        GTK_SIGNAL_FUNC (on_project_close_activate),
        NULL);
    gtk_signal_connect (GTK_OBJECT (project_tree), "selection_changed",
        GTK_SIGNAL_FUNC (on_project_tree_selection_changed),
        NULL);
    gtk_signal_connect (GTK_OBJECT (import_npc), "clicked",
        GTK_SIGNAL_FUNC (on_import_npc_clicked),
        NULL);
    gtk_signal_connect (GTK_OBJECT (add_npc), "clicked",
        GTK_SIGNAL_FUNC (on_add_npc_clicked),
        NULL);
    gtk_signal_connect (GTK_OBJECT (import_dlg), "clicked",
        GTK_SIGNAL_FUNC (on_import_dlg_clicked),
        NULL);
    gtk_signal_connect (GTK_OBJECT (add_dlg), "clicked",
        GTK_SIGNAL_FUNC (on_add_dlg_clicked),
        NULL);

    return project_window;
}
