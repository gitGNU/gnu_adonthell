/*
  $Id$
  
  Copyright (C) 1999/2000/2001/2002 Kai Sterker
  Copyright (C) 2001    Alexandre Courbot
  Part of the Adonthell Project http://adonthell.linuxgames.com
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.
  
  See the COPYING file for more details.
*/


/**
 * @file   object.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the object class.
 * 
 * 
 */

#include "object.h"

using namespace python;

object::object ()
{
    Instance = NULL;
    CtorArgs = NULL;
    Classname = "";
    Filename = "";
}

object::~object ()
{
    clear (); 
}

// Cleanup (and re-initialisation)
void object::clear ()
{
    // Delete our instance
    Py_XDECREF (Instance);
    Py_XDECREF (CtorArgs);
    
    Instance = NULL;
    CtorArgs = NULL;
    
    Classname = "";
    Filename = "";
}

// Pass a (new) Python module to be used
bool object::create_instance (std::string file, std::string classname, PyObject *args)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Instanciate!
    return instanciate (module, file, classname, args);
}

// Reload a python module in case it has changed on disk
bool object::reload_instance (std::string file, std::string classname, PyObject *args)
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
bool object::instanciate (PyObject *module, std::string file, std::string classname, PyObject *args)
{
    // Cleanup
    clear ();

    PyObject * classobj = PyObject_GetAttrString (module, (char *) classname.c_str ());
    Py_DECREF (module);
    if (!classobj)
    {
        python::show_traceback ();
        return false;
    }

    // Create the instance
    Instance = PyObject_CallObject (classobj, args);
    Py_DECREF (classobj);
    if (!Instance)
    {
        python::show_traceback ();
        return false;
    }

    Classname = classname;
    Filename = file;
    Py_XINCREF (args);
    CtorArgs = args;
    
    return true;
}

// Execute the body of the script
void object::call_method (const std::string & name, PyObject * args)
{
    if (Instance)
    {
        PyObject *tocall = PyObject_GetAttrString (Instance, (char *) name.c_str ());

        if (PyCallable_Check (tocall) == 1)
        {    
            PyObject *res = PyObject_CallObject (tocall, args);
            Py_XDECREF (res); 
            Py_DECREF (tocall); 
        }
#ifdef PY_DEBUG
        python::show_traceback ();
#endif
    }
}

// Get an attribute of the instance
PyObject *object::get_attribute (const std::string &name)
{
    if (Instance) 
        return PyObject_GetAttrString (Instance, (char *) name.c_str ());
    else
        return NULL;
}

// check for a certain attribute
bool object::has_attribute (const std::string & name)
{
    if (Instance)
        return PyObject_HasAttrString (Instance, (char *) name.c_str ());
    else
        return false;
}

// Save internal state of the script to disk
void object::put_state (ogzstream &file)
{
    Filename >> file;
    Classname >> file;
    python::put_tuple (CtorArgs, file);
    python::put_dict (PyObject_GetAttrString (Instance, "__dict__"), file);

}

// Get internal script state from disk
bool object::get_state (igzstream &file)
{
    PyObject *state;

    // clear the current state    
    clear ();
    
    // get file- and classname, constructor arguments and script state
    Filename << file;
    Classname << file;
    CtorArgs = python::get_tuple (file);
    state = python::get_dict (file);
    
    // instanciate the script and restore script state
    if (create_instance (Filename, Classname, CtorArgs))
    {
        PyObject *dict, *key, *value;
        s_int32 pos = 0;
        
        // we'll have to merge the state with the object's current
        // __dict__, as only part of it has been saved
        dict = PyObject_GetAttrString (Instance, "__dict__");
        
        while (PyDict_Next (state, &pos, &key, &value))
            PyDict_SetItem (dict, key, value);
        
        Py_DECREF (state);
        return true;
    }
    
    return false;
}

// 'clone' an object
void object::operator= (const object & script)
{
    // cleanup
    clear ();
    
    // copy
    Classname = script.class_name ();
    Filename = script.file_name ();
    CtorArgs = script.get_ctorargs ();
    Instance = script.get_instance ();
    
    // make sure the refcount is correct
    Py_XINCREF (CtorArgs);
    Py_XINCREF (Instance);
}
