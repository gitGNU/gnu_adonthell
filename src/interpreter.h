/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "array_tmpl.h"

class command
{
public:
    // pure virtual method for initializing your command
    virtual void init (s_int32*, u_int32&, void*) = 0;

    // contains the commands code
    virtual s_int32 run (u_int32&, void*) = 0;

    // type of your command
    u_int32 type;
};

typedef command* (*CmdNew)();

class interpreter
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

    // Array with pointers to functions that return a newly allocated
    // command
    static Array<CmdNew*> callbacks;
   
private:
    // Program Counter
    u_int32 PC;                     

    // Program
    command **code;

    // Length of Program
    u_int32 cmd_num;
};

#define NEW_CMD(cmd) public: static command* _new () { return (command*) new cmd; } private:
#define REGISTER_CMD(type,cmd)\
    if ((u_int32)type != interpreter::callbacks.length())\
    {\
        cout << "Error registering command " << (u_int32) type << endl;\
        return;\
    }\
    interpreter::callbacks.add_element ((CmdNew*)&cmd::_new);
