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

#include <gtk/gtk.h>
#include "circle.h"
#include "crcle_interface.h"
#include "../../interpreter.h"

extern int cond_compile (const char*, string&, vector<command*>&);

crcle_dlg::crcle_dlg (Circle *c) : circle (c)
{
    // here we create and display the dialog window
    dlg = create_dlg_node_window (circle, this);

    // We'll work on a copy of the nodes values, in case users
    // cancel their changes
    type = circle->type;
    text = circle->text;
    note = circle->comment;
    cond = circle->conditions;
    vars = circle->variables;
    retval = 0;
}

// Apply changes to Circle
void crcle_dlg::on_ok ()
{
    string error;
    vector<command*> code;

    // Look if code contains errors
    if (cond != "")
    {
        cond_compile (cond.c_str (), error, code);
    }
    
    retval = 1;

    circle->type = type;
    circle->text = text;
    circle->comment = note;
    circle->conditions = cond;
    circle->variables = vars;
}

// Text has changed
void crcle_dlg::on_text (char *t)
{
    text = t;
}

// Note has changed
void crcle_dlg::on_annotation (char *t)
{
    note = t;
}

// Conditions have changed
void crcle_dlg::on_condition (char *t)
{
    cond = t;
}

// Variables have changed
void crcle_dlg::on_variable (char *t)
{
    vars = t;
}

// Type have changed
void crcle_dlg::on_type (u_int8 t)
{
    type = t;
}