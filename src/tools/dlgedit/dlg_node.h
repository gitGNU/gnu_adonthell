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
 * @file dlg_node.h
 *
 * @author Kai Sterker
 * @brief Base class for the dialogue objects.
 */

#ifndef DLG_NODE_H
#define DLG_NODE_H

#include <list>
#include <string>
#include <stdio.h>
#include "dlg_types.h"
#include "dlg_node_gfx.h"


// Definitions needed for the dialogue source file parser
extern int parse_dlgfile (string&, int&);
extern FILE *loadlgin;


/**
 * This is the base class for all dialogue objects. As this, it has three
 * major purposes:
 *
 * - provide means for a graphical representation of the node in the Graph
 *   window. This functionality is inherited from the DlgNodeGfx class.
 *
 * - provide the glue between different Nodes, thus forming the actual graph.
 *
 * - provide a unique interface to the different flavours of Nodes in the
 *   graph.
 */
class DlgNode : public DlgNodeGfx
{
public:
    DlgNode () { }
    DlgNode (node_type t, DlgRect area);

    /** 
     * @name Methods for graph traversal
     */
    //@{
    /**
     * Get a node from this node's list of direct followers.
     * @param pos Defines what to do with the iterator of that list.
     * @param offset Move the iterator by that offset.
     * @return The node found at the specified offset from the given
     *         iterator position.
     * @see query_type
     */
    DlgNode* next (query_type pos, int offset = 1);
    /**
     * Get a node from this node's list of direct precedessors.
     * @param pos Defines what to do with the iterator of that list.
     * @param offset Move the iterator by that offset.
     * @return The node found at the specified offset from the given
     *         iterator position.
     * @see query_type
     */
    DlgNode* prev (query_type pos, int offset = 1);  
    //@}

    /**
     * @name Node insertion
     */
    //@{
    /**
     * Add a node to the list of precedessors. The nodes are sorted by their
     * position. The ones with the smallest y value come first; if nodes have
     * the same y value, then those with the smallest x value come first. 
     * @param node the DlgNode to add.
     */
    void addPrev (DlgNode *node);
    /**
     * Add a node to the list of followers.
     * @param node the DlgNode to add.
     */
    void addNext (DlgNode *node);
    //@}
    
    /**
     * Draw this node to the given surface. During drawing, the mode is
     * temporarily changed to the given mode.
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     * @param mode the new mode of the node
     */
    void draw (GdkPixmap *surface, DlgPoint &offset, mode_type mode);
    
    virtual void draw (GdkPixmap *surface, DlgPoint &offset) = 0;
    
    /**
     * Get the type of the node.
     * @return the type of the node.
     */
    node_type type ()       { return type_; }
        
protected:
    node_type type_;
        
    list<DlgNode*> prev_;
    list<DlgNode*> next_;    

    list<DlgNode*>::iterator p;
    list<DlgNode*>::iterator n;
    
    DlgNode* getNode (list<DlgNode*>::iterator &it, list<DlgNode*> &lst,
            query_type &pos, int &offset);
};

#endif // DLG_NODE_H
