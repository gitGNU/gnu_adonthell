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
#include "dlg_mover.h"
#include "gui_dlgedit.h"
#include "gui_tooltip.h"
#include "gui_circle.h"
#include "gui_graph_events.h"

// Constructor
GuiGraph::GuiGraph (GtkWidget *paned)
{
    // initialize members to sane values
    mover = NULL;
    module = NULL;
    offset = NULL;
    surface = NULL;
    scrolling = false;
    updateBackground = true;
    
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
void GuiGraph::attachModule (DlgModule *m, bool cntr)
{
    module = m;
    
    // get reference of the module's offset
    offset = &m->offset ();
    
    // center the module in view
    if (cntr) center ();    

    // if a node is selected, update the instant preview
    GuiDlgedit::window->list ()->display (module->selected ());
        
    // update the program state
    GuiDlgedit::window->setMode (module->mode ());

    // set the size of the dialogue
    module->resize (graph->allocation.width, graph->allocation.height);
    
    // display the module
    draw ();
}

// detach a module
void GuiGraph::detachModule ()
{
    module = NULL;
    
    // clear the instant preview
    GuiDlgedit::window->list ()->clear ();

    // update the program state
    GuiDlgedit::window->setMode (IDLE);

    // remove the tooltip if it is open
    if (tooltip)
    {
        delete tooltip;
        tooltip = NULL;
    }
}

// create a new circle
bool GuiGraph::newCircle (DlgPoint &point, node_type type)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;
    
    // create the new node ...
    DlgCircle *circle = new DlgCircle (point, type);
    
    // ... add it to the module ...
    module->addNode (circle);
    
    // ... and select it for editing
    module->selectNode (circle);
    
    // see whether creation was cancelled
    if (!editNode ())
    {
        // cleanup
        module->deleteNode ();

        return false;
    }
    
    return true;
}

// create a new arrow
bool GuiGraph::newArrow (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());

    // get start and end of the circle
    DlgNode *start = module->selected ();
    DlgNode *end = module->getNode (point);

    // sanity checks
    if (!start || start->type () == LINK) return false;
    if (end && end->type () == LINK) return false;
    if (start == end || ((DlgCircle *) start)->hasChild (end)) return false; 
    
    // if no end selected, create a new circle first
    if (end == NULL)
    {
        // chose a sensible type for the new circle
        node_type type = (start->type () == NPC ? PLAYER : NPC);
        
        // there musn't be a node selected to create a new circle
        module->deselectNode ();
        
        // try to create a new circle
        newCircle (point, type);
        
        // chose the newly created circle as end of the arrow
        end = module->getNode (point);
        
        // restore selection
        deselectNode ();
        selectNode (start);
        
        // do we have a valid end now?
        if (end == NULL) return false;
    }

    // no connection between start and end if both are PLAYER nodes   
    if (start->type () == PLAYER && end->type () == PLAYER) return false;

    // now create the arrow between start and end, ...
    DlgArrow *arrow = new DlgArrow (start, end);

    // ... add it to the module ...
    module->addNode (arrow);
    
    // ... and update everything
    GuiDlgedit::window->list ()->display (start);
	arrow->draw (surface, *offset);
    module->setChanged ();
    
    return true;
}

// delete the selected node
bool GuiGraph::deleteNode ()
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // try to delete node
    if (module->deleteNode ())
    {
        // update the program state
        GuiDlgedit::window->setMode (IDLE);
        
        // update the instant preview
        GuiDlgedit::window->list ()->clear ();

        // remove tooltip if it is open        
        if (tooltip) 
        {
            delete tooltip;
            tooltip = NULL;
        }

        // redraw the dialogue
        draw ();
        
        return true;
    }
    
    return false;    
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

// select the node at the given position
bool GuiGraph::selectNode (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());
    
    // see if we're over a node
    DlgNode *node = module->getNode (point);
    
    // no node at that position
    if (node == NULL) return false;
    else return selectNode (node);
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
        // see if selected is arrow
        if (selected->type () == LINK)
            selected = selected->next (FIRST);
        
        // ... try to retrieve it's parent
        DlgNode *parent = ((DlgCircle *) selected)->parent (CURRENT);

        // if we have it, then select it
        if (parent)
        {
            deselectNode ();
            return selectNode (parent);
        }
    }

    // if no node is selected, we simply select the first one
    else return selectRoot ();
    
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
        // see if selected is arrow
        if (selected->type () == LINK)
            selected = selected->prev (FIRST);
        
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
    else return selectRoot ();
    
    return false;
}

