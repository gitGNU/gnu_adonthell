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
 * Interface to various generic dialogue and project settings. Currently this
 * includes
 * - the project of the dialogue
 * - the dialogue's description
 * - the base directory of the above project
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

    /**
     * @name Member access
     */
    //@{}
    /**
     * Set the base directory according to selected project. Call this
     * method whenever the user assigns a different project.
     * @param project the name of the selected project.
     */
    void setBasedir (const std::string & project);
    /**
     * Return the basedir associated with the selected project.
     * @return The basedir of the selected project.
     */
    std::string getBasedir ();
    /**
     * Set the project list to the project the module is assigned to.
     * If that module is not in the list yet, it will be added.
     * @param project name of the module's project.
     */
    void setProject (const std::string & project);
    /**
     * Return the project the user has selected in the list.
     * @return The name of the selected project.
     */
    std::string getProject ();
    //@}
    
private:
    /**
     * Display the module's description. The text area is cleared
     * if neccessary.
     * @param desc the module's description.
     */
    void setDescription (const std::string & desc);
    /**
     * Get the Description the user has entered for the module.
     * @return the module's description.
     */
    std::string getDescription ();

    /**
     * Read available projects from DlgCmdline::datadir and add
     * them to the project list. Also add the "none" entry for
     * modules that do not belong to a project yet.
     * @param the menu widget of the drop-down list. 
     */
    void populateProjects (GtkWidget *menu);

    DlgModuleEntry *entry;  // The module's settings
    GtkWidget *window;      // The dialog window
    GtkWidget *project;     // Dropdown list for the project
    GtkWidget *description; // Entry for the dialogue's description
    GtkWidget *basedir;     // Entry for the project's base directory
};

#endif // GUI_SETTINGS_H
