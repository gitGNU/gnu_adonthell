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
 * @file dlg_mover.h
 *
 * @author Kai Sterker
 * @brief A dummy node for drag'n dropping DlgArrows.
 */

#ifndef DLG_MOVER_H
#define DLG_MOVER_H

#include "dlg_arrow.h"


/**
 * Arrows have to be connected to two nodes, so to drag them from one
 * Circle to another, it needs to be connected to an (invisble) mover.
 */
class DlgMover : public DlgNode
{
public:
    /**
     * Creates a new mover at the given position.
     * @param position The center of the new mover
     */
    DlgMover (DlgPoint &position);
    
    /**
     * Attach an arrow to this mover. The method checks whether either
     * the tip or the tail of the arrow overlap the mover and so decides
     * how the arrow is dragged.
     * @param arrow a DlgArrow to be dragged.
     * @return <b>true</b> if the mover is close enough to the arrows end,
     *      <b>false</b> otherwise.
     */
    bool attach (DlgArrow *arrow);
    
    /**
     * Drop the mover onto the given node. Connects the arrow we are 
     * moving to the node, if possible.
     * @param node A DlgNode or <b>NULL</b> if there is no node at the
     *      cursor position.
     */
    void drop (DlgNode *node);
    
private:
    int moving;                 // is arrow dragged by it's tip or tail?
    DlgNode *oldCircle;         // the node the arrow was attached to before
    
    enum { NONE, TIP, TAIL };
};

#endif // DLG_MOVER_H
