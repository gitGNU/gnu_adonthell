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
    mode_ = IDLE;
    
    // add the start and end to this arrow
    prev_.push_back (start);
    next_.push_back (end);
    
    // add the arrow to it's start and end node
    start->addNext (this);
    end->addPrev (this);
    
    // init the Arrow's shape, for faster drawing
    initShape ();
}

// Remove arrow from it's start and end node
DlgArrow::~DlgArrow ()
{
    DlgNode *circle;
    
    // remove arrow from previous circle
    if (prev_.size () != 0)
    {
        circle = prev_.front ();
        circle->removePrev (this);
    }
    
    // remove arrow from following circle
    if (next_.size () != 0)
    {
        circle = next_.front ();
        circle->removeNext (this);    
    }
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
    
bool DlgArrow::operator== (DlgPoint &point)
{
    // is the point anywhere near the arrow?
    if (contains (point))
    {
        int x, dx, dy;

        // straight g: ax + by + c = 0 through 2 points p(x, y) und q(x, y): 
        // with a = (p.y - q.y), b = (q.x - p.x), c = (-(p.x * a + p.y * b)) 

        // insert point in above function; values |x| < 500 are close to the arrow 
        dx = line[1].x - line[0].x;
        dy = line[0].y - line[1].y;

        x = (dy * point.x () + dx * point.y ()) - (line[0].x * dy + line[0].y * dx);
        if (x < 500 && x > -500) return true;
    }
    return false;  
}

// load an arrow
bool DlgArrow::load (vector<DlgNode*> &nodes)
{
    DlgNode *circle;
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
            case LOAD_END:
            {
                // claculate the Arrow's shape
                initShape ();

                return true;
            }

            // Type of node
            case LOAD_TYPE:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) type_ = (node_type) n;
                break;
            }

            // Node prior to arrow
            case LOAD_PREV:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    circle = nodes[n];
                    prev_.push_back (circle);
                    circle->addNext (this);
                }
                break;
            }

            // Node following arrow
            case LOAD_NEXT:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    circle = nodes[n];
                    next_.push_back (circle);
                    circle->addPrev (this);
                }
                break;
            }

            // Nodes linked to the arrow (obsolete -> convert to normal arrow)
            case LOAD_LINK:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    circle = nodes[n];
                    DlgArrow *arrow = new DlgArrow (circle, next_.front ());
                    nodes.push_back (arrow);
                }
                break;
            }

            // Just ignore everything else
            default: break;
        }
    }

    return false;
}

// save an arrow to file
void DlgArrow::save (ofstream &file)
{
    // Keyword "Arrow" and arrow's number
    file << "\nArrow\n";

    // arrow's type
    file << "  Type " << (int) type_ << "\n";

    // start circle
    file << "  Prev " << prev_.front ()->index () << "\n";

    // end circle
    file << "  Next " << next_.front ()->index () << "\n";

    file << "End\n";
}
