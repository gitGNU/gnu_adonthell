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

#define _DLGENGINE_

#include <iostream.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "gfx.h"
#include "array_tmpl.h"
#include "interpreter.h"
#include "commands.h"
#include "window.h"
#include "keyboard.h"
#include "dialog.h"

// Init the dialogue engine
dialog_engine::dialog_engine ()
{  
    // Init interpreter
    interpreter::callbacks.clear ();
    init_interpreter ();

    // Read strings
    FILE *str = fopen ("dialog/tomas_hterin.str", "r");
    u_int32 count;

    fread (&count, sizeof(count), 1, str);

    if (dialog::offset != NULL) delete[] dialog::offset;
    if (dialog::length != NULL) delete[] dialog::length;
    
    dialog::offset = new s_int32[count];
    dialog::length = new s_int32[count];

    fread (dialog::offset, sizeof(dialog::offset[0]), count, str);
    fread (dialog::length, sizeof(dialog::length[0]), count, str);

    fclose (str);
    
    // For the test, the only dialogue is created here
    dlg = new dialog;
    dlg->answer = 0;
    dlg->text_file = "dialog/tomas_hterin.str";

    engine = new interpreter ("dialog/tomas_hterin.dat", dlg);
}

void dialog_engine::update_keyboard ()
{
    if (keyboard::is_pushed (97)) dlg->answer = 0;
    if (keyboard::is_pushed (98)) dlg->answer = 1;
    if (keyboard::is_pushed (99)) dlg->answer = 2;
    if (keyboard::is_pushed (100)) dlg->answer = 3;
}

void dialog_engine::update (window *win)
{
    if (dlg->answer == 0xFFFF) return;

    win->set_text (run ());
    dlg->answer = 0xFFFF;
}

char *dialog_engine::run ()
{
    u_int32 i;
    s_int32 result;
    char *abc[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
    char *str = NULL;

    result = engine->run ();

    switch (result)
    {
        case -1:
        {
            return "Interpreter returned error";
        }

        case 0:
        {
            // the end
            return NULL;
        }
        
        case 1:
        {
            // Player part 
            str = strdup (dlg->npc_text);
            for (i = 0; i < dlg->player_text.length () ; i++)
            {
                str = strcat (str, "   ");
                str = strcat (str, abc[i]);
                str = strcat (str, "> ");
                str = strcat (str, dlg->player_text.get_element (i));
            }

            return str;
        }

        case 2:
        {
            /* NPC only */
            return dlg->npc_text;
        }
    }

    return str;   
}