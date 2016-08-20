/*
   Copyright (C) 2002/2004 Kai Sterker <kai.sterker@gmail.com>
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
 * @file common/gui_file.cc
 *
 * @author Kai Sterker
 * @brief Wrapper for the GtkFileChooser widget
 */

#include <gtk/gtk.h>
#include "gui_file.h"
#include "util.h"

// create a new file selection window
GuiFile::GuiFile (GtkWindow *parent, GtkFileChooserAction action, const std::string &title, const std::string &file) 
: GuiModalDialog (parent)
{
    const gchar *label;
    
    switch (action)
    {
        case GTK_FILE_CHOOSER_ACTION_SAVE:
        {
            label = GTK_STOCK_SAVE;
            break;
        }
        case GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER:
        {
            label = GTK_STOCK_NEW;
            break;
        }
        default: label = GTK_STOCK_OPEN;
    }
    
    window = gtk_file_chooser_dialog_new (title.c_str (), parent, action,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, label, GTK_RESPONSE_ACCEPT, NULL);
    
    // forbid multiple selections
    gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER(window), FALSE);
    
    // set current directory or file
    if (file[file.length()-1] == '/')
    {
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(window), file.c_str ());
    }
    else
    {
        gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(window), file.c_str ());
    }
    
    if (action == GTK_FILE_CHOOSER_ACTION_SAVE || action == GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER)
    {
        gchar *name = g_path_get_basename (file.c_str ());
        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(window), name);
        g_free (name);
    }
}

// clean up
GuiFile::~GuiFile ()
{
}

// display file select widget
bool GuiFile::run ()
{
    if (gtk_dialog_run (GTK_DIALOG (window)) == GTK_RESPONSE_ACCEPT)
    {
        gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (window));
        File = MK_UNIX_PATH (filename);
        g_free (filename);
        
        return true;
    }
    
    return false;
}

void GuiFile::add_shortcut (const std::string & shortcut)
{
    if (shortcut.length () > 0) 
    {
    	gtk_file_chooser_add_shortcut_folder (GTK_FILE_CHOOSER(window), shortcut.c_str (), NULL);
    }
}

void GuiFile::add_filter (const std::string & pattern, const std::string & name)
{
    unsigned long pos;
    std::string pattern_str = pattern;
    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, name.c_str ());

    while ((pos = pattern_str.find ('|')) != std::string::npos)
    {
        std::string filter_str = pattern_str.substr (0, pos);
        pattern_str = pattern_str.substr (pos + 1);
        gtk_file_filter_add_pattern (filter, filter_str.c_str ());
    }
    
    gtk_file_filter_add_pattern (filter, pattern_str.c_str ());
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(window), filter);    
    gtk_file_chooser_set_filter (GTK_FILE_CHOOSER(window), filter);
}
