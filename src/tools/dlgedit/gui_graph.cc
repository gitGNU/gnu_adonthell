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
 * @file gui_graph.cc
 *
 * @author Kai Sterker
 * @brief View for the dialogue graph
 */

#include <gtk/gtk.h>
#include "gui_dlgedit.h"
#include "gui_graph_events.h"

// Constructor
GuiGraph::GuiGraph (GtkWidget *paned)
{
    // no module attached yet
    module = NULL;
    offset = NULL;
    surface = NULL;
    
    // create drawing area for the graph
    graph = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (graph), 200, 450);
    gtk_paned_add1 (GTK_PANED (paned), graph);
    gtk_widget_show (graph);
    gtk_widget_grab_focus (graph);
    
    // register our event callbacks
    gtk_signal_connect (GTK_OBJECT (graph), "expose_event", (GtkSignalFunc) expose_event, this);
    gtk_signal_connect (GTK_OBJECT (graph), "configure_event", (GtkSignalFunc) configure_event, this);
    gtk_signal_connect (GTK_OBJECT (graph), "button_press_event", (GtkSignalFunc) button_press_event, this);
    gtk_signal_connect (GTK_OBJECT (graph), "button_release_event", (GtkSignalFunc) button_release_event, this);
    gtk_signal_connect (GTK_OBJECT (graph), "motion_notify_event", (GtkSignalFunc) motion_notify_event, this);
    gtk_signal_connect (GTK_OBJECT (GuiDlgedit::window->getWindow ()), "key_press_event", (GtkSignalFunc) key_press_notify_event, this);

    gtk_widget_set_events (graph, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_KEY_PRESS_MASK);
}

// attach a module
void GuiGraph::attachModule (DlgModule *m)
{
    module = m;
    
    // get reference of the module's offset
    offset = &m->offset ();
    
    // set the size of the dialogue
    module->resize (graph->allocation.width, graph->allocation.height);
    
    // display the module
    draw ();
}

// detach a module
void GuiGraph::detachModule ()
{
    module = NULL;    
}

// select a node
bool GuiGraph::selectNode (DlgNode *node)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;
    
    // a node has been selected
    if (module->selectNode (node))
    {
        // update the program state
        GuiDlgedit::window->setMode (NODE_SELECTED);
        
        // update the instant preview
        GuiDlgedit::window->list ()->display (node);
        
        // redraw the node
        node->draw (surface, *offset);

        return true;
    }
    
    return false;
}

// select parent
bool GuiGraph::selectParent ()
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // see if a node is currently selected
    DlgNode *selected = module->selected ();

    // if so ...
    if (selected)
    {
        // ... try to retrieve it's parent
        DlgNode *parent = ((DlgCircle *) selected)->parent (FIRST);

        // if we have it, then select it
        if (parent)
        {
            deselectNode ();
            return selectNode (parent);
        }
    }

    // if no node is selected, we simply select the first one
    else if (module->selectRoot ())
    {
        // update the instant preview
        GuiDlgedit::window->list ()->display (module->selected ());

        return true;
    }

    return false;
}

// select the child of a node
bool GuiGraph::selectChild ()
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // see if a node is currently selected
    DlgNode *selected = module->selected ();

    // if so ...
    if (selected)
    {
        // ... try to retrieve it's child
        DlgNode *child = ((DlgCircle *) selected)->child (FIRST);

        // if we have it, then select it
        if (child)
        {
            deselectNode ();
            return selectNode (child);
        }
    }

    // if no node is selected, we simply select the first one
    else if (module->selectRoot ())
    {
        // update the instant preview
        GuiDlgedit::window->list ()->display (module->selected ());

        return true;
    }

    return false;
}

