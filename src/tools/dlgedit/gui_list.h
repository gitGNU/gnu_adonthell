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
 * @file gui_list.h
 *
 * @author Kai Sterker
 * @brief Instant dialogue preview
 */

#ifndef GUI_LIST_H
#define GUI_LIST_H

#include <gtk/gtkstyle.h>
#include "dlg_circle.h"

/**
 * This widget uses a GtkList to display an instant preview of the dialogue.
 * It shows the parents and childs of a selected node and allows to navigate
 * through the dialogue by clicking on a line of text.
 */
class GuiList
{
public:
    /** 
     * Constructor. Main purpose is to create the drawing area for the
     * graph and to register the GTK+ event callbacks used to catch
     * user input for this widget.
     * @param paned The GtkPaned that will hold the list widget.
     */
    GuiList (GtkWidget* paned);
    ~GuiList ();
    
    /**
     * "Draw" the current contents of the list.
     */
    void draw ();
    /**
     * Empties the list and redraws it.
     */
    void clear ();
    /**
     * Displays the given Dlgnode in the list, including all of it's parents
     * and children.
     * @param the node currently selected.
     */
    void display (DlgNode *node);
    
private:
    /**
     * Add an item to the list.
     * @param mode Defines whether the circle is selected or not
     * @param circle The circle whose text is to be added to the list
     */
    void add (int mode, DlgCircle *circle);

    GtkWidget *list;        // The actual GTK+ list widget
    GList *items;           // The items in the list
};

#endif // GUI_LIST_H
