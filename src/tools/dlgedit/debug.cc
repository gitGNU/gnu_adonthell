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
    active_page = 0;
    selected_row = -1;
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

void debug_dlg::set (char *key, char *val)
{
    int ival = atoi (val);
    storage *object;
    objects *container;
    GtkCTreeRow *tree_row;
	GtkCTreeNode *node;
    GtkCTree *tree;
    char *obj;
    
    switch (active_page)
    {
        case 0:
        {
            tree = GTK_CTREE (char_tree);
            container = &game::characters;
            break;
        }
        case 1:
        {
            tree = GTK_CTREE (quest_tree);
            container = &game::quests;
            break;
        }
        default: return;
    }

    node = gtk_ctree_node_nth (tree, selected_row);
    tree_row = GTK_CTREE_ROW (node);

    // Change attribute
    if (tree_row->parent)
    {
	    obj = (char *) gtk_ctree_node_get_row_data (tree, tree_row->parent);
        gtk_ctree_node_set_text (tree, node, 1, val);
    }
    // add new attribute
    else
    {
        char *text[2] = { key, val };
       
	    obj = (char *) gtk_ctree_node_get_row_data (tree, node);
        node = gtk_ctree_insert_node (tree, node, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
        gtk_ctree_node_set_row_data (tree, node, strdup (key));
    }
    
    object = container->get (obj);
    object->set (key, ival);
}

void debug_dlg::init ()
{
    GtkCTreeNode *parent = NULL, *node;
    pair<const char*, s_int32> mypair;
    character *mychar;
    char *text[2], str[32];

    gtk_clist_freeze (GTK_CLIST (char_tree));

    while ((mychar = (character *) game::characters.next ()) != NULL)
    {
        text[0] = mychar->name;
        text[1] = "";

        parent = gtk_ctree_insert_node (GTK_CTREE (char_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);
        gtk_ctree_node_set_row_data (GTK_CTREE (char_tree), parent, text[0]);

        while ((mypair = mychar->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            node = gtk_ctree_insert_node (GTK_CTREE (char_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data (GTK_CTREE (char_tree), node, text[0]);
        }
    }

    gtk_clist_thaw (GTK_CLIST (char_tree));
}