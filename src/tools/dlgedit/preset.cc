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

#include "preset.h"
#include "pset_interface.h"
#include "../../interpreter.h"

extern int vars_compile (const char*, string&, vector<command*>&);

preset_dlg::preset_dlg (string &v, error_dlg* e) : vars(v), err (e)
{
    dlg = create_preset_dlg (this);
}


int preset_dlg::on_ok (char *v)
{
    string error ("");
    vector<command*> code;

    // is there any code at all -> try to compile
    if (strcmp (v, ""))
    {
        vars_compile (v, error, code);

        // compilation errors found
        if (error != "")
        {
            // Either create a new error_dlg window, or bring it to front
            if (!err) err = new error_dlg;
            else err->to_front ();

            // Display the error message
            err->display (error.c_str ());
            return 0;
        }
    }

    // everything turned out fine :)
    vars = v;
    return 1;
}

