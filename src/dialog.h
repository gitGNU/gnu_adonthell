/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

class dialog
{
public:
    static s_int32 *offset;
    static s_int32 *length;

    u_int32 answer;
    char *text_file;
    char *npc_text;                     // NPC´s part
    Array<char*> player_text;           // player´s part
    Array<u_int32> cur_cmds;            // New PC values
    Array<u_int32> used_text;           // Text already spoken -> don´t display again
    Array<char*> strings;               // Text data
};
