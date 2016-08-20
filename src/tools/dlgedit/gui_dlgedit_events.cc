/*
   Copyright (C) 1999/2002/2004 Kai Sterker <kai.sterker@gmail.com>
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
 * @file gui_dlgedit_events.cc
 *
 * @author Kai Sterker
 * @brief The Events triggered via the Dialogue Editor's main window
 */

#include "cfg_data.h"
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
    GtkWindow *parent = (GtkWindow *) dlgedit->getWindow ();

    GuiFile fs (parent, GTK_FILE_CHOOSER_ACTION_OPEN, "Load dialogue source", dlgedit->directory ());
    fs.add_filter ("*" FILE_EXT, "Adonthell Dialogue Source");

    // set shortcuts
    const std::vector<std::string> & projects = CfgData::data->projectsFromDatadir ();
    for (std::vector<std::string>::const_iterator i = projects.begin (); i != projects.end (); i++)
    {
        const std::string &dir = CfgData::data->getBasedir (*i);
        fs.add_shortcut (dir);
    }

    // File selection closed with OK
    if (fs.run ()) dlgedit->loadDialogue (fs.getSelection ());
}

// File Menu: Save As
void on_file_save_as_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    GtkWindow *parent = (GtkWindow *) dlgedit->getWindow ();

    GuiFile fs (parent, GTK_FILE_CHOOSER_ACTION_SAVE, "Save dialogue source", dlgedit->filename ());
    fs.add_filter ("*" FILE_EXT, "Adonthell Dialogue Source");

    // set shortcuts
    const std::vector<std::string> & projects = CfgData::data->projectsFromDatadir ();
    for (std::vector<std::string>::const_iterator i = projects.begin (); i != projects.end (); i++)
    {
        const std::string &dir = CfgData::data->getBasedir (*i);
        fs.add_shortcut (dir);
    }

    // File selection closed with OK
    if (fs.run ()) dlgedit->saveDialogue (fs.getSelection ());
}

// File Menu: Save
void on_file_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    std::string filename = dlgedit->filename ();

    // only save dialogues that aren't 'untitled'
    if (filename.find ("untitled.adlg") == filename.npos)
        dlgedit->saveDialogue (filename);
    // otherwise open file selection
    else
        on_file_save_as_activate (menuitem, user_data);
}

// File Menu: Revert to Saved
void on_file_revert_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit::window->revertDialogue ();
}

// File Menu: Close
void on_file_close_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit::window->closeDialogue ();
}

// Dialogue Menu: Compile
void on_dialogue_compile_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit::window->compileDialogue ();    
}

// Dialogue Menu: Settings
void on_dialogue_player_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit::window->settings ();
}

// Dialogue Menu: Functions
void on_dialogue_functions_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    dlgedit->customCode ();
}

// Dialogue Menu: Preview i18n
void on_dialogue_preview_activate (GtkMenuItem * menuitem, gpointer user_data)
{
    GuiDlgedit *dlgedit = (GuiDlgedit *) user_data;
    GtkWindow *parent = (GtkWindow *) dlgedit->getWindow ();

    GuiFile fs (parent, GTK_FILE_CHOOSER_ACTION_OPEN, "Select translation", dlgedit->directory ());
    fs.add_filter ("*.mo", "Gettext Message Catalogue");
    
    // start translation preview
    if (dlgedit->mode () != L10N_PREVIEW)
    {
        // File selection closed with OK
        if (fs.run ()) dlgedit->previewTranslation (fs.getSelection ());
    }
    // stop translation preview
    else dlgedit->exitPreview ();
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

// Display help text associated with a menuitem to the statusbar 
gboolean on_display_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
    int id = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (widget), "help-id"));
    GuiMessages *message = (GuiMessages *) user_data;
    
    message->display (id);
    return FALSE;
}

// Clear text displayed by the statusbar 
gboolean on_clear_help (GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
    GuiMessages *message = (GuiMessages *) user_data;
    message->clear ();
    return FALSE;
}
