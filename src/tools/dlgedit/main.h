/*
   $Id$
   
   Copyright (C) 1999/2000/2001 Kai Sterker <kaisterker@linuxgames.com>
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
#include "../../character.h"

class debug_dlg;
class run_dlg;

// Internal Editor Modes
enum
{
    OBJECT_MARKED = 0,
    OBJECT_DRAGGED = 1,
    MULTI_SELECT = 2,
    MULTI_MARKED = 3,
    IDLE = 4
};

// Colors
enum
{
    GC_RED = 0,
    GC_DARK_RED = 1,
    GC_DARK_BLUE = 2,
    GC_GREEN = 3,
    GC_DARK_GREEN = 4,
    GC_PURPLE = 5,
    GC_YELLOW = 6,
    GC_GREY = 7,
    MAX_GC = 8
};

// Dialogue Editor Data
class MainFrame
{
public:
    GtkWidget *wnd;             // Main Window
    GtkWidget *menu;            // Menubar
    GtkWidget *graph;           // Drawing Area
    GtkWidget *list;            // Listbox
    GtkWidget *text_dlg;
    GtkWidget *dialogue_run;    // The Dialogue_Run Menuitem
    GtkWidget *tooltip;         // The tooltip

    GdkPixmap *pixmap;          // Drawing surface
    GdkFont *font;              // Font for Textoutput
    GdkGC *color[MAX_GC];       // Custom Pens
    GdkRectangle multsel_rect;  // Extension of the multi-selection area
    GdkPoint multsel_start;     // Start of the multi-selection area
    
    vector<DlgNode*> nodes;     // Storage of all nodes
    vector<DlgNode*> multsel;   // Nodes in multi-selection

    DlgNode *selected_node;     // Currently selected node
    DlgNode *below_pointer;     // Node under Mouse-pointer
    DlgNode *dragged_node;      // Node being dragged

    gchar *file_name;           // Name of the dialogue currently opened

    u_int8 mode;                // Program Mode
    u_int32 number;             // Number of Nodes
    s_int32 x_offset;           // Offset scrolled in horiz. dir.
    s_int32 y_offset;           // Offset scrolled in vert. dir.
    s_int32 scroll_x;           // Auto-scroll rate
    s_int32 scroll_y;           // Auto-scroll rate
    u_int8 scroll;              // Auto-scrolling on/off
    u_int8 changed;             // Dialogue un-/changed
    u_int8 debug;               // Debugging enabled/disabled

    debug_dlg *dbg_dlg;         // For displaying debug info
    run_dlg *test_dlg;          // For testing the dialogue
    string pset_vars;           // Preset variables for running the Dialogue
    string cust_func;           // Custom functions of the Dialogue
    character *myplayer;        // A player object
    character *mynpc;           // A NPC object
    
    void set_changed ();        // Update the title bar when dialogue changes
    void set_npc (character_base*);// Set shortcut to one of the npc's
};

/* Functions */
void init_app (MainFrame *);
void delete_dialogue (MainFrame *);

#endif // __MAIN_H__
