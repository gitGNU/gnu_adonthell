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
#include "image.h"

//#include "window.h"
#include "input.h"
#include "dialog.h"

// Init the dialogue engine
dialog_engine::dialog_engine ()
{
	// For the test, the only dialogue is created here
	dlg = new dialog;
	dlg->answer = 0;

	// Import module
	dlg->class_object = import_module( "dialog/tomas_hterin.py" );
}

void dialog_engine::update_keyboard ()
{
	static u_int16 lastkeypressed=0;
	
	if(input::is_pushed(lastkeypressed)) return;
	lastkeypressed=0;

	if (input::is_pushed (97)) 
	{dlg->answer = 0; lastkeypressed=97;}
	if (input::is_pushed (98)) 
	{dlg->answer = 1; lastkeypressed=98;}
	if (input::is_pushed (99)) 
	{dlg->answer = 2; lastkeypressed=99;}
	if (input::is_pushed (100)) 
	{dlg->answer = 3; lastkeypressed=100;}
}

void dialog_engine::update (window &win)
{
/*
    if (dlg->answer == 0xFFFF) return;

    run (win);
    dlg->answer = 0xFFFF;
*/
}

void dialog_engine::run (window &win)
{
/*
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
            // NPC only
            str = dlg->npc_text;
        }
    }
    win.set_text (str.c_str ());   
*/
}

void dialog_engine::insert_plugin ()
{
    // We have to do the following:
    // - Add the plugin to the end of the main dialogue
    // - Insert a JMP <length_of_main_dlg - 2> after the main dlg's IMPORT
    //   (That means, the plugged in subjects appear before the main ones,
    //   but else, we'd have to check the main dlg for JMP's to the first
    //   block)
    // - Offset the TEXT's string_id to point after the main strings
    // - Remove the RETURN 1 of the first plugin block (fpb)
    // - Replace the CLEAR of the fpb with JMP <-length_of_main_dlg + 2>
    // - Replace all RETURN 0's of the plugin with JMP's to the first main block
}
