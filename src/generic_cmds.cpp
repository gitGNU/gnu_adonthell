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
#include "yarg.h"
#include <string.h>

binary_cmd::binary_cmd ()
{
    c_param1 = c_param2 = target = NULL;
    param1_location = param2_location = target_location = NULL;
    i_param1 = i_param2 = 1;
    ptype = 0;
}

binary_cmd::binary_cmd (u_int32 tp, string st, string sp1, string sp2)
{
    int i;
    const char *t = st.c_str ();
    const char *p1 = sp1.c_str ();
    const char *p2 = sp2.c_str ();
    
    // First give all members a sane state
    c_param1 = c_param2 = target = NULL;
    param1_location = param2_location = target_location = NULL;
    i_param1 = i_param2 = 1;
    ptype = 0;

    type = tp;

    // Now look at the parameters and see what we've got
    if (strchr (t, '.'))
    {
        target_location = new char[(i = strcspn (t, "."))+1];
        strncpy (target_location, t, i);
        target_location[i] = '\0';

        if (!strcmp (target_location, "local")) ptype += TARGET_LOCAL;
        target = strdup (t + strcspn (t, ".") + 1);
    }
    else
    {
        target_location = strdup ("game_state");
        target = strdup (t);
        ptype += TARGET_GLOBAL;
    }

    if (strchr (p1, '.'))
    {
        param1_location = new char[(i = strcspn (p1, "."))+1];
        strncpy (param1_location, p1, i);
        param1_location[i] = '\0';

        if (!strcmp (param1_location, "local")) ptype += PARAM1_LOCAL;
        c_param1 = strdup (p1 + strcspn (p1, ".") + 1);
    }
    else
    {
        if (isdigit (p1[0]) || p1[0] == '-')
        {
            i_param1 = atoi (p1);
            ptype += PARAM1_NUMBER;
        }
        else
        {
            param1_location = strdup ("game_state");
            c_param1 = strdup (p1);
            ptype += PARAM1_GLOBAL;
        }
    }
    
    if (strchr (p2, '.'))
    {
        param2_location = new char[(i = strcspn (p2, "."))+1];
        strncpy (param2_location, p2, i);
        param2_location[i] = '\0';

        if (!strcmp (param2_location, "local")) ptype += PARAM2_LOCAL;
        c_param2 = strdup (p2 + strcspn (p2, ".") + 1);
    }
    else
    {
        if (isdigit (p2[0]) || p2[0] == '-')
        {
            i_param2 = atoi (p2);
            ptype += PARAM2_NUMBER;
        }
        else
        {
            param2_location = strdup ("game_state");
            c_param2 = strdup (p2);
            ptype += PARAM2_GLOBAL;
        }
    }
}

binary_cmd::~binary_cmd ()
{
    if (param1_location != NULL && !(ptype & PARAM1_LOCAL)) delete param1_location;
    if (param2_location != NULL && !(ptype & PARAM2_LOCAL)) delete param2_location;
    if (target_location != NULL) delete target_location;
    if (c_param1 != NULL) delete c_param1;
    if (c_param2 != NULL) delete c_param2;
    if (target != NULL) delete target;
}

// Read the commands arguments from the buffer
void binary_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    ptype = buffer[i++];

    // read target
    target = strread (buffer, i);
    // ... and target location
    if (ptype & TARGET_GLOBAL) target_location = strdup ("game_state");
    else if (ptype & TARGET_LOCAL) target_location = interpreter;
    else target_location = strread (buffer, i);

    // parameter 1
    if (ptype & PARAM1_NUMBER) i_param1 = buffer[i++];
    else
    {
        c_param1 = strread (buffer, i); 
        if (ptype & PARAM1_GLOBAL) param1_location = strdup ("game_state");
        else if (ptype & PARAM1_LOCAL) param1_location = interpreter;
        else param1_location = strread (buffer, i);
    }

    // parameter 2
    if (ptype & PARAM2_NUMBER) i_param2 = buffer[i++];
    else
    {
        c_param2 = strread (buffer, i); 
        if (ptype & PARAM2_GLOBAL) param2_location = strdup ("game_state");
        else if (ptype & PARAM2_LOCAL) param2_location = interpreter;
        else param2_location = strread (buffer, i);
    }
}

