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

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <map>

#include "types.h"
#include "storage.h"


// Base class for commands to be used with the interpreter
class command
{
public:
    // pure virtual method for initializing your command
    virtual void init (s_int32*, u_int32&, void*) = 0;

    // contains the commands code
    virtual s_int32 run (u_int32&, void*) = 0;

    // write the command to a script file
    virtual void write (FILE *f) { }

    // write the command to a human readable file
    virtual void ascii (FILE *f) { }
    
    // type of your command
    u_int32 type;

    // ID of the interpreter that spawns the command; used for access to the 
    // interpreter's local storage
    char *interpreter;

protected:
    // Extract a string from the buffer
    char* strread (s_int32*, u_int32&); 
};


// Pointer to a function returning a newly allocated command
typedef command* (*CmdNew)();


// Interpreter for running various scripts
class interpreter : public storage
{
friend command run (void*);

public:
    interpreter ();
    interpreter (const char*, void*);
    ~interpreter ();

    // Load a program
    u_int8 load (const char*);

    // Continue with program execution
    s_int32 run (); 

    // Object to manipulate  
    void *user_data;

    // Array with pointers to functions that return a new'd command
    static map<int, CmdNew> callbacks;
   
private:
    // Program Counter
    u_int32 PC;                     

    // Program
    command **code;

    // Length of Program
    u_int32 cmd_num;

    // Interpreters unique id, namely it's memory address
    char id[11];
};

#define NEW_CMD(cmd) command* new_ ## cmd () { return (command*) new cmd; }
#define REGISTER_CMD(type,cmd)\
    interpreter::callbacks[type] = (CmdNew)&new_ ## cmd;

#endif // __INTERPRETER_H__