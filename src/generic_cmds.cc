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

#include "generic_cmds.h"
#include "storage.h"

binary_cmd::binary_cmd ()
{
    c_param1 = c_param2 = target = NULL;
    param1_location = param2_location = target_location = NULL;
    i_param1 = i_param2 = 1;
}

binary_cmd::~binary_cmd ()
{
    if (param1_location != NULL) delete param1_location;
    if (param2_location != NULL) delete param2_location;
    if (target_location != NULL) delete target_location;
    if (c_param1 != NULL) delete c_param1;
    if (c_param2 != NULL) delete c_param2;
    if (target != NULL) delete target;
}

void binary_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    s_int32 tmp;
    
    init (buffer, i, param1_location, tmp);
    init (buffer, i, c_param1, i_param1);
    init (buffer, i, param2_location, tmp);
    init (buffer, i, c_param2, i_param2);
    init (buffer, i, target_location, tmp);
    init (buffer, i, target, tmp);
}

// Init all the variables (needs to be updated)
void binary_cmd::init (s_int32 *buffer, u_int32 &i, char *c_param, s_int32 &i_param)
{
    u_int32 length;

    if ((length = buffer[i++]) == 0)
    {
        if (i_param) i_param = buffer[i++];
        else
        {
            c_param = new char[11];
            strcpy (c_param, interpreter);
        }
    }
    else
    {
        c_param = new char[length];
        memcpy (c_param, buffer + i, length);
        i += length / sizeof (s_int32);
    }
}

// if we have variable arguments, read their value
void binary_cmd::read_game_state ()
{
    if (c_param1 != NULL) i_param1 = objects::get (param1_location)->get (c_param1);
    if (c_param2 != NULL) i_param2 = objects::get (param2_location)->get (c_param2);
}

// target = param1 + param2 
s_int32 add_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 + i_param2);
    return 1;
}

// target = param1 - param2 
s_int32 sub_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 - i_param2);
    return 1;
}

// target = param1 * param2 
s_int32 mul_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 * i_param2);
    return 1;
}

// target = int (param1 / param2)
s_int32 div_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 / i_param2);
    return 1;
}

// target = param1 == param2 
s_int32 eq_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 == i_param2);
    return 1;
}

// target = param1 != param2 
s_int32 neq_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 != i_param2);
    return 1;
}

// target = param1 < param2 
s_int32 lt_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 < i_param2);
    return 1;
}

// target = param1 <= param2 
s_int32 leq_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 <= i_param2);
    return 1;
}

// target = param1 > param2 
s_int32 gt_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 > i_param2);
    return 1;
}

// target = param1 >= param2 
s_int32 geq_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 >= i_param2);
    return 1;
}

// target = param1 && param2 
s_int32 and_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 && i_param2);
    return 1;
}

// target = param1 || param2 
s_int32 or_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1 || i_param2);
    return 1;
}