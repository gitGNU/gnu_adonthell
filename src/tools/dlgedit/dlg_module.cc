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
#include "dlg_circle.h"
#include "dlg_arrow.h"

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

// select the dialogue's root mode.
bool DlgModule::selectRoot ()
{
    for (vector<DlgNode*>::iterator i = nodes.begin(); i != nodes.end (); i++)
        if ((*i)->type () != LINK)
            if ((*i)->prev (FIRST) == NULL)
                return selectNode (*i);
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

// load a dialogue from file
bool DlgModule::load ()
{
    int i = 1, n, y_min = 0, x_min = 0, x_max = 0;
    DlgCircle *circle;
    DlgArrow *arrow;
    string s;

    // load all nodes and toplevel items
    while (i)
    {
        switch (i = parse_dlgfile (s, n))
        {
            case LOAD_VARS:
            {
                if (parse_dlgfile (s, n) == LOAD_STR);
                break;
            }

            case LOAD_FUNC:
            {
                if (parse_dlgfile (s, n) == LOAD_STR);
                break;
            }

            case LOAD_NAME:
            {
                if (parse_dlgfile (s, n) == LOAD_STR)
                {/*
                    data::characters.erase (wnd->myplayer->get_name().c_str ());
                    wnd->myplayer->set_name (s);
                    data::characters[wnd->myplayer->get_name().c_str ()] =
                        (character*) wnd->myplayer;
                */}
                break;
            }

            case LOAD_RACE:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); //->myplayer->set_val ("race", n);
                break;
            }

            case LOAD_GENDER:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); //->myplayer->set_val ("gender", n);
                break;
            }

            case LOAD_NPC:
            {
                // racter *mynpc = NULL;

                if (parse_dlgfile (s, n) == LOAD_STR);
                //    mynpc = data::characters.find (s.c_str ())->second;

                // if (mynpc == NULL)
                // {
                //     dictionary <character *>::const_iterator itc = data::characters.begin ();
                //     mynpc = itc->second;
                // }

                // wnd->set_npc (mynpc);
                break;
            }

            case LOAD_CIRCLE:
            {
                circle = new DlgCircle;
                circle->load ();

                nodes.push_back (circle);

                // Get extension of the graph for proper displaying
                if (circle->y () < y_min) y_min = circle->y ();

                if (circle->x () < x_min) x_min = circle->x ();
                else if (circle->x () > x_max) x_max = circle->x ();

                break;
            }

            case LOAD_ARROW:
            {
                arrow = new DlgArrow;
                arrow->load (nodes);

                nodes.push_back (arrow);
                break;
            }

            default: break;
        }
    }

    return true;
}