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

// command types
// Add your own types to this list
enum
{
    RETURN = 0,        // RETURN <retval>           Stop interpreter
    IMPORT = 1,        // IMPORT <id>               Load Text
    SNPCTEXT = 2,      // SNPCTEXT <id> <offset>    NPC Text without player text
    NPCTEXT = 3,       // NPCTEXT <id>              NPC Text
    PTEXT = 4,         // PTEXT <id> <offset>       Player Text
    CLEAR = 5          // CLEAR                     Reset Text buffers
};

void init_interpreter ();