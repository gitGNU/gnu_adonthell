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

#ifndef _DLGEDIT_
#define _DLGEDIT_
#endif

#include <stdio.h>
#include <iostream.h>

#include "types.h"
#include "interpreter.h"
#include "dialog_cmds.h"
#include "dialog.h"

s_int32 *dialog::offset = NULL;
s_int32 *dialog::length = NULL;

// reads a string from the dialogue string file
s_int32 import_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    FILE *txt = fopen (dlg->text_file, "r");
    char *str = new char[dialog::length[text] + 1];

    fseek (txt, dialog::offset[text], SEEK_SET);
    fgets (str, dialog::length[text] + 1, txt);

    dlg->strings.add_element(str);

    fclose (txt);

    return 1;
}

// continues dialogue according to players selection
s_int32 clear_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;

    pc = dlg->cur_cmds.get_element (dlg->answer);
    dlg->used_text.add_element (pc);

    dlg->player_text.clear ();
    dlg->cur_cmds.clear ();
    dlg->npc_text = NULL;

    return 1;
}

// adds a line to the players list of possible answers
s_int32 ptext_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
     
    if (!dlg->used_text.search_element (new_pc))
    {
        dlg->player_text.add_element (dlg->strings.get_element (text));
        dlg->cur_cmds.add_element (new_pc);
    }

    return 1;
}

// adds an npc�s text where no player text follows
s_int32 snpctext_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    
    dlg->npc_text = dlg->strings.get_element (text);
    dlg->cur_cmds.add_element (new_pc);

    return 1;
}

// sets the npc�s text
s_int32 npctext_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    
    dlg->npc_text = dlg->strings.get_element (text);

    return 1;
}