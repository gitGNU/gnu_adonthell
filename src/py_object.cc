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
 * @file   py_object.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the py_object class.
 * 
 * 
 */

#include "py_object.h"

py_object::py_object ()
{
    instance = NULL;
    script_file_ = "";
}

py_object::~py_object ()
{
    clear (); 
}

// Cleanup (and re-initialisation)
void py_object::clear ()
{
    // Delete our instance
    Py_XDECREF (instance);
    instance = NULL;
    
    script_file_ = "";
}

// Pass a (new) Python module to be used
bool py_object::create_instance (string file, string classname, PyObject * args = NULL)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Instanciate!
    return instanciate (module, file, classname, args);
}

// Reload a python module in case it has changed on disk
bool py_object::reload_instance (string file, string classname, PyObject * args = NULL)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Now Reload
    PyObject *reload = PyImport_ReloadModule (module);
    Py_DECREF (module);
    if (!reload) return false;

    return instanciate (reload, file, classname, args);
}

// Instanciate the given class from the module
bool py_object::instanciate (PyObject *module, string file, string classname, PyObject * args)
{
    // Cleanup
    clear ();

    PyObject * classobj = PyObject_GetAttrString (module, (char *) classname.c_str ());
    Py_DECREF (module);
    if (!classobj) return false;

    // Create the instance
    instance = PyObject_CallObject (classobj, args);
    Py_DECREF (classobj);
    if (!instance) return false;

    script_file_ = classname;

    return true;
}

// Execute a method of the script
PyObject* py_object::call_method_ret (const string & name, PyObject * args = NULL)
{
    PyObject *result = NULL;
     
    if (instance)
    {
        PyObject *tocall = PyObject_GetAttrString (instance, (char *) name.c_str ());

        if (PyCallable_Check (tocall) == 1)
        {    
            result = PyObject_CallObject (tocall, args);
            Py_DECREF (tocall); 
        }
#ifdef PY_DEBUG
        python::show_traceback ();
#endif
    }
    
    return result;
}

// Get an attribute of the instance
PyObject *py_object::get_attribute (const string &name)
{
    if (!instance) return NULL;

    return PyObject_GetAttrString (instance, (char *) name.c_str ());
}
