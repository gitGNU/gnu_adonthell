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
 * @file gui_settings.h
 *
 * @author Kai Sterker
 * @brief The Dialogue settings dialog.
 */

#ifndef GUI_SETTINGS_H
#define GUI_SETTINGS_H

#include <gtk/gtkstyle.h>
#include "dlg_module_entry.h"

/**
 * Interface to various generic dialogue settings, like the project it belongs
 * to or the NPC it will be attached to. Most of these settings are optional.
 */
class GuiSettings
{
public:
    /**
     * Create empty Settings Dialog.
     */
    GuiSettings ();

    /**
     * Cleanup.
     */
    ~GuiSettings ();
    
    /**
     * Global pointer to the settings dialog
     */
    static GuiSettings *dialog;
    
    /**
      * Show the dialog and display the settings of the given module.
     * @param entry the settings of a certain DlgModule to be edited.
     * @param name name of the dialogue whose settings are being edited.
     */
    void display (DlgModuleEntry *entry, const std::string &name);

    /**
     * Store the user's entries once the OK button has been pressed.
     */
    void applyChanges ();

private:
    DlgModuleEntry *entry;  // The module's settings
    GtkWidget *window;      // The dialog window
    GtkWidget *project;     // Dropdown list for the project
    GtkWidget *description; // Entry for the dialogue's description
    GtkWidget *basedir;     // Entry for the project's base directory
};

#endif // GUI_SETTINGS_H
