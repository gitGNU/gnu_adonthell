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

#ifndef __DIALOG_CMDS_H__
#define __DIALOG_CMDS_H__

#include <iostream.h>
#include <fstream>
#include "commands.h"
#include "interpreter.h"

// Reads the strings needed for the dialogue from the string file
class import_cmd : public command
{
public:
    import_cmd () { type = IMPORT; }
    import_cmd (u_int32 *t, u_int32 s) : text(t), sz(s)  { type = IMPORT; }
    virtual ~import_cmd () { delete[] text; }
    
    void init (s_int32*, u_int32&, void *);
    s_int32 run (u_int32&, void *);

    void write (FILE*);
    void ascii (ofstream&);
    
private:
    u_int32 *text;
    u_int32 sz;
};


// Continues the dialogue after the player made his choice
class clear_cmd : public command
{
public:
    clear_cmd () { type = CLEAR; }

    void init (s_int32 *buffer, u_int32 &i, void *data) { }
    s_int32 run (u_int32 &PC, void *data);

    void write (FILE*);
    void ascii (ofstream&);   
};


// sets the speakers picture (and name)
class speaker_cmd : public command
{
public:
    speaker_cmd () { type = SPEAKER; }
    speaker_cmd (u_int32 s, u_int32 m);

    void init (s_int32 *buffer, u_int32 &i, void *data);
    s_int32 run (u_int32 &PC, void *data);

    void write (FILE*);
    void ascii (ofstream&);

private:
    u_int32 speaker;
    u_int32 mood;
};

// adds a line to the text arrays
class text_cmd : public command, public offset_cmd
{
public:
    text_cmd () { type = TEXT; }
    text_cmd (u_int32 t, s_int32 p) : text(t)
    { 
        type = TEXT;
        offset = p;
    }
    
    void init (s_int32*, u_int32&, void*);
    s_int32 run (u_int32&, void*);

    void write (FILE*);
    void ascii (ofstream&);   

private:
    u_int32 text;               // Id of the dialogue text
};

#endif // __DIALOG_CMDS_H__