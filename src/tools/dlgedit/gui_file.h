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
#include "gui_modal_dialog.h"

/**
 * A C++ interfact to GTK's file selection dialog.
 */
class GuiFile : public GuiModalDialog
{
public:
    GuiFile (int type, const std::string &file, const std::string &title);
    ~GuiFile ();

    std::string getSelection ()           { return file_; }
    void setSelection (std::string file)  { file_ = file; }

private:
    std::string file_;              // the file the user has selected
};


/**
 * Callback to intercept pressing of fileselection's okay button
 */
void on_ok_button_pressed (GtkButton *button, gpointer user_data);
