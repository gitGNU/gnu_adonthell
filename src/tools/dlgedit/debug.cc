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
#include "run.h"
#include "debug.h"
#include "dbg_interface.h"
#include "main.h"
#include "Python.h"

gint strcompare (gpointer a, gpointer b)
{
    return strcmp ((char *) a, (char *) b);
}

// set to 1 if debug-window is open
int debug_dlg::destroy = 0;

debug_dlg::debug_dlg (MainFrame *w) : wnd (w)
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
	GtkCTreeNode *parent, *node;
    GtkCTree *tree;
    char *obj;
    
    switch (active_page)
    {
        case 0:
        {
            tree = GTK_CTREE (dlg_tree);
            container = NULL;
            break;
        }
        case 1:
        {
            tree = GTK_CTREE (char_tree);
            container = &game::characters;
            break;
        }
        case 2:
        {
            tree = GTK_CTREE (quest_tree);
            container = &game::quests;
            break;
        }
        default: return;
    }

    parent = gtk_ctree_node_nth (tree, selected_row);
    tree_row = GTK_CTREE_ROW (parent);

    // Change attribute
    if (tree_row->parent)
    {
	    obj = (char *) gtk_ctree_node_get_row_data (tree, tree_row->parent);
        gtk_ctree_node_set_text (tree, parent, 1, val);
    }
    // add new attribute
    else
    {
        obj = (char *) gtk_ctree_node_get_row_data (tree, parent);

        // first check if the attribute doesn't already exist
        node = gtk_ctree_find_by_row_data_custom (tree, parent, key, (GCompareFunc) strcompare);

        // if it does, simply update it's value
        if (node) gtk_ctree_node_set_text (tree, node, 1, val);
        // ... else add a new node
        else
        {
            char *text[2] = { key, val };

            node = gtk_ctree_insert_node (tree, parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data (tree, node, strdup (key));
        }
    }
    
    if (container != NULL)
    {
        object = container->get (obj);
        object->set (key, ival);
    }
    else set_dlg_var (key, ival);
}

void debug_dlg::init ()
{
    GtkCTreeNode *parent = NULL, *node;
    pair<const char*, s_int32> mypair;
    objects *container;
    character *mychar;
    storage *myobj;
    char *text[2], str[32];

    // Init Dialogue page
    container = get_dlg_vars ();
    gtk_clist_freeze (GTK_CLIST (dlg_tree));
    if ((myobj = (storage *) container->get ("Local Variables")) != NULL)
    {
        text[0] = "Local Variables";
        text[1] = "";

        parent = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, TRUE);
        gtk_ctree_node_set_row_data (GTK_CTREE (dlg_tree), parent, text[0]);

        while ((mypair = myobj->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            node = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data (GTK_CTREE (dlg_tree), node, text[0]);
        }

        delete myobj;
    }
    else
    {
        text[0] = "No Local Variables available";
        text[1] = "";

        parent = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
        gtk_ctree_node_set_row_data (GTK_CTREE (dlg_tree), parent, text[0]);
    }
    delete container;
    gtk_clist_thaw (GTK_CLIST (dlg_tree));
    

    // Init character page
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

objects *debug_dlg::get_dlg_vars ()
{
    PyObject *instance, *keys, *values, *vars, *k, *v;
    objects *container = new objects;
    storage *object = new storage;
    int size, i;

    if (wnd->test_dlg == NULL) return container;
    instance = wnd->test_dlg->get_instance ();
    vars = PyObject_GetAttrString (instance, "debug_info");

    if (vars == NULL) return container;
    container->set ("Local Variables", object);

    keys = PyDict_Keys (vars);
    values = PyDict_Values (vars);

    size = PyList_Size (values);

    for (i = 0; i < size; i++)
    {
        k = PyList_GetItem (keys, i);
        v = PyList_GetItem (values, i);

        object->set (strdup (PyString_AsString (k)), PyInt_AsLong (v));
    }

    Py_DECREF (keys);
    Py_DECREF (values);

    return container;
}

void debug_dlg::set_dlg_var (char *key, int value)
{
    PyObject *instance;

    if (wnd->test_dlg == NULL) return;
    instance = wnd->test_dlg->get_instance ();

    PyObject_CallMethod (instance, "set_var", "si", key, value);
}