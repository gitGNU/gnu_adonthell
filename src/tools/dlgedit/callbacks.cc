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
#include "linked_list.h"
#include "dlgnode.h"
#include "main.h"
#include "graph.h"
#include "compile.h"
#include "../../interpreter.h"
#include "../../array_tmpl.h"
#include "../../dialog.h"
#include "dlgrun.h"
#include "geometrie.h"
#include "pjt_interface.h"
#include "function.h"
#include "interface.h"

/* Main Window: on_widget_destroy App */
void 
on_widget_destroy (GtkWidget * widget, gpointer data)
{
    gtk_main_quit ();
}

/* Text Dialog: Player - Part */
void 
on_button_player_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
    NodeData *cbd = (NodeData *) user_data;
    GtkText *text_box = (GtkText *) cbd->edit_box;
    GdkColor white, dark_blue;
    gchar *text;

    /* Text-Box contains text of player */
    if (gtk_toggle_button_get_active (togglebutton))
    {
        cbd->node->type = PLAYER;

        dark_blue.red = 0;
        dark_blue.green = 0;
        dark_blue.blue = 35000;

        white.red = 65535;
        white.green = 65535;
        white.blue = 65535;

        /* Set Text-color to blue (Have to assure that text is inserted) */
        gtk_text_freeze (text_box);
        text = gtk_editable_get_chars (cbd->edit_box, 0, -1);
        gtk_editable_delete_text (cbd->edit_box, 0, -1);
        gtk_text_insert (text_box, (GTK_WIDGET (cbd->edit_box))->style->font, &dark_blue, &white, "a", -1);
        gtk_text_insert (text_box, (GTK_WIDGET (cbd->edit_box))->style->font, &dark_blue, &white, text, -1);
        gtk_editable_delete_text (cbd->edit_box, 0, 1);
        g_free (text);
        gtk_text_thaw (text_box);
    }
}

/* Text Dialog: NPC - Part */
void 
on_button_npc_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
    NodeData *cbd = (NodeData *) user_data;
    GtkText *text_box = (GtkText *) cbd->edit_box;
    GdkColor white, black;
    gchar *text;

    /* Text-Box contains text of player */
    if (gtk_toggle_button_get_active (togglebutton))
    {
        cbd->node->type = NPC;

        black.red = 0;
        black.green = 0;
        black.blue = 0;

        white.red = 65535;
        white.green = 65535;
        white.blue = 65535;

        /* Set Text-color to black (Have to assure that text != NULL) */
        gtk_text_freeze (text_box);
        text = gtk_editable_get_chars (cbd->edit_box, 0, -1);
        gtk_editable_delete_text (cbd->edit_box, 0, -1);
        gtk_text_insert (text_box, (GTK_WIDGET (cbd->edit_box))->style->font, &black, &white, "a", -1);
        gtk_text_insert (text_box, (GTK_WIDGET (cbd->edit_box))->style->font, &black, &white, text, -1);
        gtk_editable_delete_text (cbd->edit_box, 0, 1);
        g_free (text);
        gtk_text_thaw (text_box);
    }
}

/* Text Dialog: Abort -> throw everything away */
void 
on_cancel_button_pressed (GtkButton * button, gpointer user_data)
{
    NodeData *cbd = (NodeData *) user_data;

    /* close dialog */
    gtk_widget_destroy (cbd->wnd->text_dlg);
    gtk_main_quit ();
}

/* Text Dialog: Accept -> preserve everything */
void 
on_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    NodeData *cbd = (NodeData *) user_data;

    /* Set Nodes Text */
    if (cbd->node->text)
        g_free (cbd->node->text);
    cbd->node->text = gtk_editable_get_chars (cbd->edit_box, 0, -1);

    /* close dialog */
    gtk_widget_destroy (cbd->wnd->text_dlg);
    gtk_main_quit ();

    cbd->retval = 1;
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

// File menu: project
void
on_file_project_activate (GtkMenuItem *menuitem, gpointer user_data)
{
    MainFrame *wnd = (MainFrame *) user_data;
    
    // Create the project window
    if (wnd->project == NULL)
    {
        wnd->project = create_project_window ();
        gtk_widget_show (wnd->project);
    }

    // Bring the window to the front
    gdk_window_raise (gtk_widget_get_parent_window (wnd->project));
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
    make_dialogue ((MainFrame *) user_data);
}

/* Dialogue Menu: Run */
void 
on_dialogue_run_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    run_dialogue ((MainFrame *) user_data);
}

/* Continue Dialogue */
void 
on_player_txt_select_row (GtkCList * clist, gint row, gint column, GdkEvent * event, gpointer user_data)
{
    RunData *rd = (RunData *) user_data;

    rd->data->answer = row;
    ShowDialogue (rd);
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

void on_function_released (GtkButton *button, gpointer user_data)
{

}

// Add function to the list
void on_add_button_clicked (GtkButton * button, gpointer user_data)
{
    ((function *) user_data)->add ();
}

// Remove funcxtion from list
void on_remove_button_clicked (GtkButton * button, gpointer user_data)
{
    ((function *) user_data)->remove ();
}

// Element of function-list selected
void on_fct_select_row (GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
    ((function *) data)->select (row);
}

// Element of function-list selected
void on_fct_unselect_row (GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
    ((function *) data)->select (-1);
}

// Move function up in list
void on_up_button_clicked (GtkButton * button, gpointer user_data)
{
    ((function *) user_data)->up ();
}

// Move function down in list
void on_down_button_clicked (GtkButton * button, gpointer user_data)
{
    ((function *) user_data)->down ();
}

//
void on_fct_ok_buttpn_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_fct_cancel_button_clicked (GtkButton * button, gpointer user_data)
{

}