/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

class dialog;

#include <stdio.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../interpreter.h"
#include "linked_list.h"
#include "dlgnode.h"
#include "main.h"
#include "graph.h"
#include "dlgrun.h"
#include "interface.h"

int 
main (int argc, char *argv[])
{
    /* The Application Data */
    MainFrame *MainWnd = new MainFrame;

    gtk_init (&argc, &argv);

    /* Misc initialization */
    init_app (MainWnd);

    MainWnd->wnd = NULL;
    MainWnd->text_dlg = NULL;
    MainWnd->pixmap = NULL;
    MainWnd->project = NULL;

    /* Create Top Level Window and Controls */
    create_mainframe (MainWnd);

    /* Event - Loop */
    gtk_main ();

    /* Clean up */
    delete_dialogue (MainWnd);
    delete MainWnd;

    return 0;
}

/* Set variables to safe values */
void 
init_app (MainFrame * MainWnd)
{
    MainWnd->mode = IDLE;
    MainWnd->number = 0;
    MainWnd->nodes = (ptr_list *) g_malloc (sizeof (ptr_list));
    MainWnd->nodes->size = 0;
    MainWnd->nodes->head = NULL; 
    MainWnd->below_pointer = NULL;
    MainWnd->selected_node = NULL;
    MainWnd->dragged_node = NULL;
    MainWnd->tooltip = NULL;
    MainWnd->file_name = g_strdup ("./new_dialogue");
    MainWnd->x_offset = 0;
    MainWnd->y_offset = 0;
}

/* free allocated memory */
void 
delete_dialogue (MainFrame * wnd)
{
    u_int32 i;

    /* Delete all nodes */
    for (i = 0; i < wnd->number; i++)
        free_node ((DlgNode *) get_ptr_list_element (wnd->nodes, i));

    clear_ptr_list (wnd->nodes);
    
    g_free (wnd->nodes);
    g_free (wnd->file_name);
}
