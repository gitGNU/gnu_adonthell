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
 * @file dlg_node_gfx.h
 *
 * @author Kai Sterker
 * @brief Base class for a DlgNode's graphical representation.
 */

#ifndef DLG_NODE_GFX_H
#define DLG_NODE_GFX_H

#include <gdk/gdktypes.h>
#include "dlg_rect.h"
#include "dlg_types.h"

/**
 * This class provide the interface for the graphical representation of 
 * the node in the graph window. This includes functionality needed for
 * the actual rendering of the node, as well as positional information
 * (inherited from DlgRect).
 */
class DlgNodeGfx : public DlgRect
{
public:
    DlgNodeGfx () { }
    DlgNodeGfx (DlgPoint &position);
    virtual ~DlgNodeGfx () { }
    
    /** 
     * Change the mode of a node.
     * @param m the new mode.
     */
    void setMode (mode_type m)  { mode_ = m; }
    
    /** 
     * Get the current mode of a node.
     * @return the node's mode.
     */
    mode_type mode ()           { return mode_; }
    
    /**
     * Draw this node to the given surface with the specified offset. 
     * The node's current mode determines the color thw node is drawn with. 
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    virtual void draw (GdkPixmap *surface, DlgPoint &offset) = 0;

    /**
     * Draw this node to the given surface. During drawing, the mode is
     * temporarily changed to the given mode.
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     * @param mode the new mode of the node
     */
    virtual void draw (GdkPixmap *surface, DlgPoint &offset, mode_type mode) = 0;
       
    /**
     * Check whether the node is located at the given point.
     * @return <b>true</b> if this is the case, <b>false</b> otherwise.
     */
    virtual bool operator== (DlgPoint &point) { return contains (point); }
    
protected:
    mode_type mode_;        // This nodes mode (NONE, HILIGHTED, SELECTED)
};

#endif // DLG_NODE_GFX_H
