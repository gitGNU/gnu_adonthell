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
 * @file gui_tree.h
 *
 * @author Kai Sterker
 * @brief View for the dialogue structure
 */

#ifndef GUI_TREE_H
#define GUI_TREE_H

#include <gtk/gtkctree.h>
#include "dlg_module.h"

/**
 * The %GuiTree displays the structure of a dialogue. Its root
 * is the top-level dialogue, its children are the various
 * sub-dialogues.
 *
 * The current selection of the tree will always match the
 * dialogue currently displayed by the graph view. Clicking on
 * an entry will have the same effect as clicking on a 
 * sub-dialogue node the graph view, i.e. switch to that 
 * module. 
 */
class GuiTree
{
public:
    /**
     * Constructor. Creates the tree widget and adds it to the
     * given pane.
     * @param paned The GtkPaned that will hold the tree widget. 
     */
    GuiTree (GtkWidget* paned);
    /**
     * Standard desctructor.
     */
    ~GuiTree ();
    
    /**
     * Displays the given module in the tree. In case the module
     * is no top-level dialogue, this method will search backwards
     * through the list of parents until it finds the top-level
     * dialogue. It will then display the structure of this dialogue.
     *
     * @param module A module whose structure should be displayed.
     */
    void display (DlgModule *module);
    
    /**
     * Empty the widget.
     */
    void clear ();
    
    /**
     * Insert a module into the tree, as child of the given parent.
     * @param parent The parent of the module to add.
     * @param module The module to add.
     */
    void insert (DlgModule *parent, DlgModule *module);

    /**
     * Select the given node of the tree. If another node is selected,
     * it will be deselected first.
     * @param node The node to be selected.
     */
    void select (GtkCTreeNode *node);
    /**
     * Select the given module in the tree. If a node is currently
     * selected, it will be deselected first.
     * @param module The module to select.
     */
    void select (DlgModule *module);
    
private:
    /**
     * Build the initial module tree, by descending recursively
     * into the module's sub-dialogues, sub-sub-dialogues, and so on.
     * @param root The root of the (sub-)tree to be build.
     */
    void build (GtkCTreeNode *root);
    /**
     * Insert a module into the tree, as child of the given parent.
     * @param parent The parent of the module to add.
     * @param module The module to add.
     * @return The node that has been added.
     */
    GtkCTreeNode *insert (GtkCTreeNode *parent, DlgModule *module);
    
    GtkWidget *tree;        // The actual GTK+ tree widget
    GtkCTreeNode *selected; // The node currently 'selected'
};

#endif // GUI_TREE_H
