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

#ifndef __GENERIC_CMDS_H__
#define __GENERIC_CMDS_H__

#include <string>
#include <fstream.h>

#include "types.h"
#include "storage.h"
#include "commands.h"
#include "interpreter.h"


// Command for stopping/pausing the interpreter
class return_cmd : public command
{
public:
    return_cmd () { type = RETURN; }
    return_cmd (s_int32 r) : retval (r) { type = RETURN; }
    
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        retval = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data)
    {
        return retval;
    }

    void write (FILE *out)
    {
        fwrite (&type, sizeof(type), 1, out);
        fwrite (&retval, sizeof(retval), 1, out);
    }
    void ascii (ofstream &out)
    {
        out << "RETURN  " << retval;
    }
    
private:
    s_int32 retval;
};


// Jump to another line in the script
class jmp_cmd : public command, public offset_cmd
{
public:
    jmp_cmd () { type = JMP; }
    jmp_cmd (s_int32 o)
    {
        type = JMP;
        offset = o;
    }
    
    void init (s_int32*, u_int32&, void*);
    s_int32 run (u_int32&, void*);

    void write (FILE*);             // Write the command as script code to file
    void ascii (ofstream&);         // Write cmd as human readable test to file
};


// Branch to another line in the script depending on the value of condition
class branch_cmd : public command, public offset_cmd
{
public:
    branch_cmd () { condition = NULL; }
    branch_cmd (s_int32 o, const char* c)
    {
        type = BRANCH;
        offset = o;
        condition = strdup (c + strcspn (c, ".") + 1);
    }
    virtual ~branch_cmd () { if (condition) delete[] condition; }

    void init (s_int32*, u_int32&, void*);
    s_int32 run (u_int32&, void*);

    void write (FILE*);             // Write the command as script code to file
    void ascii (ofstream&);         // Write cmd as human readable test to file

private:
    char* condition;
};


// base class for commands having two parameters, namely
// let, add, mul, sub, div, eq, neq, lt, leq, gt, geq, and, or
class binary_cmd : public command
{
public:
    binary_cmd ();
    binary_cmd (u_int32, string, string, string);
    virtual ~binary_cmd ();

    s_int32 run (u_int32&, void*) { return 1; }
    void init (s_int32*, u_int32&, void*);
    void write (FILE*);             // Write the command as script code to file
    void ascii (ofstream&);         // Write cmd as human readable test to file

protected:
    char *target;                   // Operations target
    char *target_location;          // Location of the target

    char *c_param1;                 // Variable parameter 1
    char *c_param2;                 // and 2 
    char *param1_location;          // and their
    char *param2_location;          // locations
    
    s_int32 i_param1;               // Immeditate parameter 1
    s_int32 i_param2;               // and 2

    u_int32 ptype;                  // Parameter types (see enum below)
    
    inline void read_game_state (); // Read variable parameters at runtime

    enum                            // Types of the parameters
    {
        TARGET_GLOBAL = 1,          // Target is the game_state storage
        TARGET_LOCAL  = 2,          // Target is the interpreters local storage
        PARAM1_GLOBAL = 4,          // Parameter 1 is inside the game_state storage
        PARAM1_LOCAL =  8,          // Param 1 inside the int. local storage
        PARAM1_NUMBER = 16,         // Param 1 is an integer
        PARAM2_GLOBAL = 32,         // game_state storage
        PARAM2_LOCAL =  64,         // interpreter storage
        PARAM2_NUMBER = 128         // integer
    };
};

// target = param1 (here param2 is unused)
class let_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 + param2 
class add_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 - param2 
class sub_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 * param2 
class mul_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = int (param1 / param2) 
class div_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 == param2 
class eq_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 != param2 
class neq_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 < param2 
class lt_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 <= param2 
class leq_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 > param2 
class gt_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 >= param2 
class geq_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 && param2 
class and_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

// target = param1 || param2 
class or_cmd : public binary_cmd
{
public:
    s_int32 run (u_int32&, void*);
};

#endif // __GENERIC_CMDS_H__