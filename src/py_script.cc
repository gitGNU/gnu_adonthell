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
    cleanup = NULL;
    set_active (true);
}


py_script::~py_script ()
{
    clear (); 
}

// Cleanup (and re-initialisation)
void py_script::clear ()
{
    // Execute the script's "destructor"
    if (cleanup)
    {
        PyEval_EvalCode (cleanup, data::globals, locals);
        python::show_traceback ();
    }

    // Delete our code objects
    Py_XDECREF (script);
    Py_XDECREF (cleanup);

    script = NULL;
    cleanup = NULL;

    script_file_ = "";
    set_active (true);
}

// Pass a (new) Python script to be used
void py_script::set_script (string file)
{
    // Cleanup if we already had a script before
    if (script)
        clear ();

    // just clear, don't set a new script
    if (file == "")
        return;

    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module)
        return;

    // Try to get the body of the script (also checks whether
    // the script is in the right form)
    script = python::get_function_code (module, "run");
    if (script)
    {
        // Try to call the "constructor"
        PyCodeObject *init = python::get_function_code (module, "init");
        if (init)
        {
            PyEval_EvalCode (init, data::globals, locals);
            python::show_traceback ();
        }

        // Get the "destructor"
        cleanup = python::get_function_code (module, "cleanup");

        // Set the script file
        script_file_ = file;
    }
    else
        cerr << "*** Error: no \"run\" function found in \"" << file << ".py\"\n";

    // Cleanup
    Py_DECREF (module);
}

// Execute the body of the script
void py_script::run () 
{
    cout << "*** running " << script_file_ << endl;
    PyObject_Print (locals, stdout, 0);
    cout << "\n\n";

    if (script && is_activated ())
    {
        PyEval_EvalCode (script, data::globals, locals);
        python::show_traceback ();
    }
}
