/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

class dialog;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "../../map/types.h"
#include "../../dialog/dlg_io.h"
#include "../../dialog/dialog_cmd.h"
#include "../../dialog/array_tmpl.h"
#include "../../dialog/dialog.h"
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

    rd->engine = new dialog;

    /* Dialog for displaying Dialogue */
    dlg = create_run_dialogue (rd);
    gtk_widget_show (dlg);

    /* Here the dialogue gets initialized */
    /* The first parameter is the part of the dialogue to start with */
    StartDialogue (wnd, rd, 0);
    
    /* Thats the dialogue loop */
    gtk_main ();

    /* Clean up */
    gtk_widget_destroy (dlg);
    delete rd->engine;
    g_free (rd);
}

/* Initialize Dialogue */
void
StartDialogue (MainFrame * wnd, RunData * rd, u_int32 start)
{
    u_int32 result;

    /* Tell the Engine to load the Dialogue */
    result = rd->engine->init_dialog (wnd->file_name, start);

    if (!result)
    {
        gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
        gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
            &rd->npc->style->white, &rd->npc->style->black,
            rd->engine->error_text, -1);

        return;
    }

    /* Start */
    ShowDialogue (rd, start);
}

/* Continue Dialogue */
s_int8 
ShowDialogue (RunData * rd, u_int32 answer)
{
    s_int32 i;
    s_int32 result;
    char *text[1];
    char **player_text;

    /* Thats the important call to the Dialogue engine with the (zero based)
       * index of the chosen answer from the list of answers */
    result = rd->engine->run_dialog (answer);

    switch (result)
    {
        case -1:
        {
            /* the end */
            gtk_main_quit ();
            return 1;
        }
        
        case -2:
        {
            /* error */
            gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
            gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
                &rd->npc->style->white, &rd->npc->style->black,
                rd->engine->error_text, -1);
            return 0;
        }

        default:
        {
            /* Recieve NPC´s text stored in  m_Engine -> m_NPCText */
            gtk_editable_delete_text (GTK_EDITABLE (rd->npc), 0, -1);
            gtk_text_insert ((GtkText *) rd->npc, rd->npc->style->font,
                &rd->npc->style->black, &rd->npc->style->white,
                rd->engine->npc_text, -1);

            gtk_clist_freeze (GTK_CLIST (rd->player));
            gtk_clist_clear (GTK_CLIST (rd->player));

            /* Check if there is a possible answer at all!
               Else the NPC´s speech will continue. */
            if (rd->engine->player_text.length() == 0)
            {
                text[0] = "... continue ...";
                gtk_clist_append (GTK_CLIST (rd->player), text);
            }

            /* Else display all Answers contained in the Array
               m_pPCText, which has the size  m_pPCText.m_Num */
            else
            {
                player_text = rd->engine->player_text.get_array ();
            
                for (i = 0; i < result; i++)
                {
                    text[0] = player_text[i];
                    gtk_clist_append (GTK_CLIST (rd->player), text);
                }
            }
        
            gtk_clist_thaw (GTK_CLIST (rd->player));
        }
    }
    
    return result;
}