// write the commands code to file
void binary_cmd::write (FILE *out)
{
    u_int32 l;

    fwrite (&type, sizeof(type), 1, out);
    fwrite (&ptype, sizeof(ptype), 1, out);

    // Target
    l = strlen (target);
    fwrite (&l, sizeof (l), 1, out);
    fwrite (target, l, 1, out);
    for (; l%4; l++) fputc (0, out);
    
    if (!(ptype & TARGET_LOCAL) && !(ptype & TARGET_GLOBAL)) 
    {
        l = strlen (target_location);
        fwrite (&l, sizeof (l), 1, out);
        fwrite (target_location, l, 1, out);
        for (; l%4; l++) fputc (0, out);
    }
    
    // Param 1
    if (ptype & PARAM1_NUMBER) fwrite (&i_param1, sizeof(i_param1), 1, out);
    else
    {
        l = strlen (c_param1);
        fwrite (&l, sizeof (l), 1, out);
        fwrite (c_param1, l, 1, out);
        for (; l%4; l++) fputc (0, out);

        if (!(ptype & PARAM1_LOCAL) && !(ptype & PARAM1_GLOBAL))
        {
            l = strlen (param1_location);
            fwrite (&l, sizeof (l), 1, out);
            fwrite (param1_location, l, 1, out);    
            for (; l%4; l++) fputc (0, out);
        }
    }

    // Param 2
    if (ptype & PARAM2_NUMBER) fwrite (&i_param2, sizeof(i_param2), 1, out);
    else
    {
        l = strlen (c_param2);
        fwrite (&l, sizeof (l), 1, out);
        fwrite (c_param2, l, 1, out);
        for (; l%4; l++) fputc (0, out);

        if (!(ptype & PARAM2_LOCAL) && !(ptype & PARAM2_GLOBAL))
        {
            l = strlen (param2_location);
            fwrite (&l, sizeof (l), 1, out);
            fwrite (param2_location, l, 1, out);    
            for (; l%4; l++) fputc (0, out);
        } 
    }
} 

// write command in human readable form
void binary_cmd::ascii (ofstream &out)
{
    char* ops[] = { "LET ", "ADD ", "SUB ", "MUL ", "DIV ", "EQ  ", "NEQ ",
                    "LT  ", "LEQ ", "GT  ", "GEQ ", "AND ", "OR  ", "RAND" };

    out << ops[type - LET] << "    ";

    if (c_param1 != NULL) out << param1_location << "." << c_param1 << " ";
    else out << i_param1 << " ";

    if (type != LET)
    {
        if (c_param2 != NULL) out << param2_location << "." << c_param2 << " ";
        else out << i_param2 << " ";
    }
    
    out << target_location << "." << target;
}

// if we have variable arguments, read their value
void binary_cmd::read_game_state ()
{
    if (c_param1 != NULL) i_param1 = objects::get (param1_location)->get (c_param1);
    if (c_param2 != NULL) i_param2 = objects::get (param2_location)->get (c_param2);
}

// target = param1
s_int32 let_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();
    objects::get (target_location)->set (target, i_param1);
    return 1;
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

// target = random number out of interval (param1, param2)
s_int32 rand_cmd::run (u_int32 &PC, void *data)
{
    read_game_state ();

    yarg rand (" ", i_param1, i_param2);
    rand.randomize ();

    objects::get (target_location)->set (target, rand.get (5));
    return 1;
}

// Read the commands arguments from the buffer
void jmp_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    offset = buffer[i++];
}

// Jump to another line in the script
s_int32 jmp_cmd::run (u_int32 &PC, void *data)
{
    PC += offset;

    return 1;
}

void jmp_cmd::write (FILE* out)
{
    fwrite (&type, sizeof(type), 1, out);
    fwrite (&offset, sizeof(offset), 1, out);
}

void jmp_cmd::ascii (ofstream &out)
{
    out << "JMP     " << offset+1;
}


// Read the commands arguments from the buffer
void branch_cmd::init (s_int32 *buffer, u_int32 &i, void *data)
{
    offset = buffer[i++];
    condition = strread (buffer, i);
}

// Branch to another line in the script
s_int32 branch_cmd::run (u_int32 &PC, void *data)
{
    if (!objects::get (interpreter)->get (condition)) PC += offset;

    return 1;
}

void branch_cmd::write (FILE* out)
{
    int l = strlen (condition);
    
    fwrite (&type, sizeof(type), 1, out);
    fwrite (&offset, sizeof(offset), 1, out);
    fwrite (&l, sizeof (l), 1, out);
    fwrite (condition, sizeof (condition[0]), l, out);
    for (; l%4; l++) fputc (0, out);
}

void branch_cmd::ascii (ofstream &out)
{
    out << "BRANCH  local." << condition << " " << offset+1;
}