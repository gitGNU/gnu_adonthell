/*
   $Id$
   
   Copyright (C) 1999/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file gui_dlgedit_events.cc
 *
 * @author Kai Sterker
 * @brief The Events triggered via the Dialogue Editor's main window
 */

#include <gtk/gtk.h>
#include "gui_dlgedit.h"
#include "gui_file.h"

// Main Window: on_widget_destroy App
void on_widget_destroy (GtkWidget * widget, gpointer data)
{
    gtk_main_quit ();
    gtk_widget_destroy (widget);
}

// File Menu: New
void on_file_new_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    dlgedit->newDialogue ();
}

// File Menu: Load
void on_file_load_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    GuiFile fs (LOAD, "Load dialogue source", dlgedit->directory ());

    // File selection closed with OK
    if (fs.run ()) dlgedit->loadDialogue (fs.getSelection ());
}

// File Menu: Save
void on_file_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    GuiFile fs (SAVE, "Save dialogue source", dlgedit->filename ());

    // File selection closed with OK
    if (fs.run ()) dlgedit->saveDialogue (fs.getSelection ());
}

// File Menu: Close
void on_file_close_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit::window->closeDialogue ();
}

// Dialogue Menu: Compile
void on_dialogue_compile_activate (GtkMenuItem * menuitem, gpointer user_data)
{
/*
    MainFrame *wnd = (MainFrame *) user_data;
    dlg_compiler compiler (wnd->nodes, wnd->file_name, wnd->cust_func, wnd->pset_vars, wnd->debug);

    compiler.run ();

    gtk_widget_set_sensitive (wnd->dialogue_run, TRUE);
*/
}

// Dialogue Menu: Settings
void on_dialogue_player_activate (GtkMenuItem * menuitem, gpointer user_data)
{
/*
    MainFrame *wnd = (MainFrame *) user_data;
    
    preset_dlg dlg (wnd);
    dlg.run  ();
    
    gtk_main ();
*/
}

// Dialogue Menu: Functions
void on_dialogue_functions_activate (GtkMenuItem * menuitem, gpointer user_data)
{
/*
    MainFrame *wnd = (MainFrame *) user_data;
    
    function_dlg dlg (wnd);
    dlg.run  ();
    
    gtk_main ();
*/
}

// Dialogue Menu: Run
void on_dialogue_run_activate (GtkMenuItem * menuitem, gpointer user_data)
{
/*
    MainFrame *wnd = (MainFrame *) user_data;

    if (wnd->test_dlg == NULL)
    {
        wnd->test_dlg = new run_dlg (wnd);
        wnd->test_dlg->run ();
    }
*/
}

// Select an entry from the Window menu
void on_window_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    DlgModule *module = (DlgModule*) user_data;
    GuiDlgedit::window->showDialogue (module);
}
