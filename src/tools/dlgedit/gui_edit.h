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
 * @brief Wrapper around the gtkeditor widget.
 */

#include <string>
#include <gtk/gtkstyle.h>
#include <gdk/gdktypes.h>

/**
 * The gtkeditor widget is a custom textbox widget with syntax-highlighting
 * abilities. This class provides the gtkeditor widget with Python support.
 */
class GuiEdit
{
public:
    /**
     * Create a new editor widget with Python syntax highlighting.
     * @param container Container to put the editor widget into.
     */
    GuiEdit (GtkWidget *container);
    ~GuiEdit ();

    /**
     * Get a pointer to the editor widget.
     * @return a pointer to the editor widget.
     */
    GtkWidget *widget ()            { return entry; }

    /**
     * Set the text of the widget
     * @param text to display in the entry
     */
    void setText (const string &text);

    /**
     * Retrieve the text of the widget
     * @return text contained in the widget.
     */
    string getText ();

private:
    GtkWidget *entry;          // The GtkText widget
    GdkFont *font;             // The font to use for the text
};
