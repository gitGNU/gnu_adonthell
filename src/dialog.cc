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

#include <stdio.h>
#include <string>

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
  static u_int16 lastkeypressed=0;
  if(keyboard::is_pushed(lastkeypressed)) return;
  lastkeypressed=0;
  if (keyboard::is_pushed (97)) 
    {dlg->answer = 0; lastkeypressed=97;}
  if (keyboard::is_pushed (98)) 
    {dlg->answer = 1; lastkeypressed=98;}
  if (keyboard::is_pushed (99)) 
    {dlg->answer = 2; lastkeypressed=99;}
  if (keyboard::is_pushed (100)) 
    {dlg->answer = 3; lastkeypressed=100;}
}

void dialog_engine::update (window &win)
{
    if (dlg->answer == 0xFFFF) return;

    run (win);
    dlg->answer = 0xFFFF;
}

void dialog_engine::run (window &win)
{
    u_int32 i;
    s_int32 result;
    char *abc[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
    string str;

    result = engine->run ();

    switch (result)
    {
        case -1:
        {
            str = "Interpreter returned error";
            break;
        }

        case 0:
        {
            // the end
            str = "";
            break;
        }
        
        case 1:
        {
            // Player part 
            str = dlg->npc_text;
            
            for (i = 0; i < dlg->player_text.length () ; i++)
            {
                str += " -- ";
                str += abc[i];
                str += "> ";
                str += dlg->player_text.get_element (i);
            }
            
            break;
        }

        case 2:
        {
            /* NPC only */
            str = dlg->npc_text;
        }
    }
    win.set_text (str.c_str ());   
}
