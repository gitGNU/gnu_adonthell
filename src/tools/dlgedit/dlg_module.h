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
 * @file dlg_module.h
 *
 * @author Kai Sterker
 * @brief One complete dialogue or sub-dialogue.
 */

#ifndef DLG_MODULE_H
#define DLG_MODULE_H

#include <vector>
#include <string>
#include "dlg_node.h"

/**
 * This class contains a dialogue, made up of elementary nodes and
 * (optionally) sub-dialogues. It is different from the other 
 * nodes as it contains a complete dialogue, but can also be part
 * of another dialogue.
 */
class DlgModule : public DlgNode
{
public:
    DlgModule (string n, string d);
    
    /**
     * Draw this node to the given surface with the specified offset. 
     * The node's current mode determines the color thw node is drawn with. 
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    void draw (GdkPixmap *surface, DlgPoint &offset) { }

    /**
     * Select a node from the list of nodes.
     * @param node The DlgNode to select.
     * @return <b>true</b> if the node has been selected, <b>false</b>
     *         otherwise.
     */
    bool selectNode (DlgNode *node);

    /**
     * Select the root node of the dialogue.
     * @return <b>true</b> if the node has been selected, <b>false</b>
     *         otherwise.
     */
    bool selectRoot ();

    /** 
     * Deselect a previously selected node.
     * @return the previously selected DlgNode, or <i>NULL</i> if no 
     *         node has been deselected
     */
    DlgNode* deselectNode ();

    /**
     * Get the node that is currently selected.
     * @return the DlgNode currently selected
     */
    DlgNode* selected ()            { return selected_; }

    /**
     * Init the Dialogue from a file
     * @return <b>true</b> if loading was successful, <b>false</b>
     *         otherwise.
     */
    bool load ();

    /**
     * Get the list of nodes in this dialogue.
     * @return a reference to the list of nodes.
     */
    vector<DlgNode*> &getNodes ()   { return nodes; }
    
    /**
     * Get the current offset of this dialogue.
     * @return a reference to the dialogue's offset.
     */
    DlgPoint &offset ()             { return offset_; }
    
    /**
     * Get the name of this dialogue.
     * @return a reference to the dialogue's name.
     */
    string &name ()                 { return name_; }
    
protected:
    vector<DlgNode*> nodes;     // all the nodes in this dialogue
    DlgNode *selected_;         // the node currently selected
    
    DlgPoint offset_;           // The current offset in the graph view
    bool changed_;              // Whether there were changes since saving
    mode_type mode_;            // Current state of this dialogue
    
    string name_;               // Short (file-) name of the dialogue 
    string description_;        // Description of the dialogue
};

#endif // DLG_MODULE_H
