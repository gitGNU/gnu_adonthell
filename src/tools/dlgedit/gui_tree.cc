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
 * @file gui_tree.cc
 *
 * @author Kai Sterker
 * @brief View for the dialogue structure
 */

#include <gtk/gtk.h>
#include "gui_tree.h"
#include "gui_dlgedit.h"

#define INSERT_NODE(parent,text,is_leaf) \
    gtk_ctree_insert_node (GTK_CTREE (tree), parent, NULL, text, 0, \
        NULL, NULL, NULL, NULL, is_leaf, true)

// selection changed
void on_tree_select_row (GtkCTree *ctree, GList *node, gint column, gpointer user_data)
{
    gtk_ctree_unselect (ctree, GTK_CTREE_NODE (node));
    
    DlgModule *module = (DlgModule *) gtk_ctree_node_get_row_data (ctree,
        GTK_CTREE_NODE (node));
    
    DlgModule *current = GuiDlgedit::window->graph ()->getAttached ();
    
    if (module != current)
    {
        // unmark current selection
        GuiTree *view = (GuiTree *) user_data;
        view->select (GTK_CTREE_NODE (node));
        
        // display the newly selected module
        GuiDlgedit::window->graph ()->switchModule (module);
    }
}

// create tree widget
GuiTree::GuiTree (GtkWidget *paned)
{
    GtkWidget *scrolledwindow;

    // the scrolled window for the tree
    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow);
    gtk_object_set_data_full (GTK_OBJECT (paned), "scrolledwindow",
        scrolledwindow, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow);
    gtk_paned_add1 (GTK_PANED (paned), scrolledwindow);
    gtk_widget_set_usize (scrolledwindow, 160, -2);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // the tree widget
    tree = gtk_ctree_new (1, 0);
    gtk_widget_ref (tree);
    gtk_object_set_data_full (GTK_OBJECT (paned), "tree", tree,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (tree);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), tree);
    gtk_clist_set_column_width (GTK_CLIST (tree), 0, 80);
    gtk_clist_column_titles_hide (GTK_CLIST (tree));
    gtk_clist_set_shadow_type (GTK_CLIST (tree), GTK_SHADOW_IN);
    GTK_WIDGET_UNSET_FLAGS (tree, GTK_CAN_FOCUS);

    gtk_signal_connect (GTK_OBJECT (tree), "tree_select_row",
        GTK_SIGNAL_FUNC (on_tree_select_row), this);
    
    selected = NULL;
}

// dtor
GuiTree::~GuiTree ()
{
}

// display the given module
void GuiTree::display (DlgModule *module)
{
    // is module valid?
    if (module == NULL) return;

    // clear the tree
    clear ();

    // build the tree
    build (insert ((GtkCTreeNode*) NULL, module));
}

// build the initial tree
void GuiTree::build (GtkCTreeNode *root)
{
    DlgModule *module = (DlgModule *) gtk_ctree_node_get_row_data (GTK_CTREE (tree), root);
    std::vector<DlgNode*> nodes = module->getNodes ();
    
    // search through all nodes of the given module
    for (std::vector<DlgNode*>::iterator i = nodes.begin (); i != nodes.end (); i++)
        // if we find a sub-dialogue
        if ((*i)->type () == MODULE)
            // insert it into the tree and continue with its sub-dialogues
            build (insert (root, (DlgModule *) *i));
}

// insert a module into the tree       
GtkCTreeNode *GuiTree::insert (GtkCTreeNode *parent, DlgModule *module)
{
    gchar *text = ((char*) module->name ().c_str ());
    GtkCTreeNode *node = INSERT_NODE (parent, &text, false);
    gtk_ctree_node_set_row_data (GTK_CTREE (tree), node, (gpointer) module);
    
    // display the module that has been in the view before
    if (module->displayed ())
        on_tree_select_row (GTK_CTREE (tree), (GList*) node, 0, this); 
    
    return node;
}
 
// insert a module into the tree       
void GuiTree::insert (DlgModule *parent, DlgModule *module)
{
    GtkCTreeNode *root = gtk_ctree_node_nth (GTK_CTREE (tree), 0);
    if (root == NULL) return;

    GtkCTreeNode *node = gtk_ctree_find_by_row_data (GTK_CTREE (tree), root,
        (gpointer) parent);
    if (node == NULL) return;
    
    insert (node, module);
}

// highlight node in view
void GuiTree::select (GtkCTreeNode *node)
{
    if (node == selected) return;
    
    // first of all, deselect old node
    if (selected != NULL)
    {
        GdkColor black = { 0, 0, 0, 0 };
        gtk_ctree_node_set_foreground (GTK_CTREE (tree), selected, &black);
    }
    
    // then select the new node
    if (node != NULL)
    {
        GdkColor red = { 0, 65535, 0, 0 };
        gtk_ctree_node_set_foreground (GTK_CTREE (tree), node, &red);
    }
    
    // remember selection
    selected = node;
}

// select the given module
void GuiTree::select (DlgModule *module)
{
    GtkCTreeNode *root = gtk_ctree_node_nth (GTK_CTREE (tree), 0);
    
    if (root == NULL) return;

    select (gtk_ctree_find_by_row_data (GTK_CTREE (tree), root, (gpointer) module));        
}

// remove everything from the tree
void GuiTree::clear ()
{
    GtkCTreeNode *root = gtk_ctree_node_nth (GTK_CTREE (tree), 0);
    
    if (root == NULL) return;
    else gtk_ctree_remove_node (GTK_CTREE (tree), root);
    
    // no tree, so nothing selected
    selected = NULL;
}
