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
#include <hash_map>

// #include "../../types.h"
#include "../../character.h"
#include "../../game.h"
#include "debug.h"
#include "dbg_interface.h"
#include "main.h"

// set to 1 if debug-window is open
int debug_dlg::destroy = 0;

debug_dlg::debug_dlg ()
{
    destroy = 1;
    dlg = create_debug_wnd (this);
    init ();
}

debug_dlg::~debug_dlg ()
{
    wnd->dbg_dlg = NULL;
}

void debug_dlg::update ()
{
}

void debug_dlg::init ()
{
    GtkCTreeNode *parent = NULL;
    pair<const char*, s_int32> mypair;
    character *mychar;
    char *text[2], str[32];

    gtk_clist_freeze (GTK_CLIST (char_tree));

    while ((mychar = (character *) game::characters.next ()) != NULL)
    {
        text[0] = mychar->name;
        text[1] = "";

        parent = gtk_ctree_insert_node (GTK_CTREE (char_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);

        while ((mypair = mychar->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            gtk_ctree_insert_node (GTK_CTREE (char_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
        }
    }

    gtk_clist_thaw (GTK_CLIST (char_tree));
}