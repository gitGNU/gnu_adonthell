/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file gui_file.cc
 *
 * @author Kai Sterker
 * @brief Wrapper for the GtkFileSelection widget
 */

#include <gtk/gtk.h>
#include "gui_file.h"
#include "gui_dlgedit.h"

// create a new file selection window
GuiFile::GuiFile (int type, const string &title, const string &file)
{
    pressedOK = false;

    GtkWidget *fs_cancel_button;
    GtkWidget *fs_ok_button;

    window = (GtkFileSelection *) gtk_file_selection_new (title.c_str ());
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    GTK_WINDOW (window)->type = GTK_WINDOW_DIALOG;
    gtk_window_set_modal (GTK_WINDOW (window), TRUE);
    gtk_window_set_policy (GTK_WINDOW (window), FALSE, FALSE, FALSE);

    // hide new/delete/rename directory buttons if we're loading a file
    if (type == LOAD) gtk_file_selection_hide_fileop_buttons (window);

    // open the directory browsed last 
    gtk_file_selection_set_filename (window, file.c_str ());
    
    // get OK and Cancel buttons
    fs_cancel_button = window->cancel_button;
    fs_ok_button = window->ok_button;

    // attach the callbacks to them
    gtk_signal_connect (GTK_OBJECT (fs_cancel_button), "clicked", GTK_SIGNAL_FUNC (gtk_main_quit), NULL);
    gtk_signal_connect (GTK_OBJECT (fs_ok_button), "clicked", GTK_SIGNAL_FUNC (on_ok_button_pressed), this);

    // give focus to the OK button
    GTK_WIDGET_SET_FLAGS (fs_ok_button, GTK_CAN_DEFAULT);
    gtk_widget_grab_focus (fs_ok_button);
    gtk_widget_grab_default (fs_ok_button);
}

// clean up
GuiFile::~GuiFile ()
{
    gtk_widget_destroy (GTK_WIDGET (window));
}

// allow the user to save or load a file
bool GuiFile::run ()
{
    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (GuiDlgedit::window->getWindow ()));
    gtk_widget_show (GTK_WIDGET (window));
    gtk_main ();

    // return whether the OK button has been pressed
    return pressedOK;
}

// event callback
void on_ok_button_pressed (GtkButton * button, gpointer user_data)
{
    GuiFile *fs = (GuiFile *) user_data;
    fs->okButtonPressed (true);
    fs->setSelection (gtk_file_selection_get_filename (fs->getWindow ()));
    gtk_main_quit ();
}
