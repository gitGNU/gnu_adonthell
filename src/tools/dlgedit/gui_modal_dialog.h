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
 * @file gui_modal_dialog.h
 *
 * @author Kai Sterker
 * @brief Base class for modal dialog windows
 */

#ifndef GUI_MODAL_DIALOG_H
#define GUI_MODAL_DIALOG_H

#include <gtk/gtkstyle.h>

/**
 * Encapsulates the most basic functionality a modal dialog window needs.
 */
class GuiModalDialog
{
public:
    /**
     * Initialize the dialog window.
     */
    GuiModalDialog ();
    /*
     * Destroy the dialog window.
     */
    virtual ~GuiModalDialog ();

    /**
     * Displays the dialog window and sets it transient for the dlgedit main window.
     * @return the state of pressedOK. This should be set to <b>true</b> if the dialog
     * was closed via the OK button. By default it is <b>false</b>.
     */
    bool run ();
    /**
     * Indicate whether the OK button of the dialog has been pressed.
     * @param button Set this to <b>true</b> if the OK button has been pressed,
     * otherwise to <b>false</b>
     */
    void okButtonPressed (bool button)  { pressedOK = button; }
    /**
     * Get the dialog window.
     * @return the dialog window.
     */
    GtkWidget *getWindow ()             { return window; }

protected:
    bool pressedOK;                 // whether the Cancel or OK button has been pushed
    GtkWidget *window;              // the dialog window
};

#endif // GUI_MODAL_DIALOG_H
