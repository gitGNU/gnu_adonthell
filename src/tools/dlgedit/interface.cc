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

class dialog;

#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "../../types.h"
#include "../../interpreter.h"
#include "linked_list.h"
#include "dlgnode.h"
#include "main.h"
#include "callbacks.h"
#include "events.h"
#include "dlgrun.h"
#include "interface.h"

/* Create Top Level Window and Controls */
void 
create_mainframe (MainFrame * MainWnd)
{
    GtkWidget *vbox;
    GtkWidget *menu;
    GtkWidget *paned;
    GtkWidget *menuitem;
    GtkWidget *scrolled;
    GdkColor color;
    GtkAccelGroup *accel_group;

    /* Main Window */
    MainWnd->wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize (GTK_WIDGET (MainWnd->wnd), 640, 480);
    gtk_window_set_title (GTK_WINDOW (MainWnd->wnd), g_strjoin (NULL, "Adonthell Dialogue Editor v0.2 - [", strrchr (MainWnd->file_name, '/') + 1, "]", NULL));
    gtk_signal_connect (GTK_OBJECT (MainWnd->wnd), "delete_event", GTK_SIGNAL_FUNC (destroy), NULL);

    /* Menu Accelerators */
    accel_group = gtk_accel_group_new ();

    /* Main Windows Menu */
    MainWnd->menu = gtk_menu_bar_new ();

    /* Attach Menubar */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (MainWnd->wnd), vbox);
    gtk_widget_show (vbox);

    gtk_box_pack_start (GTK_BOX (vbox), MainWnd->menu, FALSE, FALSE, 2);

    /* File Menu */
    menu = gtk_menu_new ();

    /* Project */
    menuitem = gtk_menu_item_new_with_label ("Project");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_p, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_project_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

    /* Seperator */
    menuitem = gtk_menu_item_new ();
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);

    /* New */
    menuitem = gtk_menu_item_new_with_label ("New");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_n, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_new_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

    /* Load */
    menuitem = gtk_menu_item_new_with_label ("Load");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_l, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_load_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

    /* Save */
    menuitem = gtk_menu_item_new_with_label ("Save");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_s, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_save_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

    /* Seperator */
    menuitem = gtk_menu_item_new ();
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);

    /* Quit */
    menuitem = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_F4, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (destroy), (gpointer) NULL);
    gtk_widget_show (menuitem);

    /* Attach File Menu */
    menuitem = gtk_menu_item_new_with_label ("File");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
    gtk_menu_bar_append (GTK_MENU_BAR (MainWnd->menu), menuitem);

    /* Dialogue Menu */
    menu = gtk_menu_new ();

    /* Compile */
    menuitem = gtk_menu_item_new_with_label ("Compile");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_c, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_compile_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

    /* Run */
    menuitem = gtk_menu_item_new_with_label ("Run");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_r, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_run_activate), (gpointer) MainWnd);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);
    MainWnd->dialogue_run = menuitem;

    /* Attach Dialogue Menu */
    menuitem = gtk_menu_item_new_with_label ("Dialogue");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
    gtk_menu_bar_append (GTK_MENU_BAR (MainWnd->menu), menuitem);

    gtk_widget_show (MainWnd->menu);

    paned = gtk_vpaned_new ();
    gtk_box_pack_end (GTK_BOX (vbox), paned, TRUE, TRUE, 2);
    gtk_paned_set_handle_size (GTK_PANED (paned), 5);
    gtk_paned_set_gutter_size (GTK_PANED (paned), 7);
    gtk_widget_show (paned);

    /* Drawing Area */
    MainWnd->graph = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (MainWnd->graph), 200, 350);
    gtk_paned_add1 (GTK_PANED (paned), MainWnd->graph);
    gtk_widget_show (MainWnd->graph);

    /* Signals used to handle backing pixmap */
    gtk_signal_connect (GTK_OBJECT (MainWnd->graph), "expose_event", (GtkSignalFunc) expose_event, MainWnd);
    gtk_signal_connect (GTK_OBJECT (MainWnd->graph), "configure_event", (GtkSignalFunc) configure_event, MainWnd);
    gtk_signal_connect (GTK_OBJECT (MainWnd->graph), "button_press_event", (GtkSignalFunc) button_press_event, MainWnd);
    gtk_signal_connect (GTK_OBJECT (MainWnd->graph), "button_release_event", (GtkSignalFunc) button_release_event, MainWnd);
    gtk_signal_connect (GTK_OBJECT (MainWnd->graph), "motion_notify_event", (GtkSignalFunc) motion_notify_event, MainWnd);
    gtk_signal_connect (GTK_OBJECT (MainWnd->wnd), "key_press_event", (GtkSignalFunc) key_press_notify_event, MainWnd);

    gtk_widget_set_events (MainWnd->graph, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_KEY_PRESS_MASK);

    gtk_window_add_accel_group (GTK_WINDOW (MainWnd->wnd), accel_group);

    /* List */
    scrolled = gtk_scrolled_window_new (NULL, NULL);
    gtk_paned_add2 (GTK_PANED (paned), scrolled);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_widget_show (scrolled);
    
    MainWnd->list = gtk_list_new ();
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled), MainWnd->list);
    gtk_widget_show (MainWnd->list);
    gtk_signal_connect (GTK_OBJECT (MainWnd->list), "select_child", (GtkSignalFunc) on_list_select, MainWnd);
    GTK_WIDGET_UNSET_FLAGS (MainWnd->list, GTK_CAN_FOCUS);

    /* Font */
    MainWnd->font = gdk_font_load ("-*-*-medium-r-normal-sans-12-*-*-*-*-*-iso8859-1");

    /* Display MainWindow */
    gtk_widget_show (MainWnd->wnd);

    /* Colors */
    /* Red */
    MainWnd->color[GC_RED] = gdk_gc_new (gtk_widget_get_parent_window (MainWnd->graph));
    color.red = 65535;
    color.green = 32700;
    color.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (MainWnd->graph), &color, TRUE, TRUE);
    gdk_gc_set_foreground (MainWnd->color[GC_RED], &color);

    /* Dark Red */
    MainWnd->color[GC_DARK_RED] = gdk_gc_new (gtk_widget_get_parent_window (MainWnd->graph));
    color.red = 65535;
    color.green = 0;
    color.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (MainWnd->wnd), &color, TRUE, TRUE);
    gdk_gc_set_foreground (MainWnd->color[GC_DARK_RED], &color);

    /* Dark Blue */
    MainWnd->color[GC_DARK_BLUE] = gdk_gc_new (gtk_widget_get_parent_window (MainWnd->graph));
    color.red = 0;
    color.green = 0;
    color.blue = 35000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (MainWnd->wnd), &color, TRUE, TRUE);
    gdk_gc_set_foreground (MainWnd->color[GC_DARK_BLUE], &color);

    /* Green */
    MainWnd->color[GC_GREEN] = gdk_gc_new (gtk_widget_get_parent_window (MainWnd->graph));
    color.red = 0;
    color.green = 35000;
    color.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (MainWnd->wnd), &color, TRUE, TRUE);
    gdk_gc_set_foreground (MainWnd->color[GC_GREEN], &color);

    /* Green */
    MainWnd->color[GC_PURPLE] = gdk_gc_new (gtk_widget_get_parent_window (MainWnd->graph));
    color.red = 35000;
    color.green = 0;
    color.blue = 35000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (MainWnd->wnd), &color, TRUE, TRUE);
    gdk_gc_set_foreground (MainWnd->color[GC_PURPLE], &color);

    return;
}

