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
#include "gui_tooltip.h"
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
     * Highlight nodes when hit by the cursor.
     * @param point Current position of the cursor. 
     */
    void mouseMoved (DlgPoint &point);
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
     * Create a new DlgCircle at the given position.
     * @param pos Current cursor position
     * @param type Type of the new circle.
     * @return <b>true</b> if the circle could be created,
     *         <b>false</b> otherwise.
     */
    bool newCircle (DlgPoint &pos, node_type type = NPC);
    /**
     * Create a new DlgArrow between the selected node and the given position.
     * @param pos Current cursor position
     * @return <b>true</b> if the link could be created, <b>false</b> otherwise.
     */
    bool newArrow (DlgPoint &pos);
    /**
     * Edit currently selected node.
     * @return <b>true</b> if editing was successful, <b>false</b> otherwise.
     */
    bool editNode ();
    /**
     * Select the given node and update the instant preview and node editor
     * accordingly.
     * @param node the DlgNode to be selected
     * @return <b>true</b> if the node exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectNode (DlgNode *node);
    /**
     * Select the node at the given position.
     * @param point the coordinates of the DlgNode to be selected
     * @return <b>true</b> if the node exists and was selected successfully,
     *         <b>false</b> otherwise.
     */
    bool selectNode (DlgPoint &point);
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
     * Select the first node in the dialogue.
     * @return <b>true</b> on success, <b>false</b> otherwise.
     */
    bool selectRoot ();
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

    /**
     * @name Drag'n dropping of DlgNodes
     */
    //@{
    bool prepareDragging (DlgPoint &point);
    void drag (DlgPoint &point);
    void stopDragging (DlgPoint &point);
    //@}
    
    /**
     * @name Auto-Scrolling (TM) ;) functionality.
     */
    //@{
    /**
     * Method deciding whether to start with scrolling. The cursor has to
     * remain in the outer 20 pixel of the view to enable scrolling.
     * @param point The current cursor position.
     */
    void prepareScrolling (DlgPoint &point);
    /**
     * Moves the view in the desired direction.
     */
    void scroll ();
    /**
     * Check whether we are currently scrolling
     * @return <b>true</b> if that is the case, <b>false</b> otherwise.
     */
    bool isScrolling ()         { return scrolling; }
    /**
     * Finish scrolling.
     */
    void stopScrolling ()       { scrolling = false; }
    //@}
    
    /**
     * @name Member access methods.
     */
    //@{
    /** 
     * Returns the current offset of the attached DlgModule.
     * @return the absolute position of the view
     */
    DlgPoint *position ()       { return offset; }
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
    /**
     * Get the state of the attached module.
     * @return the attached DlgModule's state. 
     */
    mode_type mode ()           { return module ? module->mode () : IDLE; }
    //@}
    
private:
    DlgNode *mover;         // The node currently dragged
    DlgModule *module;      // Module assigned to the graph view
    DlgPoint *offset;       // Module's relative position to the origin
    GtkWidget *graph;       // Drawing Area
    GdkPixmap *surface;     // Drawing surface
    GuiTooltip *tooltip;    // Tooltip for displaying node-text
    bool scrolling;         // Indicates whether autoscrolling is active
    DlgPoint scroll_offset; // Offset by which the view moves during scrolling
    bool updateBackground;  // Don't show updates to the drawing area 
};

#endif // GUI_GRAPH_H
