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

class return_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        retval = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data)
    {
        return retval;
    }

private:
    s_int32 retval;
};

class import_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
};

class clear_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data) 
    { 
        return;
    }
    s_int32 run (u_int32 &PC, void *data);
};

class npctext_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
};

class ptext_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
        new_pc = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
    u_int32 new_pc;
};

class snpctext_cmd : public command
{
public:
    void init (s_int32 *buffer, u_int32 &i, void *data)
    {
        text = buffer[i++];
        new_pc = buffer[i++];
    }
    s_int32 run (u_int32 &PC, void *data);

private:
    u_int32 text;
    u_int32 new_pc;
};
