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
 * @file gui_modal_dialog.cc
 *
 * @author Kai Sterker
 * @brief Base class for modal dialog windows
 */

#include <gtk/gtk.h>
#include "gui_dlgedit.h"
#include "gui_modal_dialog.h"

// ctor
GuiModalDialog::GuiModalDialog ()
{
    pressedOK = false;
}

// clean up
GuiModalDialog::~GuiModalDialog ()
{
    // destroy the window
    gtk_widget_destroy (GTK_WIDGET (window));
}

// displays the dialog window
bool GuiModalDialog::run ()
{
    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (GuiDlgedit::window->getWindow ()));
    gtk_widget_show (GTK_WIDGET (window));
    gtk_main ();

    // return whether the OK button has been pressed
    return pressedOK;
}
