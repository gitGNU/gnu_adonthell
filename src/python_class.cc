/*
   $Id$

   Copyright (C) 2001/2002 Kai Sterker <kaisterker@linuxgames.com>
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
 * @brief  Defines the python class.
 * 
 * 
 */

#include "python_class.h"
#include <iostream> 

using std::cerr;
using std::endl;
using std::string;

PyObject * data::globals;

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
bool python::exec_file (string filename)
{
    PyObject *mod = python::import_module (filename);
 
    if (!mod)
    {
        cerr << "exec_file: " << filename << " load failed!" << endl;
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
    char class_ptr[256];
    char class_addr[256];
    char *buffer = class_addr;

    // Construct the python shadow class matching the "instance" class 
    strcat (strcpy (class_ptr, class_name), "Ptr");
    
    // Construct SWIG's representation of the "instance" pointer
    *(buffer++) = '_';
    buffer = ptr_to_string (buffer, &instance, sizeof (void *));
    strcpy (buffer, "_p_");
    strcpy (buffer+3, class_name);

    // Now create the Python object corresponding to "instance"
    PyObject *cls = PyDict_GetItemString (data::globals, class_ptr);
    PyObject *arg = Py_BuildValue ("(s)", class_addr);
    PyObject *res = PyObject_CallObject (cls, arg);
    
#ifdef PY_DEBUG
    show_traceback ();
#endif

    // Clean up
    Py_DECREF (arg);
    
    // Voila: "res" is 'identical' to "instance" :)
    return res;
}

// Convert a pointer to a string (like SWIG 1.3.7 does)
char *python::ptr_to_string (char *c, void *ptr, int sz)
{
    static char hex[17] = "0123456789abcdef";
    int i;
    unsigned char *u = (unsigned char *) ptr;
    register unsigned char uu;

    for (i = 0; i < sz; i++,u++)
    {
        uu = *u;
        *(c++) = hex[(uu & 0xf0) >> 4];
        *(c++) = hex[uu & 0xf];
    }

    return c;
}

// load tuple from file
PyObject * python::get_tuple (igzstream & file)
{
    PyObject *tuple, *item;
    u_int32 size;

    size << file;
    tuple = PyTuple_New (size);

    for (u_int32 i = 0; i < size; i++)
    {
        item = get_object (file);
        if (item != NULL) PyTuple_SetItem (tuple, i, item);
    }

    return tuple;
}

// save contents of a tuple to file
void python::put_tuple (PyObject * tuple, ogzstream & file)
{
    if (!PyTuple_Check (tuple))
    {
        fprintf (stderr, "*** python::put_tuple: argument is no tuple!");
        return;
    }

    u_int32 size = PyTuple_Size (tuple);
    size >> file;

    for (u_int32 i = 0; i < size; i++)
        put_object (PyTuple_GetItem (tuple, i), file);
}

// load dict from file
PyObject *python::get_dict (igzstream & file)
{
    PyObject *dict, *key, *value;
    u_int8 load;

    dict = PyDict_New ();
    load << file;

    while (load != 0)
    {
        key = get_object (file);
        value = get_object (file);

        PyDict_SetItem (dict, key, value);
        load << file;
    }

    return dict;
}

// save contents of a dict to file
void python::put_dict (PyObject * dict, ogzstream & file)
{
    if (!PyDict_Check (dict))
    {
        fprintf (stderr, "*** python::put_dict: argument is no dict!");
        return;
    }

    PyObject *key, *value;
    s_int32 pos = 0;
    u_int8 load = 1;

    while (PyDict_Next (dict, &pos, &key, &value))
    {
        // save key only if saving value will succeed
        if (PyInt_Check (value) || PyString_Check (value))
        {
            load >> file;
            put_object (key, file);
            put_object (value, file);
        }
    }

    // end of dict
    load = 0;
    load >> file;
}

// load string or integer from file
PyObject *python::get_object (igzstream &file)
{
    char c;
    c << file;

    switch (c)
    {
        // string
        case 's':
        {
            string str;
            str << file;

            return PyString_FromString (str.c_str ());
        }

        // integer
        case 'i':
        {
            u_int32 i;
            i << file;

            return PyInt_FromLong (i);
        }

        // error:
        default:
        {
            fprintf (stderr, "*** python::get_object: unknown object code '%c'", c);
        }
    }

    return NULL;
}

// save basic python objects to file
bool python::put_object (PyObject *item, ogzstream & file)
{
    // Check for the type of this object
    // String?
    if (PyString_Check (item))
    {
        's' >> file;
        char * s = PyString_AsString (item);
        string (s) >> file;
        return true;
    }

    // Integer?
    if (PyInt_Check (item))
    {
        'i' >> file;
        u_int32 i = PyInt_AsLong (item);
        i >> file;
        return true;
    }

    return false;
}
