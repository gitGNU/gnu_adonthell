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
    // write cmds type
    fwrite (&type, sizeof (type), 1, out);
    
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
    u_int32 i;
    dialog *dlg = (dialog *) data;

    // Memorize what parts of the dialogue where already used, to avoid loops
    dlg->used_text.push_back (pc);

    // ksterker: be careful, cause pc is already incremented by one here
    pc += dlg->player_text[dlg->answer]->offset;

    // have to  delete  the contents of the 2 vector's first
    for (i = 0; i < dlg->player_text.size (); i++)
        delete dlg->player_text[i];

    for (i = 0; i < dlg->npc_text.size (); i++)
        delete dlg->npc_text[i];

    // Empty the text-arrays 
    dlg->player_text.clear ();
    dlg->npc_text.clear ();

    return 1;
}

void clear_cmd::write (FILE *out)
{
    // write cmds type
    fwrite (&type, sizeof (type), 1, out);
}

void clear_cmd::ascii (FILE *out)
{
}


// Initializes the command from the buffer
void text_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    text = buffer[i++];
    pc_off = buffer[i++];
    speaker = buffer[i++];
}

// adds a line to the text arrays
s_int32 text_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    dlg_text *t;

    // Look if that part of the conversation was already in use     
    if (find (dlg->used_text.begin(), dlg->used_text.end(), pc + pc_off) != dlg->used_text.end ())
    {
        t = new dlg_text (text, pc_off);

        // Assign this line of dialogue either to player or NPC
        if (speaker == 0) dlg->player_text.push_back (t);
        else dlg->npc_text.push_back (t);
    }

    return 1;
}

void text_cmd::write (FILE *out)
{
    // write cmds type
    fwrite (&type, sizeof (type), 1, out);
    
    // write text id
    fwrite (&text, sizeof (text), 1, out);

    // write offset
    fwrite (&pc_off, sizeof (pc_off), 1, out);

    // write speaker
    fwrite (&speaker, sizeof (speaker), 1, out);
}

void text_cmd::ascii (FILE *out)
{
}