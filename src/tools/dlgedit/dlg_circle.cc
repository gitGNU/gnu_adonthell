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
    bottom_right = DlgPoint (p.x () + CIRCLE_DIAMETER, p.y () + CIRCLE_DIAMETER);
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
    else myParent = parent (NEXT, 0);

    if (myParent != NULL)
    {
        // see if we can get a sibling in the direction we are searching
        mySibling = myParent->child (pos, offset);
        
        // if not, see if there is another parent in that direction
        if (mySibling == NULL && (pos == NEXT || pos == PREV))
        {
            prev (pos, 1);
            mySibling = sibling (pos, offset);
        }
    }
    
    return mySibling;
}

// draw the circle
void DlgCircle::draw (GdkPixmap *surface, DlgPoint &os)
{
    GdkGC *gc;

    // get the color for drawing the circle
    gc = GuiDlgedit::window->getColor (mode_, type_);
    
    // offset circle
    DlgPoint position = topLeft ().offset (os);
    DlgRect area = DlgRect (position, CIRCLE_DIAMETER, CIRCLE_DIAMETER);
            
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
