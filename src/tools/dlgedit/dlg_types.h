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
 * @file dlg_types.h
 *
 * @author Kai Sterker
 * @brief Typedefs and Enumerations used throughout dlgedit
 */

#ifndef DLG_TYPES_H
#define DLG_TYPES_H

/**
 * The type of a given DlgNode.
 */
enum node_type
{
    PLAYER          = 0,
    NPC             = 1,
    NARRATOR        = 2,
    LINK            = 3,
    MODULE          = 4
};

/**
 * Specifies which node to retrieve.
 */ 
enum query_type
{
    FIRST           = 0,
    NEXT            = 1,
    PREV            = 2,
    LAST            = 3
};

/**
 * Defines the state of an individual node, but also that of the whole program.
 */
enum mode_type
{
    NONE            = 0,
    NODE_SELECTED   = 1,
    NODE_HILIGHTED  = 2
};

/**
 * A few predefined colors for drawing operations
 */
enum
{
    GC_RED          = 0,
    GC_DARK_RED     = 1,
    GC_DARK_BLUE    = 2,
    GC_GREEN        = 3,
    GC_DARK_GREEN   = 4,
    GC_YELLOW       = 5,
    GC_GREY         = 6,
    GC_BLACK        = 7,
    GC_WHITE        = 8,
    MAX_GC          = 9
};

enum menu_item
{
    SAVE            = 0,
    CLOSE           = 1,
    SETTINGS        = 2,
    FUNCTIONS       = 3,
    COMPILE         = 4,
    RUN             = 5,
    MAX_ITEM        = 6
};

/**
 * The size of a DlgCircle
 */
#define CIRCLE_DIAMETER 20
    
#endif // DLG_TYPES_H
