/*
   Copyright (C) 2002/2007 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file gui_modal_dialog.cc
 *
 * @author Kai Sterker
 * @brief Base class for modal dialog windows
 */

#include <gtk/gtk.h>
#include "gui_modal_dialog.h"

// ctor
GuiModalDialog::GuiModalDialog (GtkWindow *p, const bool & m)
{
    parent = p;
    pressedOK = false;
    modal = m;
    window = NULL;
}

// clean up
GuiModalDialog::~GuiModalDialog ()
{
    // destroy the window
	if (window != NULL)
	{
		gtk_widget_destroy (GTK_WIDGET (window));
	}
}

// displays the dialog window
bool GuiModalDialog::run ()
{
    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (parent));
    gtk_window_set_modal (GTK_WINDOW (window), modal);
    gtk_widget_show (GTK_WIDGET (window));
    if (modal) gtk_main ();

    // return whether the OK button has been pressed
    return pressedOK;
}
