/*
   Copyright (C) 1999/2000/2001/2003   Kai Sterker
   Copyright (C) 2001    Alexandre Courbot
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   py_object.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the py_object class.
 * 
 * 
 */

#include "py_object.h"

py_object::py_object ()
{
    Instance = NULL;
    Filename = "";
    Classname = "";
}

py_object::~py_object ()
{
    clear (); 
}

// Cleanup (and re-initialisation)
void py_object::clear ()
{
    // Delete our Instance
    Py_XDECREF (Instance);
    Instance = NULL;

    Filename = "";
    Classname = "";
}

// Pass a (new) Python module to be used
bool py_object::create_instance (string file, string classname, PyObject * args)
{
    // Try to import the given script
    PyObject *module = python::import_module (file);
    if (!module) return false;

    // Instanciate!
    return instanciate (module, file, classname, args);
}

// Reload a python module in case it has changed on disk
bool py_object::reload_instance (string file, string classname, PyObject * args)
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
    if (!classobj)
    {
        python::show_traceback ();
        return false;
    }
    
    // Create the Instance
    Instance = PyObject_CallObject (classobj, args);
    Py_DECREF (classobj);
    if (!Instance)
    {
        python::show_traceback ();
        return false;
    }

    Filename = file;
    Classname = classname;

    return true;
}

// Execute a method of the script
PyObject* py_object::call_method_ret (const string &name, PyObject *args) const
{
    PyObject *result = NULL;

    if (Instance)
    {
        PyObject *tocall = PyObject_GetAttrString (Instance, (char *) name.c_str ());

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

// check for a certain attribute
bool py_object::has_attribute (const std::string & name)
{
    if (Instance)
        return PyObject_HasAttrString (Instance, (char *) name.c_str ());
    else
        return false;
}

// Get an attribute of the instance
PyObject *py_object::get_attribute (const string &name) const
{
    if (Instance)
        return PyObject_GetAttrString (Instance, (char *) name.c_str ());
    else
        return NULL;
}

// Get an int attribute of the instance
s_int32 py_object::get_attribute_int (const string &name)
{
    if (Instance)
    {
        PyObject *attribute = PyObject_GetAttrString (Instance, (char *) name.c_str ());
        if (!attribute) return 0;

        s_int32 value = PyInt_AsLong (attribute);
        Py_DECREF (attribute);
        
        return value;
    }
    else
        return 0;
}

 // Get a string attribute of the instance
string py_object::get_attribute_string (const string &name)
{
    if (Instance)
    {
        PyObject *attribute = PyObject_GetAttrString (Instance, (char *) name.c_str ());
        if (!attribute) return 0;

        string value = python::as_string (attribute);
        Py_DECREF (attribute);

        return value;
    }
    else
        return string ("");
}

// Set an attribute of the instance
void py_object::set_attribute (const string &name, PyObject *value)
{
    if (Instance)
        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), value) == -1)
            python::show_traceback ();
}

// Set an int attribute of the instance
void py_object::set_attribute_int (const string &name, int value)
{
    if (Instance)
    {
        PyObject *val = PyInt_FromLong (value);

        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), val) == -1)
            python::show_traceback ();

        Py_DECREF (val);
    }
    else return;
}

// Set a string attribute of the instance
void py_object::set_attribute_string (const string &name, const string & value)
{
    if (Instance)
    {
        PyObject *val = PyString_FromString (value.c_str ());

        if (PyObject_SetAttrString (Instance, (char *) name.c_str (), val) == -1)
            python::show_traceback ();

        Py_DECREF (val);
    }
    else return;
}
