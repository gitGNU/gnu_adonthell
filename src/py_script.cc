/*
  $Id$
  
  Copyright (C) 1999/2000/2001   Kai Sterker
  Copyright (C) 2001    Alexandre Courbot
  Part of the Adonthell Project http://adonthell.linuxgames.com
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.
  
  See the COPYING file for more details.
*/


/**
 * @file   py_script.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the py_script class.
 * 
 * 
 */

#include <iostream>
#include "py_script.h"

py_script::py_script ()
{
    locals = NULL; 
    script = NULL;
    set_active (true); 
}


py_script::~py_script ()
{
    clear (); 
}

void py_script::clear ()
{ 
    if (script)
    {
        Py_DECREF (script);
        script = NULL; 
    }

    script_file_ = ""; 
    locals = NULL; 
    set_active (true); 
}

void py_script::set_script (string file)
{
    if (file == "")
    {
        script_file_ = "";

        if (script) 
        {
            Py_DECREF (script); 
            script = NULL; 
        }
        
        return;
    } 
    else if (script) 
    {
        Py_DECREF (script); 
        script = NULL; 
    }
    
    FILE *f = fopen (file.c_str (), "r");

    // See whether the script exists at all
    if (f)
    {
        // Compile the script into a PyCodeObject for quicker execution
        struct _node *n = PyParser_SimpleParseFile (f, (char *) file.c_str (), Py_file_input);

        if (n)
        {
            // If no errors occured update script code ...
            if (script)
                delete script;

            script = PyNode_Compile (n, (char *)file.c_str ());
            PyNode_Free (n);

            script_file_ = file;
        }
        else
        {
            cout << "\n*** Cannot set script: Error in" << flush;
            python::show_traceback ();
        }
        fclose (f);
    }
    else
        cout << "\n*** Cannot open script: file \"" << file
             << "\" not found!" << flush;
}

void py_script::run () 
{
    if (script && is_activated ())
    {
        PyEval_EvalCode (script, data::globals, locals);
    }
}