/* "Add text to node" - dialog */
GtkWidget *
create_text_dialog (NodeData * cbd)
{
    GtkWidget *text_dialog;

    GtkWidget *vbox1;
    GtkWidget *scrolled_text_box;
    GtkWidget *text_box;
    GtkWidget *button_player;
    GtkWidget *button_npc;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *cancel_button;
    GtkWidget *ok_button;

    GSList *vbox1_group = NULL;

    /* The Dialog - Window */
    text_dialog = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_widget_set_name (text_dialog, "text_dialog");
    gtk_object_set_data (GTK_OBJECT (text_dialog), "text_dialog", text_dialog);
    gtk_widget_set_usize (text_dialog, 450, 200);
    gtk_window_set_title (GTK_WINDOW (text_dialog), "Text");
    gtk_window_set_position (GTK_WINDOW (text_dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (text_dialog), 450, 200);
    gtk_window_set_policy (GTK_WINDOW (text_dialog), FALSE, FALSE, FALSE);

    /* The container for all following widgets */
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_name (vbox1, "vbox1");
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (text_dialog), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 5);

    /* A Multi-line edit-Box */
    scrolled_text_box = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_name (scrolled_text_box, "scrolled_text_box");
    gtk_widget_ref (scrolled_text_box);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "scrolled_text_box", scrolled_text_box, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolled_text_box);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolled_text_box, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_text_box), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    /* The actual text-box */
    text_box = gtk_text_new (NULL, NULL);
    gtk_widget_set_name (text_box, "text_box");
    gtk_widget_ref (text_box);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "text_box", text_box, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (text_box);
    gtk_container_add (GTK_CONTAINER (scrolled_text_box), text_box);
    gtk_text_set_editable (GTK_TEXT (text_box), TRUE);
    gtk_text_set_word_wrap (GTK_TEXT (text_box), TRUE);
    if (cbd->node->text != NULL)
        gtk_text_insert (GTK_TEXT (text_box), text_box->style->font, &text_box->style->black, &text_box->style->white, cbd->node->text, -1);
    cbd->edit_box = GTK_EDITABLE (text_box);

    /* A Toggle - Button */
    button_player = gtk_radio_button_new_with_label (vbox1_group, "Player");
    vbox1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button_player));
    gtk_widget_set_name (button_player, "button_player");
    gtk_widget_ref (button_player);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "button_player", button_player, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button_player);
    gtk_box_pack_start (GTK_BOX (vbox1), button_player, FALSE, FALSE, 0);
    GTK_WIDGET_UNSET_FLAGS (button_player, GTK_CAN_FOCUS);

    /* Another Toggle - Button */
    button_npc = gtk_radio_button_new_with_label (vbox1_group, "NPC");
    vbox1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button_npc));
    gtk_widget_set_name (button_npc, "button_npc");
    gtk_widget_ref (button_npc);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "button_npc", button_npc, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button_npc);
    gtk_box_pack_start (GTK_BOX (vbox1), button_npc, FALSE, FALSE, 0);
    GTK_WIDGET_UNSET_FLAGS (button_npc, GTK_CAN_FOCUS);

    /* And a seperator */
    hseparator1 = gtk_hseparator_new ();
    gtk_widget_set_name (hseparator1, "hseparator1");
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 0);

    /* Just a Button -Box */
    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_set_name (hbuttonbox1, "hbuttonbox1");
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 5);
    gtk_button_box_set_child_size (GTK_BUTTON_BOX (hbuttonbox1), 80, 10);
    gtk_button_box_set_child_ipadding (GTK_BUTTON_BOX (hbuttonbox1), 0, 0);

    /* OK - Button */
    ok_button = gtk_button_new_with_label ("OK");
    gtk_widget_set_name (ok_button, "ok_button");
    gtk_widget_ref (ok_button);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "ok_button", ok_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    /* Cancel - Button */
    cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_widget_set_name (cancel_button, "cancel_button");
    gtk_widget_ref (cancel_button);
    gtk_object_set_data_full (GTK_OBJECT (text_dialog), "cancel_button", cancel_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    /* Callback handlers (or whatever you call them in gtk) */
    gtk_signal_connect (GTK_OBJECT (text_dialog), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (button_player), "toggled", GTK_SIGNAL_FUNC (on_button_player_toggled), cbd);
    gtk_signal_connect (GTK_OBJECT (button_npc), "toggled", GTK_SIGNAL_FUNC (on_button_npc_toggled), cbd);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "pressed", GTK_SIGNAL_FUNC (on_cancel_button_pressed), cbd);
    gtk_signal_connect (GTK_OBJECT (ok_button), "pressed", GTK_SIGNAL_FUNC (on_ok_button_pressed), cbd);

    /* various inits */
    if (cbd->node->type == PLAYER)
        on_button_player_toggled (GTK_TOGGLE_BUTTON (button_player), cbd);
    else
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button_npc), TRUE);

    gtk_widget_grab_focus (text_box);
    gtk_widget_grab_default (ok_button);

    return text_dialog;
}

