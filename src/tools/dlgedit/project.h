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

enum
{
    CHARACTER = 0,
    DIALOGUE = 1
};

// A Entry of the project tree
class project_item
{
public:

    void save (FILE*);      // save an item to disk
    void load (FILE*);      // load an item
    
protected:
    u_int32 type;           // Type thereof; either CHARACTER or DIALOGUE
    char *note;             // Personal notes
    char *label;            // Label of the entry
    char *file;             // The actual npc or dlg represented by the item
    u_int32 id;             // A unique id used throughout the project
    Array<u_int32> attached;// The Dlgs attached to a NPC and vice versa
};

// All Information needed for the project management
class project
{
friend class project_item;

public:
    GtkWidget *tree;        // Project Tree
    GtkWidget *note;        // "Personal note" edit box
    GtkWidget *npc_frame;   // NPC information
    GtkWidget *npc_add;     // Add/Remove NPC
    GtkWidget *dlg_frame;   // Dialogue information
    GtkWidget *dlg_add;     // Add/Remove Dialogue
    GtkWidget *npc_list;    // Dropdown list of all available NPCs

    char *file_name;        // Name of the project file
};