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
 * @file gui_file.h
 *
 * @author Kai Sterker
 * @brief Wrapper for the GtkFileSelection widget
 */

#include <string>
#include <gtk/gtkstyle.h>

/**
 * A C++ interfact to GTK's file selection dialog.
 */
class GuiFile
{
public:
    GuiFile (int type, const string &file, const string &title);
    ~GuiFile ();

    bool run ();
    void okButtonPressed (bool button)  { pressedOK = button; }

    string getSelection ()              { return file_; }
    void setSelection (string file)     { file_ = file; }

    GtkFileSelection *getWindow ()      { return window; }
private:
    GtkFileSelection *window;       // The actual file selection widget
    string file_;                   // the file the user has selected
    bool pressedOK;                 // whether the Cancel or OK button has been pushed
};


/**
 * Callback to intercept pressing of fileselection's okay button
 */
void on_ok_button_pressed (GtkButton *button, gpointer user_data);