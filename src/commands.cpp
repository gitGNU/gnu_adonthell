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

// These macros expand to a function that allocates a new
// instance of your command and is needed for the REGISTER_CMD
// call below
NEW_CMD(return_cmd)
NEW_CMD(import_cmd)
NEW_CMD(clear_cmd)
NEW_CMD(ptext_cmd)
NEW_CMD(npctext_cmd)
NEW_CMD(snpctext_cmd)

// Add the registration of your commands to this function
// in the same order as in commands.h
void init_interpreter ()
{
    // General Commands:
    REGISTER_CMD(RETURN,return_cmd)      // RETURN has to be first

    // Dialoge Engine Commands:
    REGISTER_CMD(IMPORT,import_cmd)
    REGISTER_CMD(SNPCTEXT,snpctext_cmd)
    REGISTER_CMD(NPCTEXT,npctext_cmd)
    REGISTER_CMD(PTEXT,ptext_cmd)
    REGISTER_CMD(CLEAR,clear_cmd)
}
