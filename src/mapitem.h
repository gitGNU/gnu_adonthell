/*
    $Id$

    Copyright (C) 1999 The Adonthell Team.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

*/

#ifndef _mapitem_h
#define _mapitem_h

#include "types.h"
#include "mapevent.h"

class mapitem
{
	u_int16 number;
	u_int16 pattern;
	u_int16 event_walk;
	u_int16 event_action;
        char name[30];
        u_int8 status;

	public:
	mapitem();

	friend int loaditems(char*, mapitem**, mapevent**, u_int16*);
};

#define EVENT_ITEM 100
#define EVENT_ACTION 101
#define EVENT_TELEPORT 1
#define EVENT_MODIF_PICT 102
#define EVENT_GET 103
#define EVENT_GET_KEY 104
#define EVENT_OPEN 105
#define EVENT_WALK_FLAG 106
#define EVENT_PLACE_ITEM 107

/* Possible values for status */
#define ITEM_OPENED 0
#define ITEM_CLOSED 1
#define ITEM_UNLOCKED 2
#define ITEM_LOCKED 3

/* Known keywords (for events) */

#define KEYWORDS "0TELEPORT1MODIF_PICT2GET3GET_KEY4WALK_FLAG5OPEN6PLACE_ITEM"
#define KEYWORD_TELEPORT '0'
#define KEYWORD_MODIF_PICT '1'
#define KEYWORD_GET '2'
#define KEYWORD_GET_KEY '3'
#define KEYWORD_WALK_FLAG '4'
#define KEYWORD_OPEN '5'
#define KEYWORD_PLACE_ITEM '6'

/* functions prototypes */
int loaditems(char*, mapitem**, mapevent**, u_int16*);
void linesep(char*,char**,int*);
int test_line(char* string, char **word, int*wordc);

#endif
