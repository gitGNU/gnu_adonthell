/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <gtk/gtkstyle.h>

class MainFrame;
class objects;
class storage;

class dbg_node_data
{
public:
    dbg_node_data (char *a, int t) : attribute (a), touched (t) { }
    char *attribute;
    int touched;
};

class debug_dlg
{
public:
    debug_dlg (MainFrame*);
    ~debug_dlg ();
    
    void init ();               // Init the trees
    void update ();             // Update the active tree
    void set (char*, char*);    // Add/change a variable of the active page
    objects* get_dlg_vars ();   // Return a Dialogue's debug info
    void set_dlg_var (char*, int);// Add/Change a dialogue's local variable
    
    GtkWidget *quest_tree;      // Contains quest variables 
    GtkWidget *char_tree;       // Contains character attributes
    GtkWidget *dlg_tree;        // Contains local dialogue variables
    GtkWidget *val_entry;       // Textentry for attribute value
    GtkWidget *attrib_entry;    // Textentry for attribute name
    GtkWidget *dlg;             // The debug window
    MainFrame *wnd;             // The main applictation data

    static int destroy;         // Set to 1 if debug window is open
    int active_page;            // Whether dialogue, character or quest page is active
    int selected_row;           // The row selected in the active tree

private:
    void check_removed (GtkCTree*); // Check a tree for obsolete nodes
    int update_children (GtkCTree*, GtkCTreeNode*, storage*);

    GdkColor red;               // Red
    GdkColor green;             // Green 
    GdkColor black;             // Black
};

#endif // __DEBUG_H__