// select a sibling of the currently selected node
bool GuiGraph::selectSibling (query_type pos)
{
    // if there is no module assigned to the view, there is nothing to select
    if (module == NULL) return false;

    // see if a node is currently selected
    DlgNode *selected = module->selected ();

    // if so ...
    if (selected)
    {
        // see if selected is arrow
        if (selected->type () == LINK)
            selected = selected->next (FIRST);
        
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
    else return selectRoot ();
    
    return false;
}

// select the first node in the dialogue
bool GuiGraph::selectRoot ()
{
    if (module->selectRoot ())
    {
        // update the instant preview
        GuiDlgedit::window->list ()->display (module->selected ());

        // update the program state
        GuiDlgedit::window->setMode (module->mode ());

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
        GuiDlgedit::window->setMode (IDLE);
        
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

// center whole dialogue in view
void GuiGraph::center ()
{
    if (module == NULL) return;
    
    int min_x, max_x, y;
    
    module->extension (min_x, max_x, y);
    
    int x_off = (graph->allocation.width - (max_x - min_x))/2 - min_x;
    int y_off = -y + 20;
    
    offset->move (x_off, y_off);
}

// edit selected node
bool GuiGraph::editNode ()
{
    if (module == NULL) return false;
    
    // see if a node is currently selected
    DlgCircle *selected = (DlgCircle *) module->selected ();

    // if so ...
    if (selected && selected->type () != LINK)
    {
        // disable scrolling (just in case)
        scrolling = false;
        
        GuiCircle edit (&selected->type (), selected->entry (), module->entry ());

	    // Editing aborted?
	    if (!edit.run ()) return false;
	
	    // otherwise update everything
        GuiDlgedit::window->list ()->display (selected);
	    selected->draw (surface, *offset);
        module->setChanged ();
        
        return true;
    }
    
    return false;
}

// set everything up for moving nodes around
bool GuiGraph::prepareDragging (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to do
    if (module == NULL) return false;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());
    
    // see if we're over a node
    DlgNode *node = module->getNode (point);

    // Not over a node
    if (node == NULL) return false;

    // if no node selected, select node for dragging
    if (module->selected () == NULL) selectNode (node);

    // else check whether dragged and selected node are the same
    else if (node != module->selected ()) return false;

    // Is dragged node circle or arrow?
    if (node->type () != LINK)
    {
        // circles can be dragged directly
        mover = (DlgCircle *) node;
        
        // remove any tooltip, as it only gets in the way
        if (tooltip)
        {
            delete tooltip;
            tooltip = NULL;
        }
    }
    else
    {
        // arrows have to be attached to a (invisible) mover 
        mover = new DlgMover (point);
        
        // try to attach arrow to mover
        if (!((DlgMover *) mover)->attach ((DlgArrow *) node)) 
        {
            // moving of arrow failed, so clean up
            delete mover;
            mover = NULL;
        }
    }
    
    // if we have a mover, update program state
    if (mover != NULL)
    {
        GuiDlgedit::window->setMode (NODE_DRAGGED);
        module->setMode (NODE_DRAGGED);
        
        return true;
    }
    
    return false;    
}

// drag a node around
void GuiGraph::drag (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to do
    if (module == NULL) return;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());
    
    // move node
    mover->setPos (point);
        
    // update arrows
    for (DlgNode *a = mover->prev (FIRST); a != NULL; a = mover->prev (NEXT))
        ((DlgArrow *) a)->initShape ();

    for (DlgNode *a = mover->next (FIRST); a != NULL; a = mover->next (NEXT))
        ((DlgArrow *) a)->initShape ();
    
    // update view
    draw ();
}

// stop dragging node
void GuiGraph::stopDragging (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to do
    if (module == NULL || mover == NULL) return;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());
    
    // see whether arrow was dragged
    if (mover->type () == MOVER)
    {
        // see whether we are over a node
        DlgNode *node = module->getNode (point);
        
        // drop the mover onto the node
        ((DlgMover *) mover)->drop (node);
        
        // cleanup
        delete mover;
    }
    
    // if circle moved, realign it to the grid
    else 
    {
        mover->setPos ( 
            DlgPoint (point.x () - (point.x () % CIRCLE_DIAMETER), 
                      point.y () - (point.y () % CIRCLE_DIAMETER)));
    
        // also need to update arrows and reorder children and parents 
        for (DlgNode *a = mover->prev (FIRST); a != NULL; a = mover->prev (NEXT))
        {
            a->prev (FIRST)->removeNext (a);
            a->prev (FIRST)->addNext (a);
            ((DlgArrow *) a)->initShape ();
        }
        
        for (DlgNode *a = mover->next (FIRST); a != NULL; a = mover->next (NEXT))
        {
            a->next (FIRST)->removePrev (a);
            a->next (FIRST)->addPrev (a);
            ((DlgArrow *) a)->initShape ();
        }
    }
    
    // clear mover
    mover = NULL;

    // update everything    
    GuiDlgedit::window->list ()->display (module->selected ());
    GuiDlgedit::window->setMode (NODE_SELECTED);
    module->setMode (NODE_SELECTED);
    module->setChanged ();
    draw ();
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
    if (updateBackground == false) return;
    
    GdkRectangle rect = (GdkRectangle) area;
    gtk_widget_draw (graph, &rect);
}

