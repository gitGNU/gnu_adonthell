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

run_dlg::run_dlg ()
{
    dlg = create_run_dlg_wnd (this);

    // test
    GList *tmp_list = (GList *) NULL;

    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("NPC Text", 2, -1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Player Text", 1, -1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Another NPC Text", 2, -1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Another Player Text", 1, -1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("\nCurrent NPC Text", 4, -1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Player choice 1", 3, 0));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Player choice 2", 3, 1));
    tmp_list = g_list_append (tmp_list, create_dlg_list_item ("Player choice 3", 3, 2));

    gtk_widget_hide (list);
    gtk_list_append_items (GTK_LIST (list), tmp_list); 
    gtk_widget_show (list);
}