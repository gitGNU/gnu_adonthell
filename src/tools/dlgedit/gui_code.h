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
 * @file gui_code.h
 *
 * @author Kai Sterker
 * @brief The Custom Code dialog.
 */

#ifndef GUI_CODE_H
#define GUI_CODE_H

#include "gui_edit.h"
#include "dlg_module_entry.h"

/**
 * Provides entries for additional import lines, for code to be inserted 
 * into constructor and destructor and for defining custom methods.
 */
class GuiCode
{
public:
    GuiCode ();
    ~GuiCode ();
    
    /**
     * Global pointer to the custom code dialog
     */
    static GuiCode *dialog;
    
    /**
     * Show the dialog and display the contents of the given module.
     * @param entry the custom code of a certain DlgModule to be edited.
     * @param name name of the dialogue whose code is being edited.
     */
    void display (DlgModuleEntry *entry, std::string &name);
    
    /**
     * Store the user's entries once the OK button has been pressed.
     */
    void applyChanges ();
    
private:
    DlgModuleEntry *entry;  // The module being edited
    GtkWidget *window;      // The dialog window
    GuiEdit *imports;       // entry for additional import statements
    GuiEdit *ctor;          // entry for code inserted into constructor
    GuiEdit *dtor;          // entry for code inserted into destructor
    GuiEdit *methods;       // entry for custom methods
};

#endif // GUI_CODE_H
