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
#include "dlgnode.h"
#include "main.h"
#include "callbacks.h"
#include "events.h"
#include "interface.h"

/* Create Top Level Window and Controls */
void create_mainframe (MainFrame * MainWnd)
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
    gtk_window_set_title (GTK_WINDOW (MainWnd->wnd), g_strjoin (NULL, "Adonthell Dialogue Editor v0.4 - [", strrchr (MainWnd->file_name, '/') + 1, "]", NULL));
    gtk_signal_connect (GTK_OBJECT (MainWnd->wnd), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

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
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_widget_destroy), (gpointer) NULL);
    gtk_widget_show (menuitem);

    /* Attach File Menu */
    menuitem = gtk_menu_item_new_with_label ("File");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
    gtk_menu_bar_append (GTK_MENU_BAR (MainWnd->menu), menuitem);

    /* Dialogue Menu */
    menu = gtk_menu_new ();

    /* Variables */
    menuitem = gtk_menu_item_new_with_label ("Variables");
    gtk_container_add (GTK_CONTAINER (menu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_v, GDK_MOD1_MASK, GTK_ACCEL_VISIBLE);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_variables_activate), (gpointer) MainWnd);
    gtk_widget_show (menuitem);

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


/* Tooltip - like widget */
GtkWidget *
create_tooltip (const gchar * text, s_int32 x, s_int32 y)
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
    gtk_misc_set_padding (GTK_MISC (tip), 4, 1);

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

// List entry for the instant preview
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
    label = gtk_label_new (((Circle *) node)->text.c_str ());
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
