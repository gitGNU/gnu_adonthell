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

#include <gtk/gtk.h>
#include <stdio.h>

#include "../../types.h"
#include "dlgnode.h"
#include "main.h"
#include "graph.h"
#include "dlgcompile.h"
#include "geometrie.h"
#include "run.h"
#include "preset.h"
#include "function.h"
#include "ps_dlg.h"
#include "debug.h"
#include "interface.h"

/* Main Window: on_widget_destroy App */
void 
on_widget_destroy (GtkWidget * widget, gpointer data)
{
    gtk_main_quit ();
    gtk_widget_destroy (widget);
}


/* File-Selection: ok -> get file name */
void 
on_fs_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkFileSelection *fs = (GtkFileSelection *) gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_FILE_SELECTION);
    GString *file = (GString *) user_data;

    file = g_string_assign (file, gtk_file_selection_get_filename (fs));
    gtk_widget_destroy (GTK_WIDGET (fs));
    gtk_main_quit ();
}

/* File-Selection: cancel -> no file name */
void 
on_fs_cancel_button_pressed (GtkButton * button, gpointer user_data)
{
    GtkFileSelection *fs = (GtkFileSelection *) gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_FILE_SELECTION);

    gtk_widget_destroy (GTK_WIDGET (fs));
    gtk_main_quit ();
}

/* File Menu: New */
void 
on_file_new_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    new_dialogue ((MainFrame *) user_data);
}

/* File Menu: Load */
void 
on_file_load_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;

    GString *file = g_string_new (NULL);
    GtkWidget *fs = create_fileselection (file, 1);
    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->file_name);

    // chose file
    gtk_widget_show (fs);
    gtk_main ();

    // Try to load file
    load_dialogue (wnd, file->str);

    // Clean up
    g_string_free (file, TRUE);
}

/* File Menu: Save */
void 
on_file_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    save_dialogue ((MainFrame *) user_data);
}

/* Dialogue Menu: Compile */
void 
on_dialogue_compile_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;
    dlg_compiler compiler (wnd->nodes, wnd->file_name, wnd->cust_func);

    compiler.run ();

    gtk_widget_set_sensitive (wnd->dialogue_run, TRUE);
}

/* Dialogue Menu: Run */
void 
on_dialogue_run_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;
    run_dlg dlg (wnd);

    dlg.run ();

    gtk_main ();
}

/* Dialogue Menu: Variables */
void 
on_dialogue_variables_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;
    
    preset_dlg dlg (wnd);
    dlg.run  ();
    
    gtk_main ();
}

/* Dialogue Menu: Functions */
void 
on_dialogue_functions_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;
    
    function_dlg dlg (wnd);
    dlg.run  ();
    
    gtk_main ();
}

/* Dialogue Menu: Variables */
void 
on_dialogue_player_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    ps_dlg dlg (((MainFrame *) user_data)->myplayer);
    dlg.run ();
    
    gtk_main ();
}

/* Extras Menu: Debug */
void 
on_extras_debug_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;

    if (wnd->dbg_dlg == NULL)
    {
        wnd->dbg_dlg = new debug_dlg;
        wnd->dbg_dlg->wnd = wnd;
    }

    wnd->dbg_dlg->update ();
}

/* Node selected in preview */
void
on_list_select (GtkList *list, GtkWidget *widget, gpointer user_data)
{
    DlgNode* node = (DlgNode *) gtk_object_get_user_data (GTK_OBJECT (widget));
    GdkPoint point = center_point (node->position);
    MainFrame *wnd = (MainFrame *) user_data;
    
    deselect_object (wnd);
    center_object (wnd, node);
    select_object (wnd, point);
}
