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
 * @file dlg_circle.cc
 *
 * @author Kai Sterker
 * @brief The node containing the actual dialogue text.
 */

#include <gtk/gtk.h>
#include "dlg_circle.h"
#include "gui_dlgedit.h"

// Constructor
DlgCircle::DlgCircle (node_type t, DlgCircleEntry *e, DlgPoint &p)
{
    type_ = t;
    entry_ = e;
    
    // Align Circle to the (imaginary) grid
    top_left = DlgPoint (p.x () - (p.x () % CIRCLE_DIAMETER), p.y () - (p.y () % CIRCLE_DIAMETER));
    bottom_right = DlgPoint (x () + CIRCLE_DIAMETER, y () + CIRCLE_DIAMETER);
}

// get a certain parent-circle of this circle
DlgCircle *DlgCircle::parent (query_type pos, int offset)
{
    DlgNode *arrow = prev (pos, offset);
    if (arrow != NULL) return (DlgCircle *) arrow->prev (FIRST);
    else return NULL;
}

// get a certain child-circle of this circle
DlgCircle *DlgCircle::child (query_type pos, int offset)
{
    DlgNode *arrow = next (pos, offset);
    if (arrow != NULL) return (DlgCircle *) arrow->next (FIRST);
    else return NULL;
}

// get a certain sibling-circle of this circle
DlgCircle *DlgCircle::sibling (query_type pos, int offset)
{
    DlgCircle *myParent, *mySibling = NULL;
    
    // first, get the right parent circle
    if (pos == FIRST || pos == LAST) myParent = parent (pos, offset);
    else myParent = parent (FIRST);
    
    if (myParent != NULL)
    {
        // first, locate this circle
        for (mySibling = myParent->child (FIRST);
             mySibling != this; mySibling = myParent->child (NEXT));
             
        // see if we can get a sibling in the direction we are searching
        mySibling = myParent->child (pos, offset);
        
        // if not, see if there is another parent in that direction
        /*if (mySibling == NULL && (pos == NEXT || pos == PREV))
        {
            prev (pos, 1);
            mySibling = sibling (pos, offset);
        }*/
    }
    
    return mySibling;
}

// draw the circle
void DlgCircle::draw (GdkPixmap *surface, DlgPoint &os)
{
    // get the color for drawing the circle
    GdkGC *gc = GuiDlgedit::window->getColor (mode_, type_);
    
    // offset circle
    DlgPoint position = topLeft ().offset (os);
    DlgRect area (position, CIRCLE_DIAMETER+1, CIRCLE_DIAMETER+1);
    
    // draw everything to the surface
    gdk_draw_arc (surface, GuiDlgedit::window->getColor (GC_WHITE), TRUE, position.x (), position.y (), 20, 20, 0, 36000);
    gdk_draw_arc (surface, gc, FALSE, position.x (), position.y (), 20, 20, 0, 36000);
    
    // Indicate wether node contains additional code
    if (hasCode ())
    {
        GString *code = g_string_new ("!");
        
        // get the font to use
        GdkFont *font = GuiDlgedit::window->font ();
    
        // place text in circles center
        int x = position.x () + (20 - gdk_string_width (font, code->str)) / 2;
        int y = position.y () + (21 + gdk_string_height (font, code->str)) / 2;
        gdk_draw_string (surface, font, gc, x, y, code->str);

        g_string_free (code, TRUE);
    }
    
    // Update the drawing area
    GuiDlgedit::window->graph ()->update (area);
}

// load a circle from a file
bool DlgCircle::load ()
{
    entry_ = new DlgCircleEntry;
    string str;
    int n;

    // as long as we haven't reached EOF or are finished with loading:
    while (1)
    {
        // look what we find in the file
        switch (parse_dlgfile (str, n))
        {
            // EOF or finished
            case 0:
            case LOAD_END: return true;

            // Type of node
            case LOAD_TYPE:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) type_ = (node_type) n;

                break;
            }

            // Coordinates of Circle
            case LOAD_POS:
            {
                int px, py;
                if (parse_dlgfile (str, n) == LOAD_NUM) px = n;
                if (parse_dlgfile (str, n) == LOAD_NUM) py = n;

                // Align Circle to the (imaginary) grid
                top_left = DlgPoint (px - (px % CIRCLE_DIAMETER), py - (py % CIRCLE_DIAMETER));
                bottom_right = DlgPoint (x () + CIRCLE_DIAMETER, y () + CIRCLE_DIAMETER);
                break;
            }

            // The Circle's Text
            case LOAD_TEXT:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) entry_->setText (str);
                break;
            }

            // The Circle's Annotations
            case LOAD_NOTE:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) entry_->setAnnotation (str);
                break;
            }

            // The Circle's Character
            case LOAD_NPC:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) entry_->setNpc (str);
                break;
            }

            // The Circle's Conditions
            case LOAD_COND:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) entry_->setCondition (str);
                break;
            }

            // The Circle's Variables
            case LOAD_VARS:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) entry_->setCode (str);
                break;
            }

            // Just ignore everything else
            default: break;
        }
    }

    return true;
}

// get the text of a circle
string DlgCircle::text ()
{
    string text = "";
    
    // is there any text at all?
    if (entry_ == NULL) return text;

    // is a certain NPC attached?
    if (entry_->npc () != "Default")
    {
        text += entry_->npc ();
        text += ": ";
    }
    
    // append actual text and return it
    return text += entry_->text ();
}
