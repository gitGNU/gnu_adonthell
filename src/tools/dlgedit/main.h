/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtkstyle.h>
#include <string>
#include <vector>
#include "dlgnode.h"

// Internal Editor Modes
enum
{
    OBJECT_MARKED = 0,
    OBJECT_DRAGGED = 1,
    IDLE = 2
};

// Colors
enum
{
    GC_RED = 0,
    GC_DARK_RED = 1,
    GC_DARK_BLUE = 2,
    GC_GREEN = 3,
    GC_PURPLE = 4,
    MAX_GC = 5
};

// Dialogue Editor Data
typedef struct
{
    GtkWidget *wnd;             // Main Window
    GtkWidget *menu;            // Menubar
    GtkWidget *graph;           // Drawing Area
    GtkWidget *list;            // Listbox
    GtkWidget *text_dlg;
    GtkWidget *dialogue_run;    // The Dialogue_Run Menuitem
    GtkWidget *tooltip;         // The tooltip
    GtkWidget *project;

    GdkPixmap *pixmap;          // Drawing surface
    GdkFont *font;              // Font for Textoutput
    GdkGC *color[MAX_GC];       // Custom Pens

    vector<DlgNode*> nodes;     // Storage of all nodes

    DlgNode *selected_node;     // Currently selected node
    DlgNode *below_pointer;     // Node under Mouse-pointer
    DlgNode *dragged_node;      // Node being dragged

    gchar *file_name;           // Name of the dialogue currently opened

    u_int8 mode;                // Program Mode
    u_int32 number;             // Number of Nodes
    s_int32 x_offset;           // Scrolling
    s_int32 y_offset;           // Dito

    string pset_vars;           // Preset variables for running the Dialogue
}
MainFrame;

/* Functions */
void init_app (MainFrame *);
void delete_dialogue (MainFrame *);

#endif // __MAIN_H__