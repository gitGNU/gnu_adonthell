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
#include <string>

#include "types.h"
#include "interpreter.h"
#include "dialog_cmds.h"
#include "dialog.h"
#include "storage.h"
#include "character.h"


dialog::dialog(void)
{

}

#if 0
// TOC of the dialogue string file
s_int32 *dialog::offset = NULL;
s_int32 *dialog::length = NULL;


// === IMPORT ===

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


// === CLEAR ===

// continues dialogue according to player's selection
s_int32 clear_cmd::run (u_int32 &pc, void *data)
{
    u_int32 i;
    dialog *dlg = (dialog *) data;

    // Memorize what parts of the dialogue where already used, to avoid loops
    if (find (dlg->loop_text.begin (), dlg->loop_text.end (), dlg->player_text[dlg->answer]->id) == dlg->loop_text.end ())
        dlg->used_text.push_back (dlg->player_text[dlg->answer]->id);

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


// === TEXT ===

// Initializes the command from the buffer
void text_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    text = buffer[i++];
    offset = buffer[i++];
}

// adds a line to the text arrays
s_int32 text_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;
    dlg_text *t;
    string s = dlg->strings[text];
    s_int32 i;
    string::iterator j;

    // Look if that part of the conversation was already in use     
    if (find (dlg->used_text.begin(), dlg->used_text.end(), text) == dlg->used_text.end ())
    {
        // The offset given with the TEXT command is relative to the current line
        // That is:  current_line + offset  gives the line where the dialogue will
        // continue when this text is chosen. Note that pc == current_line + 1 at
        // this point!
        t = new dlg_text (text, pc + offset - 1);

        // Assign this line of dialogue to the current speaker (who has been set
        // by the SPEAKER command). We only have to distinguish between player and
        // npc here, different npc's are dealt with by the SPEAKER command.
        if (dlg->speaker == 0) dlg->player_text.push_back (t);
        else dlg->npc_text.push_back (t);

        // Replace the $name and $fm macros by their proper values
        while ((i = s.find ("$name")) != -1)
            s.replace (i, 5, dlg->player_name);

        while ((i = s.find ("$fm")) != -1)
        {
            s.erase (i, 3);
            j = s.begin () + i;

            // extract the male part
            if (objects::get("player")->get("gender") == objects::get("game_state")->get("MALE"))
            {
                while ((*j) != '/') j++;
                s.erase (s.begin()+i, j+1);
                s.erase (s.find ("}", i), 1);
            }
            
            // extract the female part
            else
            {
                while ((*j) != '{') j++;
                s.erase (s.begin()+i, j+1);
                s.erase (s.find ("/", i), s.find ("}", i)+1-s.find ("/", i));
            }
        }

        delete[] dlg->strings[text];
        dlg->strings[text] = strdup (s.c_str ());
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
    fwrite (&offset, sizeof (offset), 1, out);
}

void text_cmd::ascii (ofstream &out)
{
    out << "TEXT    " << text << " " << offset;
}


// === LOOP ===

// Initializes the command from the buffer
void loop_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    text = buffer[i++];
}

// allows a text to be displayed multiple times
s_int32 loop_cmd::run (u_int32 &pc, void *data)
{
    dialog *dlg = (dialog *) data;

    // don't add the same text twice
    if (find (dlg->loop_text.begin (), dlg->loop_text.end (), text) == dlg->loop_text.end ())
        dlg->loop_text.push_back (text);
        
    return 1;
}

void loop_cmd::write (FILE *out)
{
    // write cmds type
    fwrite (&type, sizeof (type), 1, out);
    
    fwrite (&text, sizeof (text), 1, out);
}

void loop_cmd::ascii (ofstream &out)
{
    out << "LOOP    " << text;
}


// === SPEAKER ===

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


// === SWITCH_DLG ===

switch_dlg_cmd::switch_dlg_cmd (u_int32 d, char *n)
{
    type = SWITCH_DLG;
    dlg = d;
    npc_id = n;
}

void switch_dlg_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    npc_id = strread (buffer, i);
    dlg = buffer[i++];
}

s_int32 switch_dlg_cmd::run (u_int32 &PC, void *data)
{
    npc *the_npc = (npc*) objects::get(npc_id);
    the_npc->set_dialogue (dlg);

    return 1;
}

void switch_dlg_cmd::write (FILE *out)
{
    u_int32 i, l = strlen (npc_id);
    
    fwrite (&type, sizeof (type), 1, out);
    fwrite (&l, sizeof(l), 1, out);
    fwrite (npc_id, l, 1, out);
    for (i = 4; i > l%4; i--) fputc (0, out);
    fwrite (&dlg, sizeof (dlg), 1, out);
}

void switch_dlg_cmd::ascii (ofstream &out)
{
    out << "SWITCH  " << npc_id << " " << dlg;
}

#endif