/* Tooltip - like widget */
GtkWidget *
create_tooltip (gchar * text, s_int32 x, s_int32 y)
{
    GtkWidget *tip_window;
    GtkWidget *frame;
    GtkWidget *tip;

    tip_window = gtk_window_new (GTK_WINDOW_POPUP);
    gtk_widget_set_uposition (tip_window, x, y);
    gtk_object_set_data (GTK_OBJECT (tip_window), "tip_window", tip_window);
    gtk_window_set_policy (GTK_WINDOW (tip_window), FALSE, FALSE, FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_ref (frame);
    gtk_object_set_data_full (GTK_OBJECT (tip_window), "frame", frame, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame);
    gtk_container_add (GTK_CONTAINER (tip_window), frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);

    tip = gtk_label_new (g_strdup (text));
    gtk_widget_ref (tip);
    gtk_object_set_data_full (GTK_OBJECT (tip_window), "tip", tip, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (tip);
    gtk_container_add (GTK_CONTAINER (frame), tip);
    gtk_label_set_justify (GTK_LABEL (tip), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap (GTK_LABEL (tip), TRUE);

    return tip_window;
}

/* File - Select -Box */
GtkWidget *
create_fileselection (GString * file, u_int8 type)
{
    GtkWidget *fileselection1;
    GtkWidget *fs_cancel_button;
    GtkWidget *fs_ok_button;

    fileselection1 = gtk_file_selection_new ("Select File");
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fileselection1", fileselection1);
    gtk_container_set_border_width (GTK_CONTAINER (fileselection1), 10);
    GTK_WINDOW (fileselection1)->type = GTK_WINDOW_DIALOG;
    gtk_window_set_modal (GTK_WINDOW (fileselection1), TRUE);
    gtk_window_set_policy (GTK_WINDOW (fileselection1), FALSE, FALSE, FALSE);

    if (type == 1)
        gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (fileselection1));

    fs_cancel_button = GTK_FILE_SELECTION (fileselection1)->cancel_button;
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fs_cancel_button", fs_cancel_button);
    gtk_widget_show (fs_cancel_button);

    fs_ok_button = GTK_FILE_SELECTION (fileselection1)->ok_button;
    gtk_object_set_data (GTK_OBJECT (fileselection1), "fs_ok_button", fs_ok_button);
    gtk_widget_show (fs_ok_button);
    GTK_WIDGET_SET_FLAGS (fs_ok_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (fs_cancel_button), "pressed", GTK_SIGNAL_FUNC (on_fs_cancel_button_pressed), file);
    gtk_signal_connect (GTK_OBJECT (fs_ok_button), "pressed", GTK_SIGNAL_FUNC (on_fs_ok_button_pressed), file);

    gtk_widget_grab_focus (fs_ok_button);
    gtk_widget_grab_default (fs_ok_button);

    return fileselection1;
}

GtkWidget *
create_run_dialogue (RunData * rd)
{
    GtkWidget *run_dialogue;
    GtkWidget *vbox1;
    GtkWidget *scrolledwindow1;
    GtkWidget *npc_text;
    GtkWidget *hseparator1;
    GtkWidget *scrolledwindow2;
    GtkWidget *player_txt;

    run_dialogue = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_widget_set_name (run_dialogue, "run_dialogue");
    gtk_object_set_data (GTK_OBJECT (run_dialogue), "run_dialogue", run_dialogue);
    gtk_widget_set_usize (run_dialogue, 400, 300);
    gtk_window_set_title (GTK_WINDOW (run_dialogue), "Run Dialogue");
    gtk_window_set_modal (GTK_WINDOW (run_dialogue), TRUE);
    gtk_window_set_policy (GTK_WINDOW (run_dialogue), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_name (vbox1, "vbox1");
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (run_dialogue), vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 5);

    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_name (scrolledwindow1, "scrolledwindow1");
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    npc_text = gtk_text_new (NULL, NULL);
    gtk_widget_set_name (npc_text, "npc_text");
    gtk_widget_ref (npc_text);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "npc_text", npc_text, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_text);
    gtk_text_set_word_wrap (GTK_TEXT (npc_text), TRUE);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), npc_text);
    GTK_WIDGET_UNSET_FLAGS (npc_text, GTK_CAN_FOCUS);
    rd->npc = npc_text;

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_set_name (hseparator1, "hseparator1");
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "hseparator1", hseparator1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, FALSE, 5);

    scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_name (scrolledwindow2, "scrolledwindow2");
    gtk_widget_ref (scrolledwindow2);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "scrolledwindow2", scrolledwindow2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow2);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow2, TRUE, TRUE, 0);

    player_txt = gtk_clist_new (1);
    gtk_widget_set_name (player_txt, "player_txt");
    gtk_widget_ref (player_txt);
    gtk_object_set_data_full (GTK_OBJECT (run_dialogue), "player_txt", player_txt, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (player_txt);
    gtk_container_add (GTK_CONTAINER (scrolledwindow2), player_txt);
    gtk_clist_set_column_width (GTK_CLIST (player_txt), 0, 80);
    gtk_clist_column_titles_hide (GTK_CLIST (player_txt));
    rd->player = player_txt;

    gtk_signal_connect (GTK_OBJECT (run_dialogue), "delete_event", GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (player_txt), "select_row", GTK_SIGNAL_FUNC (on_player_txt_select_row), rd);

    return run_dialogue;
}

