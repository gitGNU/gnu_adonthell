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

#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "array_tmpl.h"
#include "interpreter.h"

// template class Array<CmdNew*>;
Array<CmdNew> interpreter::callbacks;

// Read a string from the int-buffer: just a convenience function
char* command::strread (s_int32 *buffer, u_int32 &i)
{
    // Hopefully the first number in buffer holds the strings length
    char *tmp = new char[buffer[i++]];

    // copy the string's contents
    memcpy (tmp, buffer+i, buffer[i-1]);

    // let i point after the string
    i += buffer[i-1];

    return tmp;
}

// Constructors
interpreter::interpreter ()
{
    PC = 0;
    cmd_num = 0;
    code = NULL;
    user_data = NULL;

    // Each interpreter needs a unique id; let's just take it's memory adress
    sprintf (id, "%p", this);

    // Now we have to register it with that id
    objects::set (id, (storage *) this);
}

interpreter::interpreter (const char *file, void *data) : user_data (data)
{
    PC = 0;
    load (file);

    // Each interpreter needs a unique id; let's just take it's memory adress
    sprintf (id, "%p", this);

    // Now we have to register it with that id
    objects::set (id, (storage *) this);
}

// Destructor
interpreter::~interpreter ()
{
    u_int32 i;
    
    if (code)
    {
        for (i = 0; i < cmd_num; i++)
            delete code[i];

        delete[] code;
    }

    // Unregister the interpreter, since it's destroyed
    objects::erase (id);
}

// Load a Program
// returns 1 on success, 0 on failure
u_int8 interpreter::load (const char *file)
{
    u_int32 i, j = 0;
    s_int32 *buffer;
    u_int32 length;
    command *cmd;

    // open File for reading
    FILE *dat = fopen (file, "r");
    if (!dat) return 0;
    
    // read program length and reserve temporary space
    fread (&length, sizeof (length), 1, dat);
    buffer = new s_int32[length];

    // Get number of commands in program
    fread (&cmd_num, sizeof (cmd_num), 1, dat);

    // Read bytecode at once into memory
    fread (buffer, sizeof (buffer[0]), length, dat);

    fclose (dat);

    // Create the buffer for the program
    code = new command*[cmd_num];

    // Read commands and their parameters
    // Commands are created by a userdefined callback that
    // has to be registered once for each command
    for (i = 0; i < cmd_num; i++)
    {
        // Call the registered function to create a instance
        // of the Command with type = buffer[j]
        cmd = (*callbacks.get_element (buffer[j]))();

        // Tell the command to which interpreter it belongs
        // This is used to access the interpreters local storage from 
        // within the command
        cmd->interpreter = id;

        // Set the commands type
        cmd->type = buffer[j];
        
        // init the command with its parameters
        // you recieve a pointer to its first argument and should
        // return the number of bytes you read (Failure to do so
        // may even result in a SEGFAULT, so take care!)
        cmd->init (buffer, ++j, user_data);

        // add command to programs code
        code[i] = cmd;
    }

    // Clean up
    delete[] buffer;
    
    return 1;
}

// Executes your program until it reaches a RETURN command
s_int32 interpreter::run ()
{
#ifdef _DEBUG_
    if (PC >= cmd_num) return -1;
#endif

    while (code[PC]->type)
    {
        // Execute all commands except RETURN
        // Since you cannot influence the interpreter directly
        // it'd be a waste to pass on (and assign) a return value.
        // If you need feedback from your Program, use RETURN to
        // stop execution, examine the returned value and call
        // this function again to carry on.
        code[PC++]->run (PC, user_data);

#ifdef _DEBUG_
        if (PC >= cmd_num) return -1;        
#endif
    }

    // In most cases, your program will run from start to finish,
    // but in case it is too big to execute in one go, or requires
    // user input (like the dialogue engine) we increase the Program
    // Counter, so execution can be resumed later. 
    return code[PC++]->run (PC, user_data);
}
