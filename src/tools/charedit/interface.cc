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

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "main.h"
#include "callbacks.h"
#include "interface.h"

GtkWidget *
create_main_wnd (main_wnd &wnd)
{
    GtkWidget *main_wnd;
    GtkWidget *vbox1;
    GtkWidget *menubar1;
    GtkWidget *file;
    GtkWidget *file_menu;
    GtkAccelGroup *file_menu_accels;
    GtkWidget *open;
    GtkWidget *save;
    GtkWidget *separator1;
    GtkWidget *quit;
    GtkWidget *notebook1;
    GtkWidget *vbox3;
    GtkWidget *frame1;
    GtkWidget *hbox2;
    GtkWidget *label6;
    GtkWidget *name_entry;
    GtkWidget *label7;
    GtkWidget *race_choice;
    GtkWidget *race_choice_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *label8;
    GtkWidget *gender_choice;
    GtkWidget *gender_choice_menu;
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
    GtkWidget *label11;
    GtkWidget *empty_notebook_page;
    GtkWidget *label12;
    GtkWidget *vbox6;
    GtkWidget *hbox6;
    GtkWidget *label26;
    GtkWidget *event_choice;
    GtkWidget *event_choice_menu;
    GtkWidget *hbuttonbox4;
    GtkWidget *event_add;
    GtkWidget *event_remove;
    GtkWidget *event_update;
    GtkWidget *scrolledwindow3;
    GtkWidget *event_list;
    GtkWidget *label23;
    GtkWidget *label24;
    GtkWidget *label25;
    GtkWidget *label13;
    GtkWidget *vbox4;
    GtkWidget *hbuttonbox2;
    GtkWidget *dlg_add;
    GtkWidget *dlg_remove;
    GtkWidget *dlg_default;
    GtkWidget *scrolledwindow2;
    GtkWidget *dlg_list;
    GtkWidget *label16;
    GtkWidget *label17;
    GtkWidget *label14;
    GtkAccelGroup *accel_group;
    GtkTooltips *tooltips;

    tooltips = gtk_tooltips_new ();

    accel_group = gtk_accel_group_new ();

    main_wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_object_set_data (GTK_OBJECT (main_wnd), "main_wnd", main_wnd);
    gtk_widget_set_usize (main_wnd, 500, 360);
    gtk_window_set_title (GTK_WINDOW (main_wnd), "Adonthell Character Editor");
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

    notebook1 = gtk_notebook_new ();
    gtk_widget_ref (notebook1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "notebook1", notebook1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (notebook1);
    gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);

    vbox3 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox3);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox3", vbox3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox3);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox3);

    frame1 = gtk_frame_new ("Required Attributes");
    gtk_widget_ref (frame1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "frame1", frame1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame1);
    gtk_box_pack_start (GTK_BOX (vbox3), frame1, FALSE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (frame1), 4);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_ref (hbox2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbox2", hbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox2);
    gtk_container_add (GTK_CONTAINER (frame1), hbox2);

    label6 = gtk_label_new ("Name");
    gtk_widget_ref (label6);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label6", label6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label6);
    gtk_box_pack_start (GTK_BOX (hbox2), label6, TRUE, TRUE, 0);
    gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);

    name_entry = gtk_entry_new ();
    wnd.name_entry = name_entry;
    gtk_widget_ref (name_entry);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "name_entry", name_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (name_entry);
    gtk_box_pack_start (GTK_BOX (hbox2), name_entry, TRUE, TRUE, 0);
    gtk_widget_set_usize (name_entry, 100, -2);
    gtk_tooltips_set_tip (tooltips, name_entry, "The character's name. Must be unique for each character. In scripts, characters can be accessed with characters[\"<name>\"]", 0);

    label7 = gtk_label_new ("Race");
    gtk_widget_ref (label7);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label7", label7, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label7);
    gtk_box_pack_start (GTK_BOX (hbox2), label7, TRUE, TRUE, 0);
    gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);

    race_choice = gtk_option_menu_new ();
    wnd.race_choice = race_choice;
    gtk_widget_ref (race_choice);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "race_choice", race_choice, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (race_choice);
    gtk_box_pack_start (GTK_BOX (hbox2), race_choice, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, race_choice, "The Characters race", 0);
    race_choice_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label (wnd.races[0]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.races[0]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (race_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.races[1]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.races[1]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (race_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.races[2]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.races[2]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (race_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.races[3]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.races[3]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (race_choice_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (race_choice), race_choice_menu);

    label8 = gtk_label_new ("Gender");
    gtk_widget_ref (label8);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label8", label8, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label8);
    gtk_box_pack_start (GTK_BOX (hbox2), label8, TRUE, TRUE, 0);
    gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);

    gender_choice = gtk_option_menu_new ();
    wnd.gender_choice = gender_choice;
    gtk_widget_ref (gender_choice);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "gender_choice", gender_choice, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (gender_choice);
    gtk_box_pack_start (GTK_BOX (hbox2), gender_choice, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, gender_choice, "The Characters gender", 0);
    gender_choice_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label (wnd.gender[0]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.gender[0]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (gender_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.gender[1]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.gender[1]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (gender_choice_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (gender_choice), gender_choice_menu);

    frame2 = gtk_frame_new ("Misc Attributes & Flags");
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

    label9 = gtk_label_new ("Attribute");
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
    gtk_tooltips_set_tip (tooltips, attrib_entry, "The Attribute. In scripts attributes can be accessed with npc.get(\"<attribute>\")", 0);

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
    gtk_tooltips_set_tip (tooltips, val_entry, "The attribute's value, an integer", 0);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (hbox3), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    attrib_update = gtk_button_new_with_label ("Update");
    gtk_widget_ref (attrib_update);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attrib_update", attrib_update, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attrib_update);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), attrib_update);
    GTK_WIDGET_SET_FLAGS (attrib_update, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, attrib_update, "Add new Attribute to the list or update a existing one", 0);

    attrib_remove = gtk_button_new_with_label ("Remove");
    gtk_widget_ref (attrib_remove);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "attrib_remove", attrib_remove, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (attrib_remove);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), attrib_remove);
    GTK_WIDGET_SET_FLAGS (attrib_remove, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, attrib_remove, "Remove selected Attribute from the list", 0);

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

    label4 = gtk_label_new ("Attribute");
    gtk_widget_ref (label4);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label4", label4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label4);
    gtk_clist_set_column_widget (GTK_CLIST (attribute_list), 0, label4);

    label5 = gtk_label_new ("Value");
    gtk_widget_ref (label5);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label5", label5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label5);
    gtk_clist_set_column_widget (GTK_CLIST (attribute_list), 1, label5);

    label11 = gtk_label_new ("Attributes");
    gtk_widget_ref (label11);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label11", label11, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label11);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label11);

    empty_notebook_page = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (empty_notebook_page);
    gtk_container_add (GTK_CONTAINER (notebook1), empty_notebook_page);

    label12 = gtk_label_new ("Graphics");
    gtk_widget_ref (label12);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label12", label12, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label12);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label12);

    vbox6 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox6);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox6", vbox6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox6);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox6);

    hbox6 = gtk_hbox_new (FALSE, 4);
    gtk_widget_ref (hbox6);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbox6", hbox6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox6);
    gtk_box_pack_start (GTK_BOX (vbox6), hbox6, FALSE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox6), 4);

    label26 = gtk_label_new ("Event");
    gtk_widget_ref (label26);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label26", label26, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label26);
    gtk_box_pack_start (GTK_BOX (hbox6), label26, FALSE, FALSE, 0);

    event_choice = gtk_option_menu_new ();
    wnd.event_choice = event_choice;
    gtk_widget_ref (event_choice);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "event_choice", event_choice, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (event_choice);
    gtk_box_pack_start (GTK_BOX (hbox6), event_choice, TRUE, FALSE, 0);
    event_choice_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label (wnd.events[0]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.events[0]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (event_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.events[1]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.events[1]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (event_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.events[2]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.events[2]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (event_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.events[3]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.events[3]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (event_choice_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label (wnd.events[4]);
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), wnd.events[4]);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (event_choice_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (event_choice), event_choice_menu);

    hbuttonbox4 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox4);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbuttonbox4", hbuttonbox4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox4);
    gtk_box_pack_start (GTK_BOX (hbox6), hbuttonbox4, TRUE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox4), GTK_BUTTONBOX_START);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox4), 0);

    event_add = gtk_button_new_with_label ("Add ...");
    gtk_widget_ref (event_add);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "event_add", event_add, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (event_add);
    gtk_container_add (GTK_CONTAINER (hbuttonbox4), event_add);
    GTK_WIDGET_SET_FLAGS (event_add, GTK_CAN_DEFAULT);

    event_remove = gtk_button_new_with_label ("Remove");
    gtk_widget_ref (event_remove);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "event_remove", event_remove, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (event_remove);
    gtk_container_add (GTK_CONTAINER (hbuttonbox4), event_remove);
    GTK_WIDGET_SET_FLAGS (event_remove, GTK_CAN_DEFAULT);

    event_update = gtk_button_new_with_label ("Update");
    gtk_widget_ref (event_update);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "event_update", event_update, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (event_update);
    gtk_container_add (GTK_CONTAINER (hbuttonbox4), event_update);
    GTK_WIDGET_SET_FLAGS (event_update, GTK_CAN_DEFAULT);

    scrolledwindow3 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow3);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "scrolledwindow3", scrolledwindow3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow3);
    gtk_box_pack_start (GTK_BOX (vbox6), scrolledwindow3, TRUE, TRUE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow3), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    event_list = gtk_clist_new (3);
    wnd.event_list = event_list;
    gtk_widget_ref (event_list);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "event_list", event_list, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (event_list);
    gtk_container_add (GTK_CONTAINER (scrolledwindow3), event_list);
    gtk_clist_set_column_width (GTK_CLIST (event_list), 0, 80);
    gtk_clist_set_column_width (GTK_CLIST (event_list), 1, 80);
    gtk_clist_set_column_width (GTK_CLIST (event_list), 2, 80);
    gtk_clist_column_titles_show (GTK_CLIST (event_list));
    gtk_clist_column_titles_passive (GTK_CLIST (event_list));

    label23 = gtk_label_new ("Event");
    gtk_widget_ref (label23);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label23", label23, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label23);
    gtk_clist_set_column_widget (GTK_CLIST (event_list), 0, label23);

    label24 = gtk_label_new ("Script");
    gtk_widget_ref (label24);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label24", label24, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label24);
    gtk_clist_set_column_widget (GTK_CLIST (event_list), 1, label24);

    label25 = gtk_label_new ("Conditions");
    gtk_widget_ref (label25);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label25", label25, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label25);
    gtk_clist_set_column_widget (GTK_CLIST (event_list), 2, label25);

    label13 = gtk_label_new ("Events");
    gtk_widget_ref (label13);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label13", label13, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label13);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label13);

    vbox4 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox4);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "vbox4", vbox4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox4);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox4);
    gtk_container_set_border_width (GTK_CONTAINER (vbox4), 4);

    hbuttonbox2 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "hbuttonbox2", hbuttonbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox2);
    gtk_box_pack_start (GTK_BOX (vbox4), hbuttonbox2, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox2), 0);
    gtk_button_box_set_child_ipadding (GTK_BUTTON_BOX (hbuttonbox2), 0, 0);

    dlg_add = gtk_button_new_with_label ("Add ...");
    gtk_widget_ref (dlg_add);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "dlg_add", dlg_add, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dlg_add);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), dlg_add);
    GTK_WIDGET_SET_FLAGS (dlg_add, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, dlg_add, "Adds a dialogue to that character. For the dialogue to work, it's script must be located in the <adonthell-data>/scripts/dialogues directory.", 0);

    dlg_remove = gtk_button_new_with_label ("Remove");
    gtk_widget_ref (dlg_remove);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "dlg_remove", dlg_remove, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dlg_remove);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), dlg_remove);
    GTK_WIDGET_SET_FLAGS (dlg_remove, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, dlg_remove, "Removes the selected dialogue", 0);

    dlg_default = gtk_button_new_with_label ("make Default");
    gtk_widget_ref (dlg_default);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "dlg_default", dlg_default, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dlg_default);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), dlg_default);
    gtk_widget_set_usize (dlg_default, 96, -2);
    GTK_WIDGET_SET_FLAGS (dlg_default, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, dlg_default, "Makes the selected dialogue the character's default. It's launched when speaking to the character unless changed by some script within the game.", 0);

    scrolledwindow2 = gtk_scrolled_window_new (0, 0);
    gtk_widget_ref (scrolledwindow2);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "scrolledwindow2", scrolledwindow2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow2);
    gtk_box_pack_start (GTK_BOX (vbox4), scrolledwindow2, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    dlg_list = gtk_clist_new (2);
    wnd.dlg_list = dlg_list;
    gtk_widget_ref (dlg_list);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "dlg_list", dlg_list, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (dlg_list);
    gtk_container_add (GTK_CONTAINER (scrolledwindow2), dlg_list);
    gtk_clist_set_column_width (GTK_CLIST (dlg_list), 0, 235);
    gtk_clist_set_column_width (GTK_CLIST (dlg_list), 1, 80);
    gtk_clist_column_titles_show (GTK_CLIST (dlg_list));
    gtk_clist_column_titles_passive (GTK_CLIST (dlg_list));

    label16 = gtk_label_new ("Dialogue");
    gtk_widget_ref (label16);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label16", label16, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label16);
    gtk_clist_set_column_widget (GTK_CLIST (dlg_list), 0, label16);

    label17 = gtk_label_new ("Default");
    gtk_widget_ref (label17);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label17", label17, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label17);
    gtk_clist_set_column_widget (GTK_CLIST (dlg_list), 1, label17);

    label14 = gtk_label_new ("Dialogues");
    gtk_widget_ref (label14);
    gtk_object_set_data_full (GTK_OBJECT (main_wnd), "label14", label14, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label14);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 3), label14);

    gtk_signal_connect (GTK_OBJECT (main_wnd), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (open), "activate", GTK_SIGNAL_FUNC (on_open_activate), NULL);
    gtk_signal_connect (GTK_OBJECT (save), "activate", GTK_SIGNAL_FUNC (on_save_activate), NULL);
    gtk_signal_connect (GTK_OBJECT (quit), "activate", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (attrib_update), "clicked", GTK_SIGNAL_FUNC (on_attrib_update_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (attrib_remove), "clicked", GTK_SIGNAL_FUNC (on_attrib_remove_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (attribute_list), "select_row", GTK_SIGNAL_FUNC (on_attribute_list_select_row), &wnd);
    gtk_signal_connect (GTK_OBJECT (attribute_list), "unselect_row", GTK_SIGNAL_FUNC (on_attribute_list_unselect_row), &wnd);
    gtk_signal_connect (GTK_OBJECT (event_add), "clicked", GTK_SIGNAL_FUNC (on_event_add_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (event_remove), "clicked", GTK_SIGNAL_FUNC (on_event_remove_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (event_update), "clicked", GTK_SIGNAL_FUNC (on_event_update_clicked), &wnd);
    gtk_signal_connect (GTK_OBJECT (event_list), "select_row", GTK_SIGNAL_FUNC (on_event_list_select_row), &wnd);
    gtk_signal_connect (GTK_OBJECT (event_list), "unselect_row", GTK_SIGNAL_FUNC (on_event_list_unselect_row), &wnd);
    gtk_signal_connect (GTK_OBJECT (dlg_add), "clicked", GTK_SIGNAL_FUNC (on_dlg_add_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (dlg_remove), "clicked", GTK_SIGNAL_FUNC (on_dlg_remove_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (dlg_default), "clicked", GTK_SIGNAL_FUNC (on_dlg_default_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (dlg_list), "select_row", GTK_SIGNAL_FUNC (on_dlg_list_select_row), NULL);
    // gtk_signal_connect (GTK_OBJECT (dlg_list), "unselect_row", GTK_SIGNAL_FUNC (on_dlg_list_unselect_row), NULL);

    gtk_object_set_data (GTK_OBJECT (main_wnd), "tooltips", tooltips);

    gtk_window_add_accel_group (GTK_WINDOW (main_wnd), accel_group);

    return main_wnd;
}

GtkWidget *
create_event_enter (main_wnd *wnd)
{
    GtkWidget *event_enter;
    GtkWidget *vbox5;
    GtkWidget *frame4;
    GtkWidget *hbox4;
    GtkWidget *enter_script_entry;
    GtkWidget *enter_browse;
    GtkWidget *frame3;
    GtkWidget *table1;
    GtkWidget *label18;
    GtkWidget *label19;
    GtkWidget *label20;
    GtkWidget *label21;
    GtkWidget *enter_character;
    GtkWidget *enter_character_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *enter_direction;
    GtkWidget *enter_direction_menu;
    GtkWidget *hbox5;
    GtkWidget *enter_x_entry;
    GtkWidget *enter_y_entry;
    GtkWidget *label22;
    GtkWidget *hbuttonbox3;
    GtkWidget *enter_ok;
    GtkWidget *enter_cancel;

    event_enter = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (event_enter), "event_enter", event_enter);
    gtk_window_set_title (GTK_WINDOW (event_enter), "Enter Event");
    gtk_window_set_position (GTK_WINDOW (event_enter), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (event_enter), TRUE);
    gtk_window_set_policy (GTK_WINDOW (event_enter), FALSE, FALSE, FALSE);

    vbox5 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox5);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "vbox5", vbox5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox5);
    gtk_container_add (GTK_CONTAINER (event_enter), vbox5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox5), 4);

    frame4 = gtk_frame_new ("Script");
    gtk_widget_ref (frame4);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "frame4", frame4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame4);
    gtk_box_pack_start (GTK_BOX (vbox5), frame4, TRUE, TRUE, 0);

    hbox4 = gtk_hbox_new (FALSE, 4);
    gtk_widget_ref (hbox4);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "hbox4", hbox4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox4);
    gtk_container_add (GTK_CONTAINER (frame4), hbox4);
    gtk_container_set_border_width (GTK_CONTAINER (hbox4), 4);

    enter_script_entry = gtk_entry_new ();
    gtk_widget_ref (enter_script_entry);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_script_entry", enter_script_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_script_entry);
    gtk_box_pack_start (GTK_BOX (hbox4), enter_script_entry, TRUE, TRUE, 0);

    enter_browse = gtk_button_new_with_label ("Browse ...");
    gtk_widget_ref (enter_browse);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_browse", enter_browse, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_browse);
    gtk_box_pack_start (GTK_BOX (hbox4), enter_browse, FALSE, FALSE, 0);

    frame3 = gtk_frame_new ("Conditions");
    gtk_widget_ref (frame3);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "frame3", frame3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame3);
    gtk_box_pack_start (GTK_BOX (vbox5), frame3, TRUE, TRUE, 0);

    table1 = gtk_table_new (4, 2, TRUE);
    gtk_widget_ref (table1);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "table1", table1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (table1);
    gtk_container_add (GTK_CONTAINER (frame3), table1);
    gtk_container_set_border_width (GTK_CONTAINER (table1), 4);
    gtk_table_set_row_spacings (GTK_TABLE (table1), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table1), 4);

    label18 = gtk_label_new ("Character");
    gtk_widget_ref (label18);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "label18", label18, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label18);
    gtk_table_attach (GTK_TABLE (table1), label18, 0, 1, 0, 1, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);

    label19 = gtk_label_new ("Coordinates");
    gtk_widget_ref (label19);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "label19", label19, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label19);
    gtk_table_attach (GTK_TABLE (table1), label19, 0, 1, 1, 2, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);

    label20 = gtk_label_new ("Direction");
    gtk_widget_ref (label20);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "label20", label20, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label20);
    gtk_table_attach (GTK_TABLE (table1), label20, 0, 1, 2, 3, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);

    label21 = gtk_label_new ("Map (???)");
    gtk_widget_ref (label21);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "label21", label21, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label21);
    gtk_table_attach (GTK_TABLE (table1), label21, 0, 1, 3, 4, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);

    enter_character = gtk_option_menu_new ();
    gtk_widget_ref (enter_character);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_character", enter_character, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_character);
    gtk_table_attach (GTK_TABLE (table1), enter_character, 1, 2, 0, 1, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    enter_character_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("None");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_character_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (enter_character), enter_character_menu);

    enter_direction = gtk_option_menu_new ();
    gtk_widget_ref (enter_direction);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_direction", enter_direction, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_direction);
    gtk_table_attach (GTK_TABLE (table1), enter_direction, 1, 2, 2, 3, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);
    enter_direction_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("None");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_direction_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label ("North");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_direction_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label ("East");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_direction_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label ("South");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_direction_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label ("West");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (enter_direction_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (enter_direction), enter_direction_menu);

    hbox5 = gtk_hbox_new (FALSE, 4);
    gtk_widget_ref (hbox5);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "hbox5", hbox5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox5);
    gtk_table_attach (GTK_TABLE (table1), hbox5, 1, 2, 1, 2, (GtkAttachOptions) (GTK_FILL), (GtkAttachOptions) (GTK_FILL), 0, 0);

    enter_x_entry = gtk_entry_new ();
    gtk_widget_ref (enter_x_entry);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_x_entry", enter_x_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_x_entry);
    gtk_box_pack_start (GTK_BOX (hbox5), enter_x_entry, FALSE, TRUE, 0);
    gtk_widget_set_usize (enter_x_entry, 50, -2);

    enter_y_entry = gtk_entry_new ();
    gtk_widget_ref (enter_y_entry);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_y_entry", enter_y_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_y_entry);
    gtk_box_pack_start (GTK_BOX (hbox5), enter_y_entry, FALSE, TRUE, 0);
    gtk_widget_set_usize (enter_y_entry, 50, -2);

    label22 = gtk_label_new ("???");
    gtk_widget_ref (label22);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "label22", label22, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label22);
    gtk_table_attach (GTK_TABLE (table1), label22, 1, 2, 3, 4, (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0, 0);

    hbuttonbox3 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox3);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "hbuttonbox3", hbuttonbox3, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox3);
    gtk_box_pack_start (GTK_BOX (vbox5), hbuttonbox3, TRUE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox3), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox3), 0);

    enter_ok = gtk_button_new_with_label ("OK");
    gtk_widget_ref (enter_ok);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_ok", enter_ok, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_ok);
    gtk_container_add (GTK_CONTAINER (hbuttonbox3), enter_ok);
    GTK_WIDGET_SET_FLAGS (enter_ok, GTK_CAN_DEFAULT);

    enter_cancel = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (enter_cancel);
    gtk_object_set_data_full (GTK_OBJECT (event_enter), "enter_cancel", enter_cancel, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (enter_cancel);
    gtk_container_add (GTK_CONTAINER (hbuttonbox3), enter_cancel);
    GTK_WIDGET_SET_FLAGS (enter_cancel, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (event_enter), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (enter_script_entry), "changed", GTK_SIGNAL_FUNC (on_enter_script_entry_changed), NULL);
    gtk_signal_connect (GTK_OBJECT (enter_browse), "clicked", GTK_SIGNAL_FUNC (on_enter_browse_clicked), NULL);
    gtk_signal_connect (GTK_OBJECT (enter_ok), "clicked", GTK_SIGNAL_FUNC (on_enter_ok_clicked), wnd);
    gtk_signal_connect (GTK_OBJECT (enter_cancel), "clicked", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    return event_enter;
}
