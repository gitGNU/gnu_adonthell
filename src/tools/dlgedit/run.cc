/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gtk/gtk.h>
#include "run.h"
#include "run_interface.h"
#include "../../dialog.h"

run_dlg::run_dlg (string f)
{
    string sf = f + ".str";
    string df = f + ".dlg";
    FILE *str;
    u_int32 num;
    u_int32 i;

    dlg = create_run_dlg_wnd (this);

    // load TOC of stringfile
    str = fopen (sf.c_str (), "r");
    fread (&num, sizeof (num), 1, str);

    cout << "\nNum: " << num << " -- ";

    dialog::offset = new s_int32[num];
    dialog::length = new s_int32[num];

    fread (dialog::offset, num, sizeof (dialog::offset[0]), str);
    fread (dialog::length, num, sizeof (dialog::length[0]), str);

    for (i = 0; i < num; i++)
        cout << dialog::offset[i] << " (" << dialog::length[i] << "), ";

    fclose (str);

    // Init Dialogue data
    dat = new dialog;
    dat->text_file = strdup (sf.c_str ());

    // Init Interpreter
    vm = new interpreter (strdup (df.c_str ()), dat);
}


run_dlg::~run_dlg ()
{
    delete[] dialog::offset;
    delete[] dialog::length;
    delete dat;
    delete vm;
}


void run_dlg::run ()
{
    s_int32 retval;
    tmp_list = NULL;
    u_int32 npc_answer;
    u_int32 i, index = 0;

    // Start the interpreter
    retval = vm->run ();

    // See if we reached the end of the script
    if (retval == 0) return;

    // Prepare the random number generator
    randgen.init (" ", 0, dat->npc_text.size ());
    randgen.randomize ();

    // Get the NPCs answer in case there are more than one possible NPC answers
    // In the other case  randgen  will deliver a number between 0 and 0, which is
    // the only available NPC text
    npc_answer = randgen.get ();

    // Now examine  dlg  and see what we've got
    // case 1: only one or more NPC-lines
    if (dat->player_text.empty ())
    {
        // We trick a bit and make the interpreter think he has a Player-
        // text to continue the dialogue (that saves us from some ugly 
        // if-statements)        
        dat->player_text.push_back (dat->npc_text[npc_answer]);

        // Now fill the list with the text (i.e add an empty line and then the
        // clickable NPC text
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -2));
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->npc_text[npc_answer]->id], 5, 0));
    }
    
    // case 2: there is NPC text and Player text available
    else
    {
        // Fill in the NPC text
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -2));
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->npc_text[npc_answer]->id], 4, -1));

        // Now look for the player text
        for (i = 0; i < npc_answer; )
            if (dat->player_text[index++] == NULL) i++;

        // ... and add it to the list
        while (dat->player_text[index] != NULL && index < dat->player_text.size ())
        {
            tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->player_text[index]->id], 3, index));            
            index++;
        }
    }

    // Now stuff everything into the list!
    gtk_widget_hide (list);
    gtk_list_append_items (GTK_LIST (list), tmp_list); 
    gtk_widget_show (list);

    return;
}