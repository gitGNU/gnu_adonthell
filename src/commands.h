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

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "types.h"

// command types (must not exceed 255 - at least those used with the dialogue compiler)
// Add your own types to this list
enum
{
    // Return has to be the first command (see interpreter.cc to find out the reason)
    RETURN = 0,     // RETURN <retval>              Stop interpreter

    // Dialogue
    IMPORT = 1,     // IMPORT <id-list>             Load Text
    TEXT = 2,       // TEXT <id> <offset>           Line of text
    CLEAR = 3,      // CLEAR                        Reset Text buffers
    SPEAKER = 4,    // SPEAKER <speaker> <mood>     Sets the Speaker
    SWITCH_DLG = 5, // SWITCH_DLG <npc> <dlg>       Changes the NPC's active dialogue
    LOOP = 6,       // LOOP <id>                    Text may be chosen multiple times
    
    // Arithmetic
    LET = 7,        // LET <id1> <id2>              id2 = id1
    ADD = 8,        // ADD <id1> <id2> <id3>        id3 = id1 + id2
    SUB = 9,        // SUB <id1> <id2> <id3>        id3 = id1 - id2
    MUL = 10,       // MUL <id1> <id2> <id3>        id3 = id1 * id2
    DIV = 11,       // DIV <id1> <id2> <id3>        id3 = int (id1 / id2)

    // Comparisons
    EQ = 12,        // EQ <id1> <id2> <bool>        bool = id1 == id2 ? true : false
    NEQ = 13,       // NEQ <id1> <id2> <bool>       bool = id1 != id2 ? true : false
    LT = 14,        // LT <id1> <id2> <bool>        bool = id1 < id2 ? true : false
    LEQ = 15,       // LEQ <id1> <id2> <bool>       bool = id1 <= id2 ? true : false
    GT = 16,        // GT <id1> <id2> <bool>        bool = id1 > id2 ? true : false
    GEQ = 17,       // GEQ <id1> <id2> <bool>       bool = id1 >= id2 ? true : false

    // Logic
    AND = 18,       // AND <bool1> <bool2> <bool3>  bool3 = bool1 && bool2
    OR = 19,        // OR <bool1> <bool2> <bool3>   bool3 = bool1 || bool2

    // Program flow
    JMP = 20,       // JMP <offset>                 PC += offset
    BRANCH = 21,    // BRANCH <bool> <offset>       bool == true ? PC++ : PC += offset

    // Functions
    RAND = 22,      // RAND <min> <max> <id>        id = random number out of [min; max]

    // Those five are used during parsing only. They are defined here to avoid
    // conflicts with other commands, but are only used internally by the 
    // dialogue compiler.
    ENDIF = 251,    // Specifies the end of an if [- else] statement
    BOOL = 252,     // Specifies a boolean value
    REG = 253,      // Specifies an internal (integer) register
    ID = 254,       // Specifies a variable name
    NUM = 255       // Specifies an integer value
};


// Base class for commands with offsets
class offset_cmd
{
public:
    void setjmp (s_int32);
    s_int32 getjmp ();
    
protected:
    s_int32 offset;
};


void init_interpreter ();

#endif // __COMMANDS_H__
