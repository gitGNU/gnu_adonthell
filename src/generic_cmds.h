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

#include "types.h"
#include "storage.h"
#include "interpreter.h"

// base class for commands having two parameters, namely
// add, mul, sub, div, eq, neq, lt, leq, gt, geq, and, or
class binary_cmd : public command
{
public:
    binary_cmd ();
    virtual ~binary_cmd ();
    
    void init (s_int32*, u_int32&, void*);

protected:
    char *target;                   // Operations target
    char *target_location;          // Location of the target

    char *c_param1;                 // Variable parameter 1
    char *c_param2;                 // and 2 
    char *param1_location;          // and their
    char *param2_location;          // locations
    
    s_int32 i_param1;               // Immeditate parameter 1
    s_int32 i_param2;               // and 2

    void init (s_int32*, u_int32&, char*, s_int32&);
    inline void read_game_state (); // Read variable parameters at runtime
};

// target = param1 + param2 
class add_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 - param2 
class sub_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 * param2 
class mul_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = int (param1 / param2) 
class div_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 == param2 
class eq_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 != param2 
class neq_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 < param2 
class lt_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 <= param2 
class leq_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 > param2 
class gt_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 >= param2 
class geq_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 && param2 
class and_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

// target = param1 || param2 
class or_cmd : public binary_cmd
{
public:
    inline s_int32 run (u_int32&, void*);
};

#endif // __GENERIC_CMDS_H__