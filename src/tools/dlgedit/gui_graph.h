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
 * @file gui_graph.h
 *
 * @author Kai Sterker
 * @brief View for the dialogue graph
 */

#ifndef GUI_GRAPH_H
#define GUI_GRAPH_H

#include <gdk/gdktypes.h>
#include <gtk/gtkstyle.h>
#include "dlg_module.h"

/**
 * The widget handling the graphical representation of the dialogue graph.
 * Displays a dialogue module and offers an interface for authors to edit
 * the dialogue. Only one instance of this class exists throughout the whole
 * dlgedit session.
 */
class GuiGraph
{
public:
    /** 
     * Constructor. Main purpose is to create the drawing area for the
     * graph and to register the GTK+ event callbacks used to catch
     * user input for this widget.
     * @param paned The GtkPaned that will hold the graph view widget.
     */
    GuiGraph (GtkWidget* paned);
    ~GuiGraph ();
    
    /**
     * Attach a dialogue module to the view for drawing.
     * @param m the DlgModule to display and edit.
     */ 
    void attachModule (DlgModule *m);
    /**
     * Detach the dialogue module attached to the view.
     */ 
    void detachModule ();
    
    /**
     * @name Drawing area handling methods.
     */
    //@{
    /**
     * Blit a certain area of the graph widget to the screen
     * @param the rectangular area to update.
     */
    void update (DlgRect &area);
    /**
     * Draw everything to the backing pixmap.
     */
    void draw ();
    /**
     * Resize the drawing area (after the user resized the window).
     * @param widget The widget containing the drawing area.
     */
    void resizeSurface (GtkWidget *widget);
    /**
     * Clear the drawing area.
     */
    void clear ();
    //@}
    
    /**
     * @name Node manipulation methods.
     */
    //@{
    /**
     * Select the given node and update the instant preview and node editor
     * accordingly.
     * @param node the DlgNode to be selected
     * @return <b>true</b> if the node exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectNode (DlgNode *node);
    /**
     * Select the parent of the currently selected node (or the first node
     * in case no node is selected so far.
     * @return <b>true</b> if the node exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectParent ();
    /**
     * Select the first child of the currently selected node (or the first 
     * node in case no node is selected so far.
     * @return <b>true</b> if the node exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectChild ();
    /**
     * Select a sibling of the currently selected node (or the first 
     * node in case no node is selected so far.
     * @param pos Specifies whether to select the previous or the next sibling.
     * @return <b>true</b> if the sibling exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectSibling (query_type pos);
    /**
     * Deselect the node currently selected.
     */
    void deselectNode ();
    /**
     * Center the view on the given node, as long as it does not lie in the 
     * inner 60\% of the visible area.
     * @param node DlgNode to center the view on.
     * @return <b>true</b> if the node exists and was centered on,
     *         <b>false</b> otherwise.
     */
    bool centerNode (DlgNode *node = NULL);
    //@}

    void mouseMoved (DlgPoint &point);
        
    /**
     * @name Member access methods.
     */
    //@{
    /** 
     * Return a pointer to the drawing surface.
     * @return the drawing surface
     */
    GdkPixmap *pixmap ()        { return surface; }
    /**
     * Return a pointer to the drawing area
     * @return the GtkDrawingArea widget
     */
    GtkWidget *drawingArea ()   { return graph; }
    /**
     * Return the attached dialogue module.
     * @return the DlgModule currently attached to the view 
     */
    DlgModule *dialogue ()      { return module; }
    //@}
    
private:
    DlgModule *module;      // Module assigned to the graph view
    DlgPoint *offset;       // Module's relative position to the origin
    GtkWidget *graph;       // Drawing Area
    GdkPixmap *surface;     // Drawing surface
};

#endif // GUI_GRAPH_H
