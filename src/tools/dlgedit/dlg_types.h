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
    PLAYER          = 1,
    NPC             = 2,
    LINK            = 3,
    MOVER           = 4,
    NARRATOR        = 5,
    MODULE          = 6
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

#define LOAD    1
#define SAVE    2

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

/**
 * The size of a DlgCircle
 */
#define CIRCLE_DIAMETER 20
    
#endif // DLG_TYPES_H
