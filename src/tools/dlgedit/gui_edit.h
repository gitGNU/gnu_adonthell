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
 * @file gui_edit.h
 *
 * @author Kai Sterker
 * @brief Wrapper around the GtkText widget.
 */

#include <string>
#include <gtk/gtkstyle.h>
#include <gdk/gdktypes.h>

/**
 * Provides a simple C++ interface to GtkText. The widget is initialised
 * to use a font with fixed width, so it should only be used to edit code.
 */
class GuiEdit
{
public:
    /**
     * Create a new GtkText widget.
     * @param container Container to put the editor widget into.
     */
    GuiEdit (GtkWidget *container);
    ~GuiEdit ();

    /**
     * Get a pointer to the GtkText widget.
     * @return a pointer to the editor widget.
     */
    GtkWidget *widget ()            { return entry; }

    /**
     * Set the text of the widget
     * @param text to display in the entry
     */
    void setText (const std::string &text);

    /**
     * Retrieve the text of the widget
     * @return text contained in the widget.
     */
    std::string getText ();

private:
    GtkWidget *entry;          // The GtkText widget
};
