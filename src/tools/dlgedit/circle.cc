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
#include <algorithm>

#include "circle.h"
#include "crcle_interface.h"

extern gchar *get_option (GtkOptionMenu*);

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
    unsigned int pos = 0;
    
    // Indicate that user hit the OK button
    retval = 1;

    // Make text safe:
    // replace linebreaks with space
    replace (text.begin (), text.end (), '\n', ' ');

    // escape quotes
    while ((pos = text.find ("\"", pos)) != text.npos)
    {
        if (pos > 0) {
            if (text[pos-1] != '\\') text.insert (pos, "\\");
        }
        else text.insert (pos, "\\");

        pos++;
    }

    if (!strncmp (cond.c_str (), "else if", 7))
        cond.replace (0, 7, "elif");
    
    // Apply changes to the circle
    circle->type = type;
    circle->text = text;
    circle->comment = note;
    circle->conditions = cond;
    circle->variables = vars;
    circle->actions = string(1, loop) + string("|") + string(1, combat) + string("|0");
    circle->character = get_option (GTK_OPTION_MENU (npc_menu));

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
