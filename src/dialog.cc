/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "types.h"
#include "dialog_cmd.h"
#include "dlg_io.h"
#include "array_tmpl.h"
#include "dialog.h"

dialog::dialog ()
{
    PC = 0;
    str_num = 0;
}

// free all memory allocated by the Dialogue Engine
dialog::~dialog ()
{
    u_int32 i;
    
    for (i = 0; i < str_num; i++)
        delete[] dlg_text[i];

    for (i = 0; i < cmd_num; i++)
        delete[] dlg_code[i];
        
    delete[] dlg_text;
    delete[] dlg_code;
}

// Open & Load a Dialogue provided in a single File
// (Someday all Dialogues shall be in one file)
u_int8
dialog::init_dialog (char *file, u_int32 start)
{
    char *data_file = new char[strlen (file) + 5];
    char *text_file = new char[strlen (file) + 5];

    sprintf (data_file, "%s.dat", file);
    sprintf (text_file, "%s.str", file);

    if (!read_dialog_data (fopen (data_file, "r")))
    {
        sprintf (error_text, "Can´t open file %s", data_file);
        return 0;
    }
    
    if (!read_dialog_text (fopen (text_file, "r")))
    {
        sprintf (error_text, "Can´t open file %s", text_file);
        return 0;
    }

    // That is the DISPLAY cmd we start with
    cur_cmds.add_element (dlg_code[str_num + start]);

    delete[] data_file;
    delete[] text_file;
    
    return 1;
}

// Read the Dialogue data from file
u_int8
dialog::read_dialog_data (FILE * dat)
{
    u_int32 i, j = 0;
    s_int32 *buffer;
    u_int32 length;
    dialog_cmd *cmd;

    if (!dat) return 0;
    
    // read Dialogue length
    length = h2d (4, dat);
    buffer = new s_int32[length];

    // Get number of commands in Dialogue
    cmd_num = h2d (4, dat);

    // Read dialogue
    ri (buffer, length, 4, dat);

    fclose (dat);

    dlg_code = new dialog_cmd*[cmd_num];

    // Read commands and their parameters
    for (i = 0; i < cmd_num; i++, j++)
    {
        cmd = new dialog_cmd;
        cmd->type = buffer[j];

        switch (buffer[j])
        {
        case CLEAR:
        case DISPLAY:
        case END:
            {
                cmd->text = -1;
                cmd->new_pc = i;
                break;
            }
        case IMPORT:
            {
                cmd->text = buffer[++j];
                cmd->new_pc = i;
                str_num++;
                break;
            }
        case NPCTEXT:
            {
                cmd->text = buffer[++j];
                cmd->new_pc = i;
                break;
            }
        case PTEXT:
        case SNPCTEXT:  
            {
                cmd->text = buffer[++j];
                cmd->new_pc = buffer[++j];
                break;
            }
        case JUMP:
            {
                cmd->new_pc = buffer[++j];
                break;
            }
        default:
            {
                delete[] buffer;
                return 0;
            }
        }

        dlg_code[i] = cmd;
    }

    delete[] buffer;
    
    return 1;
}

// Read the Dialogue text from file 
u_int8
dialog::read_dialog_text (FILE * str)
{
    if (!str) return 0;

    u_int32 i;
    u_int32 count = h2d (4, str);
        
    s_int32 *offset = new s_int32[count];
    s_int32 *length = new s_int32[count];

    dialog_cmd *cmd;
    char *text;

    ri (offset, count, 4, str);
    ri (length, count, 4, str);
    
    dlg_text = new char *[str_num];

    for (i = 0; i < str_num; i++)
    {
        cmd = dlg_code[PC++];

        text = new char[length[cmd->text] + 1];

        fseek (str, offset[cmd->text], SEEK_SET);
        fgets (text, length[cmd->text] + 1, str);

        dlg_text[i] = text;
    }

    delete[] offset;
    delete[] length;
    fclose (str);
    
    return 1;
}

// The main Dialogue Engine loop
s_int32
dialog::run_dialog (u_int32 index)
{
    dialog_cmd *cmd;

    // check for valid answer chosen by the player
    if (index >= cur_cmds.length() || index < 0)
    {
        sprintf (error_text, "%i is no valid answer (Index out of range 0 - %i)!", (int)index, (int)cur_cmds.length() - 1);
        return -2;
    }

    cmd = cur_cmds.get_element (index);

    // Set Program Counter according to selected answer
    PC = cmd->new_pc;

    // Add String to array of used strings
    used_text.add_element (cmd->text);

    // execute commands until we get to DISPLAY or END
    for (;;)
    {
        // check for valid Program Counter
        if (PC >= cmd_num || PC < 0)
        {
            sprintf (error_text, "Program Counter (%i) out of range 0 - %i!", (int)PC, (int)cmd_num - 1);
            return -2;
        }

        cmd = dlg_code[PC];

        switch (cmd->type)
        {
        // empties all buffers
        case CLEAR:            
            {
                player_text.clear ();
                cur_cmds.clear ();
                npc_text = NULL;

                PC++;
                break;
            }
            
        // display NPC´s text and players possible reactions
        case DISPLAY:
            {
                return player_text.length ();
            }
            
        // End of Dialogue
        case END:             
            {
                return -1;
            }
            
        // Add NPC´s text to buffer          
        case NPCTEXT:
            {
                npc_text = dlg_text[cmd->text];

                PC++;
                break;
            }

        // Add players text to buffer if never chosen before          
        case PTEXT:
            {
                if (!used_text.search_element (cmd->text))
                {
                    player_text.add_element (dlg_text[cmd->text]);
                    cur_cmds.add_element (cmd);
                }

                PC++;
                break;
            }

        // Add NPC´s text to buffer
        case SNPCTEXT:
            {
                npc_text = dlg_text[cmd->text];
                cur_cmds.add_element (cmd);

                PC++;
                break;
            }
            
        case JUMP:
            {
                PC = cmd->new_pc;
                break;
            }

        default:
            {
                sprintf (error_text, "Encountered unknown command %i", (int)cmd->type);
                return -2;
            }
        }
    }

    return -2;
}
