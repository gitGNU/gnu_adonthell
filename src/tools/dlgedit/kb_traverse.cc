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
 * @file kb_traverse.cc
 *
 * @author Kai Sterker
 * @brief Graph traversal via keyboard
 */

#include <algorithm>
#include <vector>
#include "kb_traverse.h"

// ctor
KBTraverse::KBTraverse ()
{
}

// dtor
KBTraverse::~KBTraverse ()
{
    clear ();
}

// move to prev element
DlgNode* KBTraverse::left ()
{
    if (Siblings.empty ()) return NULL;

    // get prev node
    if (Current == Siblings.begin ())
        Current = Siblings.end ();

    Current--;

    // select it
    return *Current;
}

 // move to next element
DlgNode* KBTraverse::right ()
{
    if (Siblings.empty ()) return NULL;

    // get next node
    if (++Current == Siblings.end ())
        Current = Siblings.begin ();

    // select it
    return *Current;
}

// move to parent
DlgNode* KBTraverse::up ()
{
    if (Siblings.empty ()) return NULL;

    // try to select parent
    if (selectParents ()) return *Current;
    else return NULL;
}

// move to child
DlgNode* KBTraverse::down ()
{
    if (Siblings.empty ()) return NULL;

    // try to select child
    if (selectChildren ()) return *Current;
    else return NULL;
}

// select parents of a node
bool KBTraverse::selectParents ()
{
    DlgNode *parent;
    DlgNode *current = *Current;

    // get first parent
    DlgNode *arrow = current->prev (FIRST);
    if (arrow != NULL) parent = arrow->prev (FIRST);
    else return false;
    
    // cleanup
    clear ();

    while (parent != NULL)
    {
        addSibling (parent);
        
        // get subsequent parents
        arrow = current->prev (NEXT);
        if (arrow != NULL) parent = arrow->prev (FIRST);
        else parent = NULL;
    }

    // select first parent
    Current = Siblings.begin ();    

    return true;
}

// select children of a node
bool KBTraverse::selectChildren ()
{
    DlgNode *child;
    DlgNode *current = *Current;

    // get first parent
    DlgNode *arrow = current->next (FIRST);
    if (arrow != NULL) child = arrow->next (FIRST);
    else return false;

    // cleanup
    clear ();

    while (child != NULL)
    {
        addSibling (child);

        // get subsequent children
        arrow = current->next (NEXT);
        if (arrow != NULL) child = arrow->next (FIRST);
        else child = NULL;
    }

    // select first parent
    Current = Siblings.begin ();

    return true;
}

// select a certain node
bool KBTraverse::select (DlgNode *node)
{
    if (node == NULL || node->prev (FIRST) == NULL) return false;
    
    DlgNode *prev;

    // cleanup
    clear ();

    // make sure node ain't a link
    if (node->type () == LINK)
    {
        prev = node->prev (FIRST);
        node = node->next (FIRST);
    }
    else
    {
        prev = node->prev (FIRST)->prev (FIRST);
    }

    // select children of parent node (i.e. siblings of node)
    Siblings.push_back (prev);
    Current = Siblings.begin ();
    selectChildren ();

    // actually select node
    Current = find (Siblings.begin (), Siblings.end (), node);

    return true;
}

// select root node
DlgNode* KBTraverse::selectRoot (std::vector<DlgNode*> *nodes)
{
    std::vector<DlgNode*>::iterator i;

    // find all the root nodes
    for (i = nodes->begin (); i != nodes->end (); i++)
        if ((*i)->type () != LINK)
            if ((*i)->prev (FIRST) == NULL)
                addSibling (*i);

    // actually select node
    Current = Siblings.begin ();

    return *Current;   
}

// clear list of Siblings
void KBTraverse::clear ()
{
    Siblings.clear ();
    Current = Siblings.end ();
}

// add node to list of siblings
void KBTraverse::addSibling (DlgNode *node)
{
    std::list<DlgNode*>::iterator i = Siblings.begin ();
    DlgRect *cmp1, *cmp2 = node->type () == LINK ? node->next (FIRST) : node;

    // search the proper place for insertion
    while (i != Siblings.end ())
    {
        cmp1 = (*i)->type () == LINK ? (*i)->next (FIRST) : *i;
        if (*cmp2 < *cmp1) break;
        i++;
    }

    // insert
    Siblings.insert (i, node);
}
