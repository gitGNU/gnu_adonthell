/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gtk/gtk.h>

#include "run.h"
#include "run_interface.h"
#include "debug.h"
#include "../../dialog.h"
#include "../../game.h"
#include "../../py_inc.h"

int run_dlg::destroy = 0;

run_dlg::run_dlg (MainFrame *w) : wnd (w)
{
    dat = NULL;
    destroy = 1;

    dlg = create_run_dlg_wnd (this);
    gtk_window_set_transient_for (GTK_WINDOW (dlg), GTK_WINDOW (wnd->wnd));

    char *path = g_dirname (wnd->file_name);
    insert_path (path);
    free (path);

    start ();
}

run_dlg::~run_dlg ()
{
    wnd->test_dlg = NULL;
    delete dat;
}

void run_dlg::start ()
{
    char *file = g_basename (wnd->file_name);

    if (dat) delete dat;
    dat = new dialog;

    // Import module
    if (!dat->init (file, file))
    {
        cout << "\n*** Error loading dialogue script! ";
        show_traceback ();
        cout << flush;
        answer = -1;
    }

    // The start of the dialogue
    else answer = 0;
}

PyObject *run_dlg::get_instance ()
{
    return dat->get_instance ();
}

void run_dlg::run ()
{
    u_int32 i;
    tmp_list = NULL;
    GtkAdjustment *adj;

    // Error occured:
    if (answer < 0) return;

    // Run the interpreter
    dat->run ((u_int32) answer);

    // Update the debug window
    if (debug_dlg::destroy != 0) wnd->dbg_dlg->update ();

    // See if we reached the end of the script
    if (dat->text() == NULL) return;

    // Now examine  dat->text  and see what we've got
    // case 1: only one NPC line
    if (dat->text_size() == 1)
    {
        // Fill the list with the text (i.e add an empty line and then the
        // clickable NPC text
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -2));
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->text()[0], 5, 0));
    }
    
    // case 2: there is NPC text and Player text available
    else
    {
        // Fill in the NPC text
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -1));
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->text()[0], 4, -1));

        // Now look for the player text
        for (i = 1; i < dat->text_size(); i++)
            tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->text()[i], 3, i));            
    }

    // Now stuff everything into the list
    gtk_list_append_items (GTK_LIST (list), tmp_list);

    // and scroll to its bottom (gtk_list_scroll_vertical () didn't work!)
    adj = gtk_viewport_get_vadjustment ((GtkViewport *) list->parent);
    gtk_adjustment_set_value (adj, adj->upper);
    
    return;
}
