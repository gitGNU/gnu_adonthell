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
#include "../../storage.h"
#include "../../character.h"
#include "../../py_inc.h"

run_dlg::run_dlg (string f, string v, player *p)
{
    char *file = g_basename (f.c_str ());
    char *path = g_dirname (f.c_str ());
    dlg = create_run_dlg_wnd (this);

    dat = new dialog;
    dat->answer = 0;

    // Import module
    insert_path (path);
    dat->class_obj = import_module (file);

    free (path);
}


run_dlg::~run_dlg ()
{
#if 0
    delete[] dialog::offset;
    delete[] dialog::length;

    delete dat;
    delete vm;
#endif
}


void run_dlg::run ()
{
#if 0
    s_int32 retval;
    tmp_list = NULL;
    s_int32 npc_answer;
    u_int32 i, index = 0;
    GtkAdjustment *adj;

    // Start the interpreter
    retval = vm->run ();

    // See if we reached the end of the script
    if (retval == 0) return;

    // Prepare the random number generator
    if (dat->npc_text.size ())
        randgen.init (dat->strings[dat->npc_text[0]->id], 0, dat->npc_text.size ()-1);
    else
        randgen.init (" ", -1, -1);
    randgen.randomize ();

    // Get the NPCs answer in case there are more than one possible NPC answers
    // In the other case  randgen  will deliver a number between 0 and 0, which is
    // the only available NPC text
    npc_answer = randgen.get (5);

    // see if  player_text  contains something different than NULL's
    for (i = 0; i < dat->player_text.size (); i++)
        if (dat->player_text[i] == NULL)
            index++;

    // Now examine  dlg  and see what we've got
    // case 1: only one or more NPC-lines
    if (dat->player_text.size () == index)
    {
        // Possibly  player_text  contains some NULL values, so we clear it
        // first. It can't contain non-NULL values though, therefore we don't
        // leave any memory unallocated
        dat->player_text.clear ();
        
        // We trick a bit and make the interpreter think he has a Player-
        // text to continue the dialogue (that saves us from some ugly 
        // if-statements) (make hard copy!!!)
        dat->player_text.push_back (new dlg_text(dat->npc_text[npc_answer]));

        // Now fill the list with the text (i.e add an empty line and then the
        // clickable NPC text
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -2));
        tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->npc_text[npc_answer]->id], 5, 0));
    }
    
    // case 2: there is NPC text and Player text available
    else
    {
        index = 0;
        
        // Fill in the NPC text
        if (npc_answer != -1)
        {
            tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, -2));
            tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->npc_text[npc_answer]->id], 4, -1));
        }
        else tmp_list = g_list_append (tmp_list, create_dlg_list_item (" ", 2, 0));

        // Now look for the player text
        for (i = 0; (i < dat->player_text.size () && (s_int32) index < npc_answer); i++)
            if (dat->player_text[i] == NULL) index++;

        index = 0;
        
        // ... and add it to the list
        while (dat->player_text[i] != NULL && i < dat->player_text.size ())
        {
            if (i != index)
            {
                if (dat->player_text[index] != NULL) delete dat->player_text[index];
                dat->player_text[index] = new dlg_text (dat->player_text[i]);
            }
            
            tmp_list = g_list_append (tmp_list, create_dlg_list_item (dat->strings[dat->player_text[i]->id], 3, index++));            
            i++;
        }
    }

    // Now stuff everything into the list
    gtk_list_append_items (GTK_LIST (list), tmp_list);

    // and scroll to its bottom (gtk_list_scroll_vertical () didn't work!)
    adj = gtk_viewport_get_vadjustment ((GtkViewport *) list->parent);
    gtk_adjustment_set_value (adj, adj->upper);
    
    return;
#endif
}
