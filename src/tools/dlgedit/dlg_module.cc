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
 * @file dlg_module.cc
 *
 * @author Kai Sterker
 * @brief One complete dialogue or sub-dialogue.
 */

#include <algorithm>
#include "dlg_module.h"

DlgModule::DlgModule (string n, string d)
{
    type_ = MODULE;
    name_ = n;
    selected_ = NULL;
    description_ = d;
    changed_ = false;
    mode_ = NONE;
}

// select a given node
bool DlgModule::selectNode (DlgNode *node)
{
    // try to find the given node in our array of nodes
    vector<DlgNode*>::iterator i = find (nodes.begin (), nodes.end (), node);
    
    // if it is not there, return
    if (i == nodes.end ()) return false;
    
    // see if a node is already selected
    if (selected_ != NULL) return false;
    
    // remember which node is selected
    selected_ = node;
    
    // set the mode of dialogue and node
    mode_ = NODE_SELECTED;
    node->setMode (NODE_SELECTED);

    return true;
}

// deselect the selected node
DlgNode* DlgModule::deselectNode ()
{
    DlgNode *retval = selected_;
    
    // see if a node is selected at all
    if (selected_ == NULL) return NULL;

    // unselect node
    mode_ = NONE;
    selected_->setMode (NONE);
    
    // nothing selected
    selected_ = NULL;
    
    // return the node for drawing
    return retval;
}
