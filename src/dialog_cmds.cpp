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
#include <fstream.h>

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
    char *str;
    dialog *dlg = (dialog *) data;
    FILE *txt = fopen (dlg->text_file, "r");

    for (i = 0; i < sz; i++)
    {
        str = new char[dialog::length[text[i]] + 1];

        fseek (txt, dialog::offset[text[i]], SEEK_SET);
        fread (str, dialog::length[text[i]], 1, txt);
        str[dialog::length[text[i]]] = '\0';

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
void import_cmd::ascii (ofstream &out)
{
    u_int32 i;

    out << "IMPORT  ";

    for (i = 0; i < sz; i++)
        out << text[i] << " ";
}


// continues dialogue according to players selection
s_int32 clear_cmd::run (u_int32 &pc, void *data)
{
    u_int32 i;
    dialog *dlg = (dialog *) data;

    // Memorize what parts of the dialogue where already used, to avoid loops
    dlg->used_text.push_back (dlg->player_text[dlg->answer]->offset);

    // Continue the dialogue according to the players choice
    pc = dlg->player_text[dlg->answer]->offset;

    // have to  delete  the contents of the 2 vector's first
    for (i = 0; i < dlg->player_text.size (); i++)
        if (dlg->player_text[i] != NULL) delete dlg->player_text[i];

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

void clear_cmd::ascii (ofstream &out)
{
    out << "CLEAR";
}


// Initializes the command from the buffer
void text_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    text = buffer[i++];
    pc_off = buffer[i++];
}

// adds a line to the text arrays
s_int32 text_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    dlg_text *t;

    // Look if that part of the conversation was already in use     
    if (find (dlg->used_text.begin(), dlg->used_text.end(), pc + pc_off - 1) == dlg->used_text.end ())
    {
        // The offset given with the TEXT command is relative to the current line
        // That is:  current_line + pc_off  gives the line where the dialogue will
        // continue when this text is chosen. Note that pc == current_line + 1 at
        // this point!
        t = new dlg_text (text, pc + pc_off - 1);

        // Assign this line of dialogue to the current speaker (who has been set
        // by the SPEAKER command). We only have to distinguish between player and
        // npc here, different npc's are dealt with by the SPEAKER command.
        if (dlg->speaker == 0) dlg->player_text.push_back (t);
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
}

void text_cmd::ascii (ofstream &out)
{
    out << "TEXT    " << text << " " << pc_off;
}

void text_cmd::setjmp (s_int32 p)
{
    pc_off = p;
}

s_int32 text_cmd::getjmp ()
{
    return pc_off;
}

speaker_cmd::speaker_cmd (u_int32 s, u_int32 m) : speaker(s), mood(m)
{
    type = SPEAKER;
}

// Initializes the command from the buffer
void speaker_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    speaker = buffer[i++];
    mood = buffer[i++];
}

// adds a line to the text arrays
s_int32 speaker_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;

    // have to deal with different NPCs and stuff later
    // (like changing their portraits if their mood changes and so on ...)
    dlg->speaker = speaker;

    // if the NPC has multiple lines of text, we have to add a dummy element to
    // the player_text - list, so we know what Player-text belongs to what
    // NPC text later
    if (!dlg->npc_text.empty () && speaker != 0)
        dlg->player_text.push_back (NULL);
    
    return 1;
}

void speaker_cmd::write (FILE *out)
{
    // write cmds type
    fwrite (&type, sizeof (type), 1, out);
    
    fwrite (&speaker, sizeof (speaker), 1, out);
    fwrite (&mood, sizeof (mood), 1, out);
}

void speaker_cmd::ascii (ofstream &out)
{
    out << "SPEAKER " << speaker << " " << mood;
}
