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

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <gtk/gtkstyle.h>
#include "dlgnode.h"
#include "main.h"

// Wrapper class for the Node Editing dialog
class crcle_dlg
{
public:
    crcle_dlg (Circle*, MainFrame*);

    int on_ok ();               // Discard changes and exit
    void on_text (char *);      // Text has been updated
    void on_annotation (char*); // Note updated
    void on_condition (char *); // Conditions updated
    void on_variable (char *);  // Variables updated
    void on_type (u_int8);      // Type updated
    void on_combat (u_int8);    // Combat updated
    void on_loop (u_int8);      // Loop updated
    void on_change_dlg (u_int8);// dialogue/plugin radiobuttons changed
    u_int16 get_cur_dlg ();     // dialogue/plugin radiobutton setting

    GtkWidget *dlg;             // The actual Dialog Window
    GtkWidget *text_entry;      // The Editboxes for the Nodes text
    GtkWidget *npc_menu;        // The "chose NPC" option-menu
    GtkWidget *dlg_menu;        // The "chose dialogue" option-menu
    GtkWidget *dlg_action_list; // The list containing dialogue actions
    
    u_int8 retval;              // Return value

private:
    Circle *circle;             // The node we edit 
    MainFrame *wnd;             // The Application

    u_int8 type;                // Either PLAYER or NPC
    u_int8 combat;              // State of combat-checkbox
    u_int8 loop;                // State of Loop-checkbox
    u_int8 change_dlg;          // State of dialoge/plugin radiobuttons
    string text;                // Dialogue Text
    string note;                // Annotations
    string cond;                // Conditions for the node
    string vars;                // Statements changing game state variables
};

#endif // __CIRCLE_H__