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
 * @file gui_settings.cc
 *
 * @author Kai Sterker
 * @brief The Dialogue settings dialog.
 */

#include <gtk/gtk.h>
#include <sys/stat.h>
#include <iostream>
#include "gui_file.h"
#include "gui_dlgedit.h"
#include "gui_settings.h"

// global pointer to the settings dialog
GuiSettings *GuiSettings::dialog = NULL;

// browse the harddisk for a project
void on_browse_project_clicked (GtkButton *button, gpointer user_data)
{    
    GuiFile fs (LOAD, "Select project directory", DATA_DIR"/games/");
    
    // File selection closed with OK
    if (fs.run ())
    {
        // check if we have a directory
        struct stat statbuf;
        std::string file = fs.getSelection ();
        stat (file.c_str (), &statbuf);
        
        if (S_ISDIR (statbuf.st_mode))
        {
            // extract the projectname from the directory
            GtkEntry *entry = (GtkEntry *) user_data;
            unsigned int pos, len = file.length () - 1;
            
            if (file[len] == '/')
            {
                pos = file.rfind ('/', len - 1) + 1;
                gtk_entry_set_text (entry, file.substr (pos, len - pos).c_str ());
            }
            else
            {
                pos = file.rfind ('/') + 1;
                gtk_entry_set_text (entry, file.substr (pos).c_str ());
            }
        }
    }
}

// OK button pressed
void on_ok_button_clicked (GtkButton *button, gpointer user_data)
{
    GuiSettings::dialog->applyChanges ();
    delete GuiSettings::dialog;
}

// callback for closing the window
void on_close_settings (GtkButton *button, gpointer user_data)
{
    delete GuiSettings::dialog;
}

// ctor
GuiSettings::GuiSettings ()
{
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *hseparator;
    GtkWidget *hbuttonbox;

    GtkTooltips *tooltips = gtk_tooltips_new ();

    // the dialog window
    window = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (window), "window", window);
    gtk_widget_set_usize (window, 400, 320);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_policy (GTK_WINDOW (window), FALSE, FALSE, FALSE);

    vbox = gtk_vbox_new (FALSE, 2);
    gtk_widget_ref (vbox);
    gtk_object_set_data_full (GTK_OBJECT (window), "vbox", vbox, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    // the project entry
    hbox = gtk_hbox_new (FALSE, 4);
    gtk_widget_ref (hbox);
    gtk_object_set_data_full (GTK_OBJECT (window), "hbox", hbox, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 4);

    label = gtk_label_new ("Project");
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (window), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
    gtk_misc_set_padding (GTK_MISC (label), 12, 0);

    project = gtk_entry_new ();
    gtk_widget_ref (project);
    gtk_object_set_data_full (GTK_OBJECT (window), "project", project, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (project);
    gtk_box_pack_start (GTK_BOX (hbox), project, TRUE, FALSE, 0);
    gtk_widget_set_usize (project, 275, -2);
    gtk_tooltips_set_tip (tooltips, project, "The project this dialogue belongs to, e.g. wastesedge", NULL);

    button = gtk_button_new_with_label ("...");
    gtk_widget_ref (button);
    gtk_object_set_data_full (GTK_OBJECT (window), "button", button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, FALSE, 0);
    gtk_widget_set_usize (button, 30, -2);
    gtk_tooltips_set_tip (tooltips, button, "Browse your HD for a project", NULL);
    gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (on_browse_project_clicked), project);

    // seperator
    hseparator = gtk_hseparator_new ();
    gtk_widget_ref (hseparator);
    gtk_object_set_data_full (GTK_OBJECT (window), "hseparator", hseparator, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator);
    gtk_box_pack_start (GTK_BOX (vbox), hseparator, FALSE, FALSE, 0);

    // OK and Cancel buttons
    hbuttonbox = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox);
    gtk_object_set_data_full (GTK_OBJECT (window), "hbuttonbox", hbuttonbox, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbuttonbox, FALSE, FALSE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox), 0);

    button = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (button);
    gtk_object_set_data_full (GTK_OBJECT (window), "button", button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (on_close_settings), NULL);

    button = gtk_button_new_with_label ("OK");
    gtk_widget_ref (button);
    gtk_object_set_data_full (GTK_OBJECT (window), "button", button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), button);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (on_ok_button_clicked), NULL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (on_close_settings), NULL);
    gtk_object_set_data (GTK_OBJECT (window), "tooltips", tooltips);

    // set transient for dialogue editor main window
    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (GuiDlgedit::window->getWindow ()));    

    dialog = this;
    entry = NULL;
}

// dtor
GuiSettings::~GuiSettings ()
{
    gtk_widget_destroy (window);
    dialog = NULL;
}

// display the dialog
void GuiSettings::display (DlgModuleEntry *e, const std::string &name)
{
    if (entry != e)
    {
        entry = e;
    
        // display the contents of the module
        gtk_entry_set_text (GTK_ENTRY (project), entry->project ().c_str ());
    }
    
    // set the title
    gchar *title = g_strjoin (NULL, "Settings - [", 
        name.c_str (), "]", NULL);
    gtk_window_set_title (GTK_WINDOW (window), title);

    // now show the window
    gtk_widget_show (window);
}

// store the user's entries
void GuiSettings::applyChanges ()
{
    if (!entry->setProject (gtk_entry_get_text (GTK_ENTRY (project))))
        std::cout << "Loading quests/characters failed!\n";
}
