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
#include <iostream> 

PyObject * data::globals;

using namespace std;


/*
 * Start Python
 */
void python::init (void)
{
    Py_Initialize();
}

void python::cleanup () 
{     
    Py_Finalize ();
}

/*
 * Insert a string into the module search path.
 */
void python::insert_path( char *name )
{
    char buf[256];
    
    sprintf ( buf, "import sys ; sys.path.insert(0, '%s')", name );
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
bool python::exec_file( char *filename )
{
    FILE *f;
    char *fn = new char[strlen("scripts/")+strlen(filename)+1];
    strcpy (fn, "scripts/");
    strcat (fn, filename);
    f = fopen (fn, "r");
    
    if (!f)
    {
        cerr << "exec_file: " << fn << " load failed!" << endl;
    	delete[] fn;
        return false;
    }
    
    PyRun_SimpleFile (f, fn);
//     if (res) Py_DECREF (res);
//     else
//     {
//         cout << "exec_file: " << fn << " execution failed!" << endl; 
//         show_traceback (); 
//     }
    
    fclose (f);
    delete[] fn;
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
    
    show_traceback ();
    return result;
}

// Make a C++ instance available to Python
PyObject *python::pass_instance (void *instance, const char *class_name)
{
    char class_ptr[256];
    char class_addr[256] = "_";
    char buffer[256];
    
    // Construct the python shadow class matching the "instance" class 
    strcat (strcpy (class_ptr, class_name), "Ptr");
    
    // Construct SWIG's representation of the "instance" pointer
    // (unfortunately this has changed from _<address>_<name>_p to 
    // _<address>_p_<name> as of SWIG 1.3 alpha 5)
    sprintf (buffer, "%p_p_%s", instance, class_name);
    strcat (class_addr, buffer+2);
    
    // Now create the Python object corresponding to "instance"
    PyObject *cls = PyDict_GetItemString (data::globals, class_ptr);
    PyObject *arg = Py_BuildValue ("(s)", class_addr);
    PyObject *res = PyObject_CallObject (cls, arg);
    
    show_traceback ();
    
    // Clean up
    Py_DECREF (arg);
    
    // Voila: "res" is 'identical' to "instance" :)
    return res;
}

// Grab a function's code object from a Python module
PyCodeObject *python::get_function_code (PyObject *module, const char* func_name)
{
    PyCodeObject *code = NULL;

    // Try to grab the function object
    if (PyObject_HasAttrString (module, (char*) func_name))
    {
        PyObject *function = PyObject_GetAttrString (module, (char*) func_name);

        // If the function exists, get it's code object
        if (function && PyCallable_Check (function))
        {
            code = (PyCodeObject *) PyObject_GetAttrString (function, "func_code");
/*
            cout << "code->co_flags   " << code->co_flags << endl;
            cout << "code->co_nlocals " << code->co_nlocals << endl;
            cout << "code->co_names ";
            PyObject_Print (code->co_names, stdout, 0);	
            cout << endl << "code->co_varnames ";
            PyObject_Print (code->co_varnames, stdout, 0);
            cout << endl << endl;
*/
            //if (code->co_flags & CO_NEWLOCALS)
            //    code->co_flags -= CO_NEWLOCALS;
            // code->co_flags = 0;
        }

        // Clean up
        Py_XDECREF (function);
    }

    return code;
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