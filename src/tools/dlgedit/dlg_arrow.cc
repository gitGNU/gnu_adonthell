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
 * @file dlg_arrow.cc
 *
 * @author Kai Sterker
 * @brief The node linking two DlgCircle's.
 */

#include <math.h>
#include <gtk/gtk.h>
#include "dlg_arrow.h"
#include "gui_dlgedit.h"

// Create a new link between two nodes
DlgArrow::DlgArrow (DlgNode *start, DlgNode *end)
{
    type_ = LINK;
    
    // add the start and end to this arrow
    prev_.push_back (start);
    next_.push_back (end);
    
    // add the arrow to it's start and end node
    start->addNext (this);
    end->addPrev (this);
    
    // init the Arrow's shape, for faster drawing
    initShape ();
}

// prepare the Arrow's shape
void DlgArrow::initShape ()
{
    // start- and end-point of arrow
    DlgPoint start = prev_.front ()->center ();
    DlgPoint end = next_.front ()->center ();
    
    // gradient of line
    int x = start.x () - end.x ();
    int y = start.y () - end.y ();

    // scaled to 1
    float s = sqrt (x*x + y*y);
    float a = x / s;
    float b = y / s;
    
    DlgPoint line0 =  start.offset ((int)(-10.0f * a), (int)(-10.0f * b));
    DlgPoint line1 = end.offset ((int)(10.0f * a), (int)(10.0f * b));

    // line from start- to end-circle
    line[0] = (GdkPoint) line0;
    line[1] = (GdkPoint) line1;

    // arrow's tip
    tip[0] = line[1];
    tip[1] = (GdkPoint) line1.offset ((int)(a * 10.0f + b * 5.0f), (int)(b * 10.0f - a * 5.0f));
    tip[2] = (GdkPoint) line1.offset ((int)(a * 10.0f - b * 5.0f), (int)(b * 10.0f + a * 5.0f));
    
    // calculate arrow's new dimension
    init (line0, line1);
    grow (2, 2);
}

// draw the Arrow
void DlgArrow::draw (GdkPixmap *surface, DlgPoint &point)
{
    GdkPoint l[2];
    GdkPoint t[3];
    GdkGC *gc = GuiDlgedit::window->getColor (mode_, type_);
    DlgRect area = inflate (10, 10);
    
    // Current position
    area.move (point);
    l[0] = (GdkPoint) DlgPoint (line[0]).offset (point);
    l[1] = (GdkPoint) DlgPoint (line[1]).offset (point);
    t[0] = (GdkPoint) DlgPoint (tip[0]).offset (point);
    t[1] = (GdkPoint) DlgPoint (tip[1]).offset (point);
    t[2] = (GdkPoint) DlgPoint (tip[2]).offset (point);

    // draw everything
    gdk_draw_polygon (surface, gc, FALSE, l, 2);
    gdk_draw_polygon (surface, GuiDlgedit::window->getColor (GC_WHITE), TRUE, t, 3);
    gdk_draw_polygon (surface, gc, FALSE, t, 3);

    // update drawing area
    GuiDlgedit::window->graph ()->update (area);
}
