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

#include <iostream.h>

#include "preset.h"
#include "pset_interface.h"
#include "../../interpreter.h"

extern int vars_compile (const char*, string&, vector<command*>&);

preset_dlg::preset_dlg (string &v) : vars(v)
{
    dlg = create_preset_dlg (this);
}


void preset_dlg::on_ok (char *v)
{
    string error;
    vector<command*> code;

    vars = v;

    if (vars != "")
    {
        vars_compile (v, error, code);
    }

    // test
    ofstream out ("code.txt");
    out << "\nError:\n" << error << "\n\nCode:";
    for (u_int32 i = 0; i < code.size(); i++)
        code[i]->ascii (out);

    out.close ();
}

