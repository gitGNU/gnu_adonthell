/*
   $Id$

   Copyright (C) 2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
#include <iostream>
#include "dlg_arrow.h"
#include "dlg_module.h"
#include "gui_resources.h"

// Create a new link between two nodes
DlgArrow::DlgArrow (DlgNode *start, DlgNode *end)
{
    type_ = LINK;
    mode_ = IDLE;
    nid_ = 0;
    
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
        circle->removeNext (this);
    }
    
    // remove arrow from following circle
    if (next_.size () != 0)
    {
        circle = next_.front ();
        circle->removePrev (this);    
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
    
    DlgPoint line0 = getIntersection (start, end, *prev_.front ());
    DlgPoint line1 = getIntersection (end, start, *next_.front ());
    
    // line from start- to end-circle
    line[0] = (GdkPoint) line0;
    line[1] = (GdkPoint) line1;

    // arrow's tip
    tip[0] = line[1];
    tip[1] = (GdkPoint) line1.offset ((int)(a * 8.0f + b * 4.0f), (int)(b * 8.0f - a * 4.0f));
    tip[2] = (GdkPoint) line1.offset ((int)(a * 8.0f - b * 4.0f), (int)(b * 8.0f + a * 4.0f));
    
    // calculate arrow's new dimension
    init (line0, line1);
    grow (2, 2);
}

// calculate intersection of arrow and node shape
DlgPoint DlgArrow::getIntersection (DlgPoint &start, DlgPoint &end, DlgRect &shape)
{
    DlgPoint tl = shape.topLeft ();
    DlgPoint br = shape.bottomRight ();
    
    int direction;
    double a, b;
    DlgPoint p;    
    
    // gradient of line
    int x = end.x () - start.x ();
    int y = end.y () - start.y ();

    // tangens of angle between line(start, end) and x-axis
    double m = x == 0 ? 1.0 : double (y) / x;
    
    // direction where line(start, end) intersects with border of start
    enum { NORTH, EAST, SOUTH, WEST };

    // calculate where line(start, end) intersects with border of start
    if (y < 0)
    {
        a = tl.y () - start.y ();
        
        if (x > 0)
        {
            // 1. quadrant
            b = br.x () - start.x ();
            
            if (m > a / b) direction = EAST;
            else direction = NORTH;
        }
        else 
        {
            // 2. quadrant
            b = tl.x () - start.x ();
            
            if (m >= a / b) direction = NORTH;
            else direction = WEST;
        }
    }
    else
    {
        a = br.y () - start.y ();
        
        if (x < 0)
        {
            // 3. quadrant
            b = tl.x () - start.x ();

            if (m > a / b) direction = WEST;
            else direction = SOUTH;
        }
        else
        {
            // 4. quadrant
            b = br.x () - start.x ();

            if (m >= a / b) direction = SOUTH;
            else direction = EAST;
        }
    }
    
    // now we have one coordinate of the intersection and can
    // calculate the second
    switch (direction)
    {
        case NORTH:
        {
            p = DlgPoint ((tl.y () - start.y ()) * x / y + start.x (), tl.y ());
            break;
        }
        case EAST:
        {
            p = DlgPoint (br.x (), start.y () + y / x * (br.x () - start.x ()));
            break;
        }
        case SOUTH: 
        {
            p = DlgPoint ((br.y () - start.y ()) * x / y + start.x (), br.y ());
            break;
        }
        case WEST:
        {
            p = DlgPoint (tl.x (), start.y () + y / x * (tl.x () - start.x ()));
            break;
        }
    }
    
    return p;
}

// draw the Arrow
void DlgArrow::draw (GdkPixmap *surface, DlgPoint &point, GtkWidget *widget)
{
    GdkPoint l[2];
    GdkPoint t[3];
    GdkGC *gc = GuiResources::getColor (mode_, type_);
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
    gdk_draw_polygon (surface, GuiResources::getColor (GC_WHITE), TRUE, t, 3);
    gdk_draw_polygon (surface, gc, FALSE, t, 3);

    // update drawing area
    update (widget, area);
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
bool DlgArrow::load (DlgNode *m)
{
    DlgModule *module = (DlgModule *) m;
    DlgModule *owner = module;
    DlgModule *toplevel = module->toplevel ();
    DlgNode *circle;
    std::string str;
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

                // finish concatenation
                prev_.front ()->addNext (this);
                next_.front ()->addPrev (this);

                return true;
            }

            // Module node belongs to
            case LOAD_MODULE:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    // get the module the node belongs to
                    owner = toplevel->getModule (n);
                    
                    // failed
                    if (!owner || owner->type () != MODULE)
                    {
                        std::cout << "DlgArrow::load: failed to getModule (" 
                             << n << ")!" << std::endl;
                        
                        return false;
                    }
                }
                 
                break;
            }
            
            // Node prior to arrow
            case LOAD_PREV:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    // get the id of the previous circle
                    circle = owner->getNode (n);
                        
                    // failed
                    if (circle == NULL) return false;
                }
                    
                // everything okay, so add circle
                prev_.push_back (circle);
                
                // restore owner
                owner = module;
                
                break;
            }

            // Node following arrow
            case LOAD_NEXT:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    // get the id of the previous circle
                    circle = owner->getNode (n);
                        
                    // failed
                    if (circle == NULL) return false;
                }
                    
                // everything okay, so add circle
                next_.push_back (circle);

                // restore owner
                owner = module;
                
                break;
            }

            // Nodes linked to the arrow (obsolete -> convert to normal arrow)
            case LOAD_LINK:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM)
                {
                    circle = module->getNode (n);
                    if (circle == NULL) break;
                    
                    DlgArrow *arrow = new DlgArrow (circle, next_.front ());
                    module->addNode (arrow);
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
void DlgArrow::save (std::ofstream &file)
{
    // Keyword "Arrow" and arrow's number
    file << "\nArrow\n";

    // module of start circle, if != 0
    if (prev_.front ()->module_id () != 0)
        file << "  Module " << prev_.front ()->module_id () << "\n";
    
    // start circle
    file << "  Prev " << prev_.front ()->node_id () << "\n";

    // module of end circle, if != 0
    if (next_.front ()->module_id () != 0)
        file << "  Module " << next_.front ()->module_id () << "\n";

    // end circle
    file << "  Next " << next_.front ()->node_id () << "\n";

    // end of node
    file << "End\n";
}