GtkWidget* 
create_list_item (MainFrame *wnd, DlgNode *node, int mode)
{
    GtkWidget *label;
    GtkWidget *list_item;
    GdkWindow *list;
    GdkColor color;
    GtkStyle *style = gtk_style_copy (gtk_widget_get_default_style ());
    int w, h;

    switch (mode)
    {
        // Selcted node's text
        case 1:
        {
            color.red = 65535;
            if (node->type == NPC) color.green = 0;
            else color.green = 32700;
            color.blue = 0;
            break;
        }
        // Linked node's text
        case 2:
        {
            color.red = 30000;
            color.green = 30000;
            if (node->type == NPC) color.blue = 30000;
            else color.blue = 45000;
            break;
        }
        // Directly attached node's text
        default:
        {
            color.red = 0;
            color.green = 0;
            if (node->type == NPC) color.blue = 0;
            else color.blue = 35000;
            break;
        }
    }

    /* Set List_item colors */
    style->fg[0] = color;
    style->bg[2] = color;
    
    /* get width to use for label */
    list = gtk_widget_get_parent_window (wnd->list);
    gdk_window_get_size (list, &w, &h);

    /* create label */    
    label = gtk_label_new (node->text);
    gtk_widget_set_style (label, style);
    gtk_widget_set_usize (label, w - 10, 0);
    gtk_label_set_justify ((GtkLabel *) label, GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap ((GtkLabel *) label, TRUE);
    gtk_widget_show (label);

    /* add label to list_item */
    list_item = gtk_list_item_new ();
    gtk_container_add (GTK_CONTAINER(list_item), label);
    gtk_object_set_user_data (GTK_OBJECT (list_item), (gpointer) node);
    GTK_WIDGET_UNSET_FLAGS (list_item, GTK_CAN_FOCUS);
    gtk_widget_show (list_item);
    
    return list_item;
}
