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

#include <gtk/gtkstyle.h>
#include "gtkeditor/gtkeditor.h"

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
    ~GuiEdit () { }

    /**
     * Get a pointer to the editor widget.
     * @return a pointer to the editor widget.
     */
    GtkEditor *widget ()        { return entry; }
    
private:
    GtkEditor *entry;
};
