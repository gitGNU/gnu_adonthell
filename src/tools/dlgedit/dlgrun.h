/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* Callback Data for running a Dialogue */
typedef struct
{
    dialog *data;
    interpreter *engine;
    GtkWidget *npc;
    GtkWidget *player;
}
RunData;

void run_dialogue (MainFrame *);
void StartDialogue (RunData *);
s_int8 ShowDialogue (RunData *);
