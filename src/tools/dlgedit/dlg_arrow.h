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
 * @file dlg_arrow.h
 *
 * @author Kai Sterker
 * @brief The node linking two DlgCircle's.
 */

#ifndef DLG_ARROW_H
#define DLG_ARROW_H

#include <vector>
#include "dlg_node.h"

/**
 * This node types' sole purpose is to link any two DlgCircles.
 * It's graphical representation is an arrow, hence the name.  
 */
class DlgArrow : public DlgNode
{
public:
    DlgArrow () { mode_ = IDLE; }
    /**
     * Create a new link between two nodes.
     * @param s The node where the link starts.
     * @param e The node the link points to.  
     */
    DlgArrow (DlgNode *s, DlgNode *e);

    /**
     * Calculate the arrow's shape. Call this function every time either
     * it's start- or end-circle has moved.
     */
    void initShape ();
    
    /**
     * Draw this node to the given surface with the specified offset. 
     * The node's current mode determines the color thw node is drawn with. 
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    void draw (GdkPixmap *surface, DlgPoint &offset);

    /**
     * Init the Arrow from a file.  The file to load from is a global 
     * variable, as loading is handled by a scanner generated with flex.
     * @param nodes The rest of the dialogue, to connect this Arrow to
     * @return <b>true</b> if loading was successful, <b>false</b>
     *         otherwise.
     */
    bool load (vector<DlgNode*> &nodes);

    /**
     * save an Arrow to a file
     * @param file Stream to an open file
     */
    void save (ofstream &file);
    
    /**
     * Check whether the arrow is located at the given point.
     * @return <b>true</b> if this is the case, <b>false</b> otherwise.
     */
    bool operator== (DlgPoint &point);
    
    DlgPoint getTip ()      { return DlgPoint (line[1]); }
    DlgPoint getTail ()     { return DlgPoint (line[0]); }
    
private:
    GdkPoint line[2];       // the arrow's line
    GdkPoint tip[3];        // the arrow's tip
};

#endif // DLG_ARROW_H
