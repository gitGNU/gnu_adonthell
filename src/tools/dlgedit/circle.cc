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
#include "error_dlg.h"

extern int cond_compile (const char*, string&, vector<command*>&);
extern int vars_compile (const char*, string&, vector<command*>&);

crcle_dlg::crcle_dlg (Circle *c, MainFrame *w) : circle (c), wnd (w)
{
    gchar **actions = g_strsplit (circle->actions.c_str (), "|", 2);
    
    // here we create and display the dialog window
    dlg = create_dlg_node_window (circle, this);

    // We'll work on a copy of the nodes values, in case users
    // cancel their changes
    type = circle->type;
    text = circle->text;
    note = circle->comment;
    cond = circle->conditions;
    vars = circle->variables;
    loop = actions[0][0];
    combat = actions[1][0]; 
    change_dlg = 0;

    g_strfreev (actions);
    
    retval = 0;
}

// Apply changes to Circle
int crcle_dlg::on_ok ()
{
/*
    string error, t1, t2;
    vector<command*> code;

    // Look if code contains errors
    if (cond != "") cond_compile (cond.c_str (), t1, code);
    if (t1 != "") error = string ("Error(s) in Condition code:\n") + t1 + "\n\n";  
    if (vars != "") vars_compile (vars.c_str (), t2, code);
    if (t2 != "") error += string ("Error(s) in Variables code:\n") + t2;

    // compilation errors found
    if (error != "")
    {
        // Either create a new error_dlg window, or bring it to front
        if (!wnd->err) wnd->err = new error_dlg (wnd);
        else wnd->err->to_front ();

        // Display the error message
        wnd->err->display (error.c_str ());
            
        return 0;
    }
    else if (wnd->err) wnd->err->display ("Compilation successfull :-)");
*/

    // Indicate that user hit the OK button
    retval = 1;

    // Apply changes to the circle
    circle->type = type;
    circle->text = text;
    circle->comment = note;
    circle->conditions = cond;
    circle->variables = vars;
    circle->actions = string(1, loop) + string("|") + string(1, combat) + string("|0");
 
    if (type == PLAYER) circle->character = 0;
    else circle->character = 1; // update this line once multiple NPCs are supported

    return 1;
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

// Combat updated
void crcle_dlg::on_combat (u_int8 c)
{
    combat = c;
}

// Loop updated
void crcle_dlg::on_loop (u_int8 l)
{
    loop = l;
}

// dialogue/plugin radiobuttons changed
void crcle_dlg::on_change_dlg (u_int8 c)
{
    change_dlg = c;
}

u_int16 crcle_dlg::get_cur_dlg ()
{
    return change_dlg;
}