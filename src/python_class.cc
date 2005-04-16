/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   python_class.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the python class.
 * 
 * 
 */

#include "python_class.h"
#include "game.h"
#include <iostream> 

PyObject *data::globals;
PyObject *python::module;

// defined in py_adonthell_wrap.cc
PyObject *pass_instance (void *instance, const char* class_name);

using namespace std;

/*
 * Start Python
 */
void python::init ()
{
    Py_Initialize ();
}

/**
 * Stop Python
 */
void python::cleanup () 
{
    // Cleanup the global namespace of python interpreter
    // Note that we don't have to DECREF data::globals, because they're a
    // borrowed reference of py_module.
    Py_XDECREF (module);
    Py_Finalize ();
}

/*
 * Insert a string into the module search path.
 */
void python::insert_path( char *name )
{
    char buf[256];
    
    sprintf ( buf, "import sys ; sys.path.insert(0, \"%s\")", name );
    PyRun_SimpleString ( buf );
}

/*
 * Some convenience functions
 */

/*
 * Executes the Python statements in the string
 */
void python::exec_string(char * s)
{
    PyRun_SimpleString(s);
}

/*
 * Execute the file given by 'filename'
 */
bool python::exec_file (string filename)
{
    PyObject *mod = python::import_module (filename);
 
    if (!mod)
    {
        cerr << "exec_file: " << filename << " load failed: " << endl;
        show_traceback ();

        return false;
    }

    Py_DECREF (mod); 

    return true; 
}

/*
 * Dump any error information to stderr
 */
void python::show_traceback(void)
{
    if ( PyErr_Occurred() )
    {
        PyErr_Print();
        fflush (stderr);
    }
}

/* Import a module, return module ptr */
PyObject *python::import_module (string filename)
{
    PyObject *result = PyImport_ImportModule ((char *) filename.c_str ());
    
#ifdef PY_DEBUG
    show_traceback ();
#endif
    return result;
}

// Make a C++ instance available to Python
PyObject *python::pass_instance (void *instance, const char *class_name)
{
    return pass_instance (instance, class_name);
}

PyObject * python::get_tuple (igzstream & file)
{
    PyObject * tuple; 
    u_int32 l;
    l << file;

    tuple = PyTuple_New (l);

    for (u_int32 i = 0; i < l; i++) 
    {
        string ms;
        u_int32 j;
        char c;
        
        c << file;
        switch (c) 
        {
            case 's':
                ms << file;
                // Stolen reference
                PyTuple_SetItem (tuple, i, PyString_FromString (ms.c_str ()));
                break;
                
            case 'i':
                j << file;
                // Stolen reference
                PyTuple_SetItem (tuple, i, PyInt_FromLong (j));
                break; 
        }
    }
    return tuple; 
}

void python::put_tuple (PyObject * tuple, ogzstream & file)
{
    u_int32 l = PyTuple_Size (tuple);
    l >> file;
    for (u_int32 i = 0; i < l; i++) 
    {
        // Borrowed reference
        PyObject * item = PyTuple_GetItem (tuple, i);
        
        // Check for the type of this object
        // String?
        if (PyString_Check (item)) 
        {
            's' >> file;
            char * s = PyString_AsString (item); 
            string (s) >> file;
        }
        
        // Integer?
        else if (PyInt_Check (item)) 
        {
            'i' >> file;
            u_int32 li = PyInt_AsLong (item); 
            li >> file;
        }
    }
}
