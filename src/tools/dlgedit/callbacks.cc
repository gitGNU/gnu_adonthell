/*
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
#include "dlgrun.h"
#include "geometrie.h"
#include "interface.h"

/* Main Window: Destroy App */
void 
destroy (GtkWidget * widget, gpointer data)
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

/* File Menu: Load */
void 
on_file_new_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    new_dialogue ((MainFrame *) user_data);
}

/* File Menu: Load */
void 
on_file_load_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    load_dialogue ((MainFrame *) user_data);
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
    ShowDialogue ((RunData *) user_data, row);
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
