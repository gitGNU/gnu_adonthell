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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../array_tmpl.h"
#include "../../interpreter.h"
#include "../../commands.h"
#include "../../dialog.h"
#include "linked_list.h"
#include "dlgnode.h"
#include "main.h"
#include "dlgrun.h"
#include "interface.h"

/* run the compiled dialogue */
void 
run_dialogue (MainFrame * wnd)
{
    RunData *rd = (RunData *) g_malloc (sizeof (RunData));
    GtkWidget *dlg;

    /* Register commands with interpreter 
       Normally you would do this once at Program startup */
    interpreter::callbacks.clear ();
    init_interpreter ();

    rd->data = new dialog;
    rd->data->text_file = g_strjoin (NULL, wnd->file_name, ".str", NULL);
    rd->engine = new interpreter (g_strjoin (NULL, wnd->file_name, ".dat", NULL), rd->data);

    /* Dialog for displaying Dialogue */
    dlg = create_run_dialogue (rd);
    gtk_widget_show (dlg);

    /* Here the dialogue gets initialized */
    /* The first parameter is the part of the dialogue to start with */
    StartDialogue (rd);
    
    /* Thats the dialogue loop */
    gtk_main ();

    /* Clean up */
    gtk_widget_destroy (dlg);
    delete rd->engine;
    delete rd->data;
    delete[] dialog::offset;
    delete[] dialog::length;
    g_free (rd);
}

/* Initialize Dialogue */
void
StartDialogue (RunData * rd)
{
    FILE *str = fopen (rd->data->text_file, "r");
    u_int32 count;

    fread (&count, sizeof(count), 1, str);
        
    dialog::offset = new s_int32[count];
    dialog::length = new s_int32[count];

    fread (dialog::offset, sizeof(dialog::offset[0]), count, str);
    fread (dialog::length, sizeof(dialog::length[0]), count, str);

    fclose (str);
    
    /* Start */
    ShowDialogue (rd);
}

/* Continue Dialogue */
s_int8 
ShowDialogue (RunData * rd)
{
    u_int32 i;
    s_int32 result;
    char *text[1];
    char **player_text;

    /* Thats the important call to the Dialogue engine with the (zero based)
       index of the chosen answer from the list of answers */
    result = rd->engine->run ();

    switch (result)
    {
        case -1:
        {
            /* error */
            gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
            gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
                &rd->npc->style->white, &rd->npc->style->black,
                "Interpreter returned error", -1);
            return 0;
        }

        case 0:
        {
            /* the end */
            gtk_main_quit ();
            return 1;
        }
        
        case 1:
        {
            /* Player part */
            gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
            gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
                &rd->npc->style->black, &rd->npc->style->white,
                rd->data->npc_text, -1);

            gtk_clist_freeze (GTK_CLIST (rd->player));
            gtk_clist_clear (GTK_CLIST (rd->player));

            player_text = rd->data->player_text.get_array ();
            
            for (i = 0; i < rd->data->player_text.length (); i++)
            {
                text[0] = player_text[i];
                gtk_clist_append (GTK_CLIST (rd->player), text);
            }
        
            gtk_clist_thaw (GTK_CLIST (rd->player));
            break;
        }

        case 2:
        {
            /* NPC only */
            gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
            gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
                &rd->npc->style->black, &rd->npc->style->white,
                rd->data->npc_text, -1);

            gtk_clist_freeze (GTK_CLIST (rd->player));
            gtk_clist_clear (GTK_CLIST (rd->player));

            text[0] = "... continue ...";

            gtk_clist_append (GTK_CLIST (rd->player), text);
            gtk_clist_thaw (GTK_CLIST (rd->player));
            break;
        }
    }
    
    return result;
}
