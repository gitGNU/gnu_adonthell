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


#ifndef __DLGNODE_H__
#define __DLGNODE_H__

#include <vector>
#include <string>
#include <fstream.h>
#include <gdk/gdktypes.h>
#include "../../types.h"

// Definitions needed for the dialogue source file parser
extern int parse_dlgfile (string&, int&);
extern FILE* loadlgin;

// For loading Dialogue Files
enum
{
    LOAD_CIRCLE = 1,
    LOAD_ARROW = 2,
    LOAD_END = 3,
    LOAD_TYPE = 4,
    LOAD_PREV = 5,
    LOAD_NEXT = 6,
    LOAD_LINK = 7,
    LOAD_POS = 8,
    LOAD_NOTE = 9,
    LOAD_TEXT = 10,
    LOAD_COND = 11,
    LOAD_VARS = 12,
    LOAD_FUNC = 13,
    LOAD_ACT = 14,
    LOAD_NAME = 15,
    LOAD_RACE = 16,
    LOAD_GENDER = 17,
    LOAD_NPC = 18,
    LOAD_STR = 19,
    LOAD_NUM = 20,
    LOAD_UNKNOWN = 21
};

// Node Types
enum
{
    PLAYER = 1,
    NPC = 2,
    LINK = 3,
    MOVER = 4,
    NARRATOR = 5
};

// Base class for the Dialogue Nodes
class DlgNode
{
public:
    DlgNode () { highlite = 0; }
    virtual ~DlgNode () { }
    
    u_int32 number;                     // unique ID
    u_int8 type;                        // one of NPC, PLAYER, LINK
    u_int8 highlite;                    // color of node

    GdkRectangle position;              // coordinates
    vector<DlgNode*> prev;              // precedessor(s)
    vector<DlgNode*> next;              // successor(s)
    vector<DlgNode*> link;              // indirect connection(s)

    virtual void save (ofstream&, u_int32*) { }   // Save DlgNode
};

// Circle
class Circle : public DlgNode
{
public:
    Circle ();
    Circle (u_int32, u_int8, s_int32, s_int32);
    virtual ~Circle () { }
    
    u_int32 mood;                       // This characters mood
                        
    string text;                        // text of node
    string comment;                     // User's annotation
    string conditions;                  // Condition script
    string variables;                   // Variable script
    string actions;                     // further actions
    string character;                   // Who's characters text is this?
    
    void save (ofstream&, u_int32*);    // Save Circle
    void load (u_int32);                // Load Circle
}; 

// Arrow
class Arrow : public DlgNode
{
public:
    Arrow () { }
    Arrow (u_int32, u_int8);
    virtual ~Arrow () { }

    GdkPoint line[2];                   // Line of arrow
    GdkPoint tip[3];                    // tip of arrow

    void save (ofstream&, u_int32*);    // Save Arrow
    void load (u_int32, vector<DlgNode*>&);// Load Arrow
};

#endif // __DLGNODE_H__
