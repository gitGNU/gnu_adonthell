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
 * @file dlg_circle.h
 *
 * @author Kai Sterker
 * @brief The node containing the actual dialogue text.
 */

#ifndef DLG_CIRCLE_H
#define DLG_CIRCLE_H

#include "dlg_node.h"
#include "dlg_circle_entry.h"

/**
 * This is the node that contains the actual dialogue text and associated
 * attributes, such as conditions or arbitrary (Python) code.
 *
 * There are three types of circles: Player, NPC and Narrator nodes.
 * - Player nodes contain text assigned to the person in front of the screen.
 * - NPC nodes are assigned to a certain, computer controlled entity.
 * - Narrator nodes are assigned to an (imaginary) voice from the off.  
 */
class DlgCircle : public DlgNode
{
public:
    DlgCircle () { mode_ = IDLE; last_parent = NULL; }
    DlgCircle (DlgPoint &p, node_type t, DlgCircleEntry *e = NULL);
    ~DlgCircle ();
    
    /** 
     * @name Methods for graph traversal
     */
    //@{
    /**
     * This method retrieves a %DlgCircle preceeding this node, i.e.
     * (one of) it's parent(s). 
     * @param pos Defines from what position to start retrieving the parent.
     * @param offset Get the parent found at the specified offset from the
     *        position given above.
     * @return The %DlgCircle found at the specified offset from the 
     *        given position.
     * @see query_type
     */
    DlgCircle* parent (query_type pos, int offset = 1);  
    /**
     * This method retrieves a %DlgCircle following this node, i.e.
     * (one of) it's child(ren). 
     * @param pos Defines from what position to start retrieving the child.
     * @param offset Get the child found at the specified offset from the
     *        position given above.
     * @return The %DlgCircle found at the specified offset from the 
     *        given position.
     * @see query_type
     */
    DlgCircle* child (query_type pos, int offset = 1);  
    /**
     * This method retrieves %DlgCircle's that are the children of
     * this node's parent(s), i.e. it's sibling(s).  
     * @param pos Defines from what position to start retrieving the sibling.
     * @param offset Get the sibling found at the specified offset from the
     *        position given above.
     * @return The %DlgCircle found at the specified offset from the 
     *        given position.
     * @see query_type
     */
    DlgCircle* sibling (query_type pos, int offset = 1);  
    //@}

    /**
     * load circle from a file. The file to load from is a global variable, 
     * as loading is handled by a scanner generated with flex.
     * @return <b>true</b> on success, <b>false</b> otherwise.
     */
    bool load ();
    
    /**
     * save a circle to a file
     * @param file Stream to an open file
     */
    void save (ofstream &file);

    /**
     * Draw this node to the given surface with the specified offset. 
     * The node's current mode determines the color thw node is drawn with. 
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    void draw (GdkPixmap *surface, DlgPoint &offset);

    /**
     * Get the contents of this node.
     * @return a DlgCircleEntry with the circle's text, code, conditions, etc.
     */
    DlgCircleEntry *entry ()            { return entry_; }
    
    /**
     * Get the text of this circle
     * @return a string of the form "<NPC>: <Text>" if a certain NPC is 
     *         attached to the node, otherwise returns only the text.
     */
    string text ();
    
    /**
     * Get the text of this circle for use with the GuiTooltip
     * @return a string of the form "<condition>\n<Text>\n<code>".
     */
    string tooltip ();

    /**
     * Check whether any arbitrary Python code is assigned to this circle.
     * @return <b>true</b> if this is the case, <b>false</b> otherwise.
     */
    bool hasCode ()                     { return entry_->hasCode (); }
    
    /**
     * Check whether the given node is a child of this circle
     * @return <b>true</b> if this is the case, <b>false</b> otherwise.
     */
    bool hasChild (DlgNode *child);
    
    DlgCircle *lastParent ()            { return last_parent; }
    void setLastParent (DlgCircle *p)   { last_parent = p; }
private:
    DlgCircle *last_parent;
    DlgCircleEntry *entry_;
};

#endif // DLG_CIRCLE_H