// draw the graph to the surface
void GuiGraph::draw ()
{
    // nothing to draw
    if (module == NULL) return;
            
    GdkRectangle t;
    std::vector<DlgNode*>::reverse_iterator i;
    std::vector<DlgNode*> nodes = module->getNodes ();
    
    // get visible part of graph
    t.x = -offset->x ();
    t.y = -offset->y ();
    t.width = module->width ();
    t.height = module->height ();

    DlgRect rect (t);

    // prevent changes to the drawing area to propagate to the screen
    updateBackground = false;
    
    // Clear graph
    gdk_draw_rectangle (surface, GuiDlgedit::window->getColor (GC_WHITE), TRUE, 0, 0, t.width, t.height);

    // normalize rect
    t.x = 0;
    t.y = 0;

    // check for each node, wether it is visible
    for (i = nodes.rbegin (); i != nodes.rend (); i++)
        // draw nodes and arrows
        if ((*i)->contains (rect))
            (*i)->draw (surface, *offset);

    // allow that again
    updateBackground = true;

    // draw backing image to screen
    gtk_widget_draw (graph, &t);
}

// the mouse has been moved
void GuiGraph::mouseMoved (DlgPoint &point)
{
    // if there is no module assigned to the view, there is nothing to do
    if (module == NULL) return;

    // calculate absolute position of the point
    point.move (-offset->x (), -offset->y ());
    
    // see if we're over a node
    DlgNode *node = module->getNode (point);
    
    // get the node that was highlighted before (if any)
    DlgNode *prev = module->highlightNode (node);
    
    // there's no need for action unless old and new highlight differs
    if (prev != node)
    {
        // clear old if necessary
        if (prev != NULL)
        {
            prev->draw (surface, *offset);
            if (tooltip) 
            {
                delete tooltip;
                tooltip = NULL;
            }
        }
        
        // then highlight the new one
        if (node != NULL) 
        {
            node->draw (surface, *offset, NODE_HILIGHTED);
            tooltip = new GuiTooltip (node);
            tooltip->draw (graph, *offset);
        }
    }
    
    return;
}

// prepare everything for 'auto-scrolling' (TM) ;-)
void GuiGraph::prepareScrolling (DlgPoint &point)
{
    // if there is no module assigned to the view or no nodes
    // in the module yet, there is nothing to do
    if (module == NULL || module->getNodes ().empty ()) return;

    int scroll_x = 0;
    int scroll_y = 0;

    // set scrolling offset and direction    
    if (point.x () < 20) scroll_x = 15;
    if (point.y () < 20) scroll_y = 15;
    if (point.x () + 20 > graph->allocation.width) scroll_x = -15;
    if (point.y () + 20 > graph->allocation.height) scroll_y = -15;

    // enable scrolling
    if (scroll_x || scroll_y)
    {
        scroll_offset = DlgPoint (scroll_x, scroll_y);

        if (!scrolling)
        {
            scrolling = true;
            gtk_timeout_add (100, on_scroll_graph, this);
        }
    }
    else scrolling = false;
}

// the actual scrolling
void GuiGraph::scroll ()
{
    offset->move (scroll_offset);
    draw ();
}
