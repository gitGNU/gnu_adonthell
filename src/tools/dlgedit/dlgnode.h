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

class DlgNode;

#include <vector>
#include "function.h"

/* Node */
class DlgNode
{
public:
    u_int32 number;             /* unique ID */
    u_int8 type;                /* one of NPC, PLAYER, LINK */
    u_int8 highlite;            /* color of node */

    gchar *text;                /* text of node */
    GdkRectangle position;      /* coordinates */
    GPtrArray *prev;            /* precedor(s) */
    GPtrArray *next;            /* successor(s) */
    GPtrArray *link;            /* indirect connection(s) */

    vector<function_data*> fctn;// assigned functions

    GdkPoint line[2];           /* Line of arrow */
    GdkPoint tip[3];            /* tip of arrow */
};