bool GuiGraph::selectSibling (query_type pos)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // see if a node is currently selected
    DlgNode *selected = module->selected ();

    // if so ...
    if (selected)
    {
        // ... try to retrieve it's child
        DlgNode *sibling = ((DlgCircle *) selected)->sibling (pos);

        // if there is none in that direction, try to wrap around
        if (!sibling)
        {      
            if (pos == PREV) sibling = ((DlgCircle *) selected)->sibling (LAST);
            else sibling = ((DlgCircle *) selected)->sibling (FIRST);
        }
        
        // if we have something now
        if (sibling && sibling != selected)
        {
            deselectNode ();
            return selectNode (sibling);
        }
    }

    // if no node is selected, we simply select the first one
    else if (module->selectRoot ())
    {
        // update the instant preview
        GuiDlgedit::window->list ()->display (module->selected ());

        return true;
    }

    return false;
}

// deselect a node
void GuiGraph::deselectNode ()
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return;
    
    DlgNode *deselected = module->deselectNode ();
    
    if (deselected)
    {
        // update the program state
        GuiDlgedit::window->setMode (NONE);
        
        // update the instant preview
        GuiDlgedit::window->list ()->clear ();
        
        // redraw the node
        deselected->draw (surface, *offset);
    }
    
    return;
}

// center the view on given node
bool GuiGraph::centerNode (DlgNode *node)
{
    if (module == NULL) return false;

    if (node == NULL && module->selected () != NULL)
        node = module->selected ();
    else return false;

    // calculate the correct offset for the given node
    DlgPoint pos = node->center ().offset (*offset);
    int x, y; 
    
    x = module->width () / 5;
    y = module->height () / 5;

    // is node outside the views inner 60% ?
    if (!module->inflate (-x, -y).contains (pos))
    {
        // then move the view so it is centered on the given point
        DlgPoint o (-(pos.x()-module->width()/2), -(pos.y()-module->height()/2));
        offset->move (o);

        draw ();
        return true;
    }
            
    return false;
}

// resize the drawing area
void GuiGraph::resizeSurface (GtkWidget *widget)
{
    // delete the old surface
    if (surface) gdk_pixmap_unref (surface);
    
    // create a new one with the proper size
    surface = gdk_pixmap_new (widget->window, widget->allocation.width,
        widget->allocation.height, -1);

    // init the surface
    if (GuiDlgedit::window->getColor (GC_GREY)) clear ();
            
    // set the size of the attached dialogue
    if (module) module->resize (widget->allocation.width,
        widget->allocation.height);
}

// empty the drawing area
void GuiGraph::clear ()
{
    GdkRectangle t;

    gdk_draw_rectangle (surface, GuiDlgedit::window->getColor (GC_GREY), 
        TRUE, 0, 0, graph->allocation.width, graph->allocation.height);

    t.x = 0;
    t.y = 0;
    t.width = graph->allocation.width;
    t.height = graph->allocation.height;

    gtk_widget_draw (graph, &t);        
}

// update the graph widget
void GuiGraph::update (DlgRect &area)
{
    GdkRectangle rect = (GdkRectangle) area;
    gtk_widget_draw (graph, &rect);
}

// draw the graph to the surface
void GuiGraph::draw ()
{
    // nothing to draw
    if (module == NULL) return;
            
    GdkRectangle t;
    vector<DlgNode*>::reverse_iterator i;
    vector<DlgNode*> nodes = module->getNodes ();
    
    // get visible part of graph
    t.x = -offset->x ();
    t.y = -offset->y ();
    t.width = module->width ();
    t.height = module->height ();

    DlgRect rect (t);

    // Clear graph
    gdk_draw_rectangle (surface, GuiDlgedit::window->getColor (GC_WHITE), TRUE, 0, 0, t.width, t.height);

    // normalize rect
    t.x = 0;
    t.y = 0;

    // draw backing image to screen
    gtk_widget_draw (graph, &t);

    // check for each node, wether it is visible
    for (i = nodes.rbegin (); i != nodes.rend (); i++)
        // draw nodes and arrows
        if ((*i)->contains (rect))
            (*i)->draw (surface, *offset);

    // Mark object below cursor if neccessary
    // if (wnd->mode != OBJECT_DRAGGED)
    // {
    //     wnd->below_pointer = NULL;
    //     mouse_over (wnd, point);
    // }
}
