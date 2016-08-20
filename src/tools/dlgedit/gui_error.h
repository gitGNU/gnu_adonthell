/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file gui_error.h
 *
 * @author Kai Sterker
 * @brief The error console
 */

#ifndef GUI_ERROR_H
#define GUI_ERROR_H

#include <string>
#include "dlg_node.h"

/**
 * Displays a list of compile errors. Selecting an error message from the
 * list automatically brings up the edit dialog of the faulty node.
 */
class GuiError
{
public:
    GuiError ();
    ~GuiError ();

    /**
     * Global pointer to the error console, to allow easy access from anywhere.
     */
    static GuiError *console;
    
    /**
     * Add an error message to the list of errors.
     * @param error the text to display
     * @param node the node where the error occured
     */
    void add (std::string error, DlgNode *node);
    /**
     * Show the error console and display all errors
     */
    void display ();
    /**
     * Remove all entries from the error console
     */
    void clear ();
    
private:
    GtkWidget *window;      // The dialog window
    GtkWidget *list;        // The list widget
};

#endif // GUI_ERROR_H
