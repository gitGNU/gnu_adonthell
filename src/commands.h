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

// command types (must not exceed 255)
// Add your own types to this list
enum
{
    // Return has to be the first command (see interpreter.cc to find out the reason)
    RETURN = 0,        // RETURN <retval>           Stop interpreter

    // Dialogue
    IMPORT = 1,        // IMPORT <id>               Load Text
    SNPCTEXT = 2,      // SNPCTEXT <id> <offset>    NPC Text without player text
    NPCTEXT = 3,       // NPCTEXT <id>              NPC Text
    PTEXT = 4,         // PTEXT <id> <offset>       Player Text
    CLEAR = 5,         // CLEAR                     Reset Text buffers

    // Arithmetic
    LET = 6,
    ADD = 7,
    SUB = 8,
    MUL = 9,
    DIV = 10,

    // Comparisons
    EQ = 11,
    NEQ = 12,
    LT = 13,
    LEQ = 14,
    GT = 15,
    GEQ = 16,

    // Logic
    AND = 17,
    OR = 18,

    // Others
    JMP = 19,

    // Those four are used for parsing only. They are defined here to avoid
    //  conflicts with other commands.
    REG = 251,
    THEN = 252,
    ELSE = 253,
    ID = 254,
    NUM = 255
};

void init_interpreter ();