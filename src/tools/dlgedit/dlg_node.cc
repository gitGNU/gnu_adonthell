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
 * @file dlg_node.cc
 *
 * @author Kai Sterker
 * @brief Base class for the dialogue objects.
 */

#include "dlg_node.h"

// Coonstructor
DlgNode::DlgNode (node_type t, DlgRect area)
{
    type_ = t;
    index_ = -1;
    top_left = area.topLeft ();
    bottom_right = area.bottomRight ();
}

// Get the next node
DlgNode* DlgNode::next (query_type pos, int offset)
{
    if (next_.empty ()) return NULL;
    else return getNode (n, next_, pos, offset);
}

// Get the previous node
DlgNode* DlgNode::prev (query_type pos, int offset)
{
    if (prev_.empty ()) return NULL;
    else return getNode (p, prev_, pos, offset);
}

// Get a node from the specified list
DlgNode* DlgNode::getNode (list<DlgNode*>::iterator &it, 
    list<DlgNode*> &lst, query_type &pos, int &offset)
{ 
    switch (pos)
    {
        case FIRST:
            // point to the next element
            it = lst.begin ();
            offset -= 1;
            break;
        case NEXT:
            // nothing needs be done
            break;
        case LAST:
            // point to the last element
            it = lst.end ();
            offset = -offset;
            break;
        case PREV:
            // reverse searching
            offset = -offset;
            break;
        case CURRENT:
            if (it != lst.end ()) return *it;
            else return *(lst.begin ());
            break;
        default:
            return NULL;
    }
    
    // forward iteration
    if (offset > 0)
    {
        for (int i = 0; i < offset && it != lst.end (); i++, it++);
        if (it != lst.end ()) return *it;
    }
    
    // backward iteration
    else
    {
        int i;
        for (i = offset; i < 0 && it != lst.begin (); i++, it--);
        if (i == 0) return *it;
    }
    
    return NULL;
}

// add node to list of followers
void DlgNode::addNext (DlgNode *node)
{
    list<DlgNode*>::iterator i = next_.begin ();
    DlgRect *cmp = node->type () == LINK ? node->prev (FIRST) : node;

    // search the proper place for insertion
    if (cmp != NULL && !next_.empty ()) 
        while (i != next_.end () && *(*i) < *cmp) i++;
    
    // insert
    next_.insert (i, node);
}

// add node to list of precedessors
void DlgNode::addPrev (DlgNode *node)
{
    list<DlgNode*>::iterator i = prev_.begin ();
    DlgRect *cmp = node->type () == LINK ? node->next (FIRST) : node;
    
    // search the proper place for insertion
    if (cmp != NULL && !prev_.empty ()) 
        while (i != prev_.end () && *(*i) < *cmp) i++;
    
    // insert
    prev_.insert (i, node);
}

// draw the node (with a certain mode)
void DlgNode::draw (GdkPixmap *surface, DlgPoint &offset, mode_type m)
{
    // change the mode temporarily
    mode_type oldMode = mode_;
    mode_ = m;
    
    // draw
    draw (surface, offset);
    
    // restore old mode
    mode_ = oldMode; 
}

