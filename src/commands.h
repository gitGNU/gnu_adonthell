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

// command types (must not exceed 255 - at least those used with the dialogue compiler)
// Add your own types to this list
enum
{
    // Return has to be the first command (see interpreter.cc to find out the reason)
    RETURN = 0,     // RETURN <retval>              Stop interpreter

    // Dialogue (those five will change in future versions!!!)
    IMPORT = 1,     // IMPORT <id-list>             Load Text
    TEXT = 2,       // TEXT <id> <offset>           Line of text
    CLEAR = 3,      // CLEAR                        Reset Text buffers
    SPEAKER = 4,    // SPEAKER <speaker> <mood>     Sets the Speaker
    
    // Arithmetic
    LET = 6,        // LET <id1> <id2>              id2 = id1
    ADD = 7,        // ADD <id1> <id2> <id3>        id3 = id1 + id2
    SUB = 8,        // SUB <id1> <id2> <id3>        id3 = id1 - id2
    MUL = 9,        // MUL <id1> <id2> <id3>        id3 = id1 * id2
    DIV = 10,       // DIV <id1> <id2> <id3>        id3 = int (id1 / id2)

    // Comparisons
    EQ = 11,        // EQ <id1> <id2> <bool>        bool = id1 == id2 ? true : false
    NEQ = 12,       // NEQ <id1> <id2> <bool>       bool = id1 != id2 ? true : false
    LT = 13,        // LT <id1> <id2> <bool>        bool = id1 < id2 ? true : false
    LEQ = 14,       // LEQ <id1> <id2> <bool>       bool = id1 <= id2 ? true : false
    GT = 15,        // GT <id1> <id2> <bool>        bool = id1 > id2 ? true : false
    GEQ = 16,       // GEQ <id1> <id2> <bool>       bool = id1 >= id2 ? true : false

    // Logic
    AND = 17,       // AND <bool1> <bool2> <bool3>  bool3 = bool1 && bool2
    OR = 18,        // OR <bool1> <bool2> <bool3>   bool3 = bool1 || bool2

    // Program flow
    JMP = 19,       // JMP <offset>                 PC += offset
    BRANCH = 20,    // BRANCH <bool> <offset>       bool == true ? PC++ : PC += offset

    // Those five are used during parsing only. They are defined here to avoid
    // conflicts with other commands, but are only used internally by the 
    // dialogue compiler.
    ENDIF = 251,    // Specifies the end of an if [- else] statement
    BOOL = 252,     // Specifies a boolean value
    REG = 253,      // Specifies an internal (integer) register
    ID = 254,       // Specifies a variable name
    NUM = 255       // Specifies an integer value
};

void init_interpreter ();

#endif // __COMMANDS_H__