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

#include <iostream.h>
#include "types.h"
#include "commands.h"
#include "interpreter.h"

// Include the header file of your own commands here
#include "dialog_cmds.h"
#include "generic_cmds.h"

// These macros expand to a function that allocates a new
// instance of your command and is needed for the REGISTER_CMD
// call below
NEW_CMD (return_cmd)
NEW_CMD (import_cmd)
NEW_CMD (clear_cmd)
NEW_CMD (text_cmd)
NEW_CMD (speaker_cmd)
NEW_CMD (add_cmd)
NEW_CMD (sub_cmd)
NEW_CMD (mul_cmd)
NEW_CMD (div_cmd)
NEW_CMD (and_cmd)
NEW_CMD (or_cmd)
NEW_CMD (eq_cmd)
NEW_CMD (neq_cmd)
NEW_CMD (lt_cmd)
NEW_CMD (leq_cmd)
NEW_CMD (gt_cmd)
NEW_CMD (geq_cmd)
NEW_CMD (let_cmd)
NEW_CMD (jmp_cmd)
NEW_CMD (branch_cmd)


// Add the registration of your commands to this function
void init_interpreter ()
{
    // General Commands:
    REGISTER_CMD (RETURN, return_cmd)
    REGISTER_CMD (IMPORT, import_cmd)
    REGISTER_CMD (ADD, add_cmd)
    REGISTER_CMD (SUB, sub_cmd)
    REGISTER_CMD (MUL, mul_cmd)
    REGISTER_CMD (DIV, div_cmd)
    REGISTER_CMD (AND, and_cmd)
    REGISTER_CMD (OR, or_cmd)
    REGISTER_CMD (EQ, eq_cmd)
    REGISTER_CMD (NEQ, neq_cmd)
    REGISTER_CMD (LT, lt_cmd)
    REGISTER_CMD (LEQ, leq_cmd)
    REGISTER_CMD (GT, gt_cmd)
    REGISTER_CMD (GEQ, geq_cmd)
    REGISTER_CMD (LET, let_cmd)
    REGISTER_CMD (JMP, jmp_cmd)
    REGISTER_CMD (BRANCH, branch_cmd)

    // Dialoge Engine Commands:
    REGISTER_CMD (IMPORT, import_cmd)
    REGISTER_CMD (TEXT, text_cmd)
    REGISTER_CMD (CLEAR, clear_cmd)
    REGISTER_CMD (SPEAKER, speaker_cmd)
}
