
/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

/* command types */
enum
{
    IMPORT = 10,                /* IMPORT     <offset> <length> Load Text */
    SNPCTEXT = 20,              /* SNPCTEXT   <id> <offset>  NPC Text without player text */
    NPCTEXT = 21,               /* NPCTEXT    <id>     NPC Text */
    PTEXT = 22,                 /* PTEXT      <id> <offset>  Player Text */
    CLEAR = 40,                 /* CLEAR      Reset Text buffers */
    DISPLAY = 50,               /* DISPLAY      Display text buffers */
    JUMP = 90,                  /* JUMP       <offset>          Set PC to offset */
    END = 100                   /* END       Quit dialogue */
};

/* Base class of the commands */
typedef struct
{
    s_int32 type;               /* Command´s type */
    s_int32 text;               /* ID of string to load */
    s_int32 new_pc;             /* New value of Program Counter */
}
dialog_cmd;