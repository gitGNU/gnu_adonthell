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
#include <fstream>
#include "dlg_node_gfx.h"


// Definitions needed for the dialogue source file parser
extern int parse_dlgfile (std::string&, int&);
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
    DlgNode () { index_ = -1; }
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
     * @name Node removal
     */
    //@{
    /**
     * Remove a node from the list of precedessors.
     * @param node the DlgNode to remove.
     */
    void removePrev (DlgNode *node);
    /**
     * Remove a node from the list of followers.
     * @param node the DlgNode to remove.
     */
    void removeNext (DlgNode *node);
    //@}

    /**
     * Draw this node to the given surface. During drawing, the mode is
     * temporarily changed to the given mode.
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     * @param mode the new mode of the node
     */
    void draw (GdkPixmap *surface, DlgPoint &offset, mode_type mode);
    
    /**
     * Draw this node to the given surface.
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    virtual void draw (GdkPixmap *surface, DlgPoint &offset) { }
    
    /**
     * Save a node to file
     * @param out Stream to the file to save to
     */
    virtual void save (std::ofstream &out) { }
    
    /**
     * Get the type of the node.
     * @return the type of the node.
     */
    node_type &type ()      { return type_; }
    
    /**
     * Set the node's position in the dlg_module::nodes array.
     * @param i position of the node.
     */
    void setIndex (int i)   { index_ = i; }

    /**
     * Get the node's position in the dlg_module::nodes array.
     * @return position of the node.
     */
    int index ()            { return index_; }
    
    /**
     * Return the node's unique id. This is given when the node is
     * created and will never change from then on. It can be used
     * to correctly identify nodes of subdialogues and the like.
     * @return unique id of the node.
     */
    int node_id ()          { return nid_; }
    
    /**
     * Returns id of the module this node belongs to. It allows us 
     * to tell nodes of different modules apart. Together with the
     * node id, this gives each node a unique id.
     * @return module id.
     */
    std::string module_id (){ return mid_; }
        
protected:
    node_type type_;                    // type of the node
    int index_;                         // position of node in file when saving
    int nid_;                           // unique node id
    std::string mid_;                   // unique module id
    
    std::list<DlgNode*> prev_;          // list of node's parents
    std::list<DlgNode*> next_;          // list of node's children

    std::list<DlgNode*>::iterator p;    // iterator over parents
    std::list<DlgNode*>::iterator n;    // iterator over children
    
    DlgNode* getNode (std::list<DlgNode*>::iterator &it, 
        std::list<DlgNode*> &lst, query_type &pos, int &offset);
};

#endif // DLG_NODE_H
