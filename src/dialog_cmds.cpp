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


// TOC of the dialogue string file
s_int32 *dialog::offset = NULL;
s_int32 *dialog::length = NULL;


// Initializes the command from the buffer
void import_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    u_int32 j;

    sz = buffer[i++];
    text = new u_int32[sz];

    for (j = 0; j < sz; j++)
        text[j] = buffer[i++];
}

// reads all strings from the dialogue string file
s_int32 import_cmd::run (u_int32 &pc, void *data)
{
    u_int32 i;
    dialog *dlg = (dialog *) data;
    FILE *txt = fopen (dlg->text_file, "r");

    for (i = 0; i < sz; i++)
    {
        char *str = new char[dialog::length[text[i]] + 1];

        fseek (txt, dialog::offset[text[i]], SEEK_SET);
        fgets (str, dialog::length[text[i]] + 1, txt);

        dlg->strings.push_back (str);
    }
    
    fclose (txt);

    return 1;
}

// write command to script file
void import_cmd::write (FILE *out)
{
    // write number of strings
    fwrite (&sz, sizeof (sz), 1, out);

    // write string IDs
    fwrite (text, sizeof (text[0]), sz, out);
}

// output command in human readable form
void import_cmd::ascii (FILE *out)
{
}


// continues dialogue according to players selection
s_int32 clear_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;

    dlg->used_text.push_back (pc);

    // be careful, cause pc is already incremented by one here
    pc += dlg->player_text[dlg->answer]->offset;

    // have to  delete  the contents of the 2 vector's first
    dlg->player_text.clear ();
    dlg->npc_text.clear ();

    return 1;
}

// adds a line to the players list of possible answers
s_int32 ptext_cmd::run (u_int32 &pc, void *data)
{
/*
    dialog *dlg = (dialog *) data;
     
    if (dlg->used_text.find (new_pc) != dlg->used_text.end ())
    {
        dlg->player_text.add_element (dlg->strings.get_element (text));
        dlg->cur_cmds.add_element (new_pc);
    }
*/
    return 1;
}

// adds an npc´s text where no player text follows
s_int32 snpctext_cmd::run (u_int32 &pc, void *data)
{
/*
    dialog *dlg = (dialog *) data;
    
    dlg->npc_text = dlg->strings.get_element (text);
    dlg->cur_cmds.add_element (new_pc);
*/
    return 1;
}

// sets the npc´s text
s_int32 npctext_cmd::run (u_int32 &pc, void *data)
{
/*
    dialog *dlg = (dialog *) data;
    
    dlg->npc_text = dlg->strings.get_element (text);
*/
    return 1;
}