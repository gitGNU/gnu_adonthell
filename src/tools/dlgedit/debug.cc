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
#ifdef GCC_3
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include "../../character.h"
#include "../../quest.h"
#include "run.h"
#include "debug.h"
#include "dbg_interface.h"
#include "main.h"
#include "Python.h"

// Compares two nodes by their data
gint strcompare (gpointer a, gpointer b)
{
    dbg_node_data *data = (dbg_node_data *) a;
    
    return strcmp (data->attribute, (char *) b);
}

// Deletes the data when a node is removed from the tree
void destroy_data (gpointer n)
{
    dbg_node_data *data = (dbg_node_data *) n;

    delete data->attribute;
    delete data;
}

// Check whether a node is "touched"
gint is_touched (gpointer a, gpointer b)
{
    dbg_node_data *data = (dbg_node_data *) a;
    int retval = data->touched;
    data->touched ^= 1;
    return retval;
}

// set to 1 if debug-window is open
int debug_dlg::destroy = 0;

debug_dlg::debug_dlg (MainFrame *w) : wnd (w)
{
    destroy = 1;
    active_page = 0;
    selected_row = -1;
    dlg = create_debug_wnd (this);

    green.red = 0;
    green.green = 35000;
    green.blue = 0;

    black.red = 0;
    black.green = 0;
    black.blue = 0;

    red.red = 65535;
    red.green = 0;
    red.blue = 0;

    init ();
}

debug_dlg::~debug_dlg ()
{
    wnd->dbg_dlg = NULL;
}

// Update the info and highlight changed attributes
void debug_dlg::update ()
{
    GtkCTreeNode *parent = NULL;
    GtkCTree *tree;
    dbg_node_data *data;
    objects *container;
    storage *myobj;
    char *text[2];

    // Update local dialogue variables
    container = get_dlg_vars ();
    tree = GTK_CTREE (dlg_tree);
    parent = gtk_ctree_node_nth (tree, 0);
    data = (dbg_node_data *) gtk_ctree_node_get_row_data (tree, parent);

    gtk_clist_freeze (GTK_CLIST (dlg_tree));
    if ((myobj = (storage *) container->next ()) != NULL)
    {
        // No loacal variables available so far, so add some
        if (!strcmp ("No Local Variables available", data->attribute))
        {
            text[0] = "Local Variables";
            text[1] = "";
            
            gtk_ctree_remove_node (tree, parent);
            
            data = new dbg_node_data (strdup (text[0]), 2);
            parent = gtk_ctree_insert_node (tree, NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);
            gtk_ctree_node_set_row_data_full (tree, parent, data, destroy_data);
        }

        // See whether the current nodes children have changed
        if (update_children (tree, parent, myobj))
            // if so, also highlight the parent
            gtk_ctree_node_set_foreground (tree, parent, &green);
        else gtk_ctree_node_set_foreground (tree, parent, &black);
    }
    // No local variables available
    else
    {
        // If old ones are displayed, remove them
        if (!strcmp ("Local Variables", data->attribute))
        {
            text[0] = "No Local Variables available";
            text[1] = "";
            
            gtk_ctree_remove_node (tree, parent);

            data = new dbg_node_data (strdup (text[0]), 2);
            parent = gtk_ctree_insert_node (tree, NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);
            gtk_ctree_node_set_row_data_full (tree, parent, data, destroy_data);
        }
    }
    // Now check whether some variables were removed
    check_removed (tree);
    delete container;
    gtk_clist_thaw (GTK_CLIST (dlg_tree));

    // update character tree
    tree = GTK_CTREE (char_tree);
    gtk_clist_freeze (GTK_CLIST (char_tree));

    dictionary <character *>::iterator itc; 
    for (itc = data::characters.begin (); itc != data::characters.end () && itc->second != NULL; itc++) 
    {
        parent = gtk_ctree_find_by_row_data_custom (tree, NULL, (void *) itc->second->get_name().c_str (),
                                                    (GCompareFunc) strcompare);

        // See whether the current node's children have changed
        if (update_children (tree, parent, itc->second))
            // if so, also highlight the parent
            gtk_ctree_node_set_foreground (tree, parent, &green);
        else gtk_ctree_node_set_foreground (tree, parent, &black);
    }

    // Now check whether some variables were removed
    check_removed (tree);
    gtk_clist_thaw (GTK_CLIST (char_tree));

    // update quest tree
    tree = GTK_CTREE (quest_tree);
    gtk_clist_freeze (GTK_CLIST (quest_tree));

    dictionary <quest *>::iterator itq; 
    for (itq = data::quests.begin (); itq != data::quests.end () && itq->second != NULL; itq++) 
    {
        parent = gtk_ctree_find_by_row_data_custom (tree, NULL, (void *) itq->second->name.c_str (), (GCompareFunc) strcompare);

        // See whether the current node's children have changed
        if (update_children (tree, parent, itq->second))
            // if so, also highlight the parent
            gtk_ctree_node_set_foreground (tree, parent, &green);
        else gtk_ctree_node_set_foreground (tree, parent, &black);
    }

    // Now check whether some variables were removed
    check_removed (tree);
    gtk_clist_thaw (GTK_CLIST (quest_tree));
}

int debug_dlg::update_children (GtkCTree *tree, GtkCTreeNode *parent, storage *myobj)
{
    char *text[2], str[32], *key, *val;
    pair<const char*, s_int32> mypair;
    dbg_node_data *data;
    GtkCTreeNode *node;    
    int changed = 0;

    while ((mypair = myobj->next ()).first != NULL)
    {
        key = strdup (mypair.first);
        sprintf (str, "%i", mypair.second);

        // Try to find this attribute in the tree
        node = gtk_ctree_find_by_row_data_custom (tree, parent, key, (GCompareFunc) strcompare);

        // found, so check whether it has changed
        if (node != NULL)
        {
            data = (dbg_node_data *) gtk_ctree_node_get_row_data (tree, node);
            data->touched = 1;

            gtk_ctree_node_get_text (tree, node, 1, &val);

            // It has changed, so highlight it green
            if (strcmp (str, val))
            {
                changed = 1;
                gtk_ctree_node_set_text (tree, node, 1, str);
                gtk_ctree_node_set_foreground (tree, node, &green);
            }
            else gtk_ctree_node_set_foreground (tree, node, &black);
        }
        // not found, so we have to add it
        else
        {
            changed = 1;
            text[0] = key;
            text[1] = str;

            data = new dbg_node_data (strdup (text[0]), 1);
            node = gtk_ctree_insert_node (tree, parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data_full (tree, node, data, destroy_data);
            gtk_ctree_node_set_foreground (tree, node, &green);
        }
    }

    return changed;
}

// "Remove" all untouched nodes from the tree
void debug_dlg::check_removed (GtkCTree *tree)
{
    GList *untouched = gtk_ctree_find_all_by_row_data_custom (tree, NULL, NULL, (GCompareFunc) is_touched);
    GtkCTreeNode *node;

    while (untouched != NULL)
    {
        node = GTK_CTREE_NODE (untouched->data);
        
        gtk_ctree_node_set_text (tree, node, 1, "0");
        gtk_ctree_node_set_foreground (tree, node, &red);

        untouched = g_list_next (untouched);
    }
}

// Add or change a certain attribute
void debug_dlg::set (char *key, char *val)
{
    int ival = atoi (val);
    storage *object;
    dictionary <character *>  *containerc = NULL;
    dictionary <quest *>  *containerq = NULL;
    dbg_node_data *data;
    GtkCTreeRow *tree_row;
	GtkCTreeNode *parent, *node;
    GtkCTree *tree;
    char *attribute;
    
    
    switch (active_page)
    {
        case 0:
        {
            tree = GTK_CTREE (dlg_tree);
            //             container = NULL;
            break;
        }
        case 1:
        {
            tree = GTK_CTREE (char_tree);
            containerc = &data::characters;
            break;
        }
        case 2:
        {
            tree = GTK_CTREE (quest_tree);
            containerq = &data::quests;
            break;
        }
        default: return;
    }

    parent = gtk_ctree_node_nth (tree, selected_row);
    tree_row = GTK_CTREE_ROW (parent);

    // Change attribute
    if (tree_row->parent)
    {
        data = (dbg_node_data *) gtk_ctree_node_get_row_data (tree, tree_row->parent);
        attribute = data->attribute;
        gtk_ctree_node_set_text (tree, parent, 1, val);
        gtk_ctree_node_set_foreground (tree, parent, &black);
    }
    // add new attribute
    else
    {
        data = (dbg_node_data *) gtk_ctree_node_get_row_data (tree, parent);
        attribute = data->attribute;
        
        // first check if the attribute doesn't already exist
        node = gtk_ctree_find_by_row_data_custom (tree, parent, key, (GCompareFunc) strcompare);

        // if it does, simply update it's value
        if (node) 
        {
            gtk_ctree_node_set_text (tree, node, 1, val);
            gtk_ctree_node_set_foreground (tree, node, &black);
        }
        // ... else add a new node
        else
        {
            char *text[2] = { key, val };

            data = new dbg_node_data (strdup (key), 0);
            node = gtk_ctree_insert_node (tree, parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data_full (tree, node, data, destroy_data);
        }
    }
    
    if (containerc != NULL)
    {
        object = (*containerc)[attribute];
        object->set_val (key, ival);
    }
    else if (containerq != NULL)
    {
        object = (*containerq)[attribute];
        object->set_val (key, ival);
    }
    else set_dlg_var (key, ival);
}

void debug_dlg::init ()
{
    dbg_node_data *data;
    GtkCTreeNode *parent = NULL, *node;
    pair<const char*, s_int32> mypair;
    objects *container;
    storage *myobj;
    char *text[2], str[32];

    // Init Dialogue page
    container = get_dlg_vars ();
    gtk_clist_freeze (GTK_CLIST (dlg_tree));

    if ((myobj = (storage *) container->next ()) != NULL)
    {
        text[0] = "Local Variables";
        text[1] = "";

        data = new dbg_node_data (strdup (text[0]), 2);
        parent = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, TRUE);
        gtk_ctree_node_set_row_data_full (GTK_CTREE (dlg_tree), parent, data, destroy_data);

        while ((mypair = myobj->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            data = new dbg_node_data (text[0], 0);
            node = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data_full (GTK_CTREE (dlg_tree), node, data, destroy_data);
        }

        delete myobj;
    }
    else
    {
        text[0] = "No Local Variables available";
        text[1] = "";

        data = new dbg_node_data (strdup (text[0]), 2);
        parent = gtk_ctree_insert_node (GTK_CTREE (dlg_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
        gtk_ctree_node_set_row_data_full (GTK_CTREE (dlg_tree), parent, data, destroy_data);
    }
    delete container;
    gtk_clist_thaw (GTK_CLIST (dlg_tree));

    // Init character page
    gtk_clist_freeze (GTK_CLIST (char_tree));

    dictionary <character *>::iterator itc; 
    for (itc = data::characters.begin (); itc != data::characters.end (); itc++) 
    {
        if (itc->second == NULL) continue;

        text[0] = (char *) itc->second->get_name().c_str ();
        text[1] = "";

        data = new dbg_node_data (strdup (text[0]), 2);
        parent = gtk_ctree_insert_node (GTK_CTREE (char_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);
        gtk_ctree_node_set_row_data_full (GTK_CTREE (char_tree), parent, data, destroy_data);

        while ((mypair = itc->second->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            data = new dbg_node_data (text[0], 0);
            node = gtk_ctree_insert_node (GTK_CTREE (char_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data_full (GTK_CTREE (char_tree), node, data, destroy_data);
        }
    }
    gtk_clist_thaw (GTK_CLIST (char_tree));

    // Init quest page
    gtk_clist_freeze (GTK_CLIST (quest_tree));
    dictionary <quest *>::iterator itq; 
    for (itq = data::quests.begin (); itq != data::quests.end (); itq++) 
    {
        if (itq->second == NULL) continue;

        text[0] = (char *) itq->second->name.c_str ();
        text[1] = "";

        data = new dbg_node_data (strdup (text[0]), 2);
        parent = gtk_ctree_insert_node (GTK_CTREE (quest_tree), NULL, NULL, text, 0, NULL, NULL, NULL, NULL, FALSE, FALSE);
        gtk_ctree_node_set_row_data_full (GTK_CTREE (quest_tree), parent, data, destroy_data);

        while ((mypair = itq->second->next ()).first != NULL)
        {
            sprintf (str, "%i", mypair.second);

            text[0] = strdup (mypair.first);
            text[1] = str;

            data = new dbg_node_data (text[0], 0);
            node = gtk_ctree_insert_node (GTK_CTREE (quest_tree), parent, NULL, text, 0, NULL, NULL, NULL, NULL, TRUE, FALSE);
            gtk_ctree_node_set_row_data_full (GTK_CTREE (quest_tree), node, data, destroy_data);
        }
    }
    gtk_clist_thaw (GTK_CLIST (quest_tree));
}

objects *debug_dlg::get_dlg_vars ()
{
    PyObject *instance, *keys, *values, *vars, *k, *v;
    objects *container = new objects;
    storage *object;
    int size, i;

    if (wnd->test_dlg == NULL) return container;

    instance = wnd->test_dlg->get_instance ();
    if (instance == NULL) return container;

    vars = PyObject_GetAttrString (instance, "debug_info");
    if (vars == NULL) return container;

    keys = PyDict_Keys (vars);
    if (keys == NULL) return container;
    values = PyDict_Values (vars);

    size = PyList_Size (values);
    object = new storage;
    container->set_val ("Local Variables", object);

    for (i = 0; i < size; i++)
    {
        k = PyList_GetItem (keys, i);
        v = PyList_GetItem (values, i);

        object->set_val (strdup (PyString_AsString (k)), PyInt_AsLong (v));
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
