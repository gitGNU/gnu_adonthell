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

#ifdef USE_PYTHON

#include "py_callback.h"

py_callback::py_callback (PyObject *func, PyObject *args)
{
    function = func;
    arguments = args;
    Py_XINCREF (function);
    Py_XINCREF (arguments);
}

py_callback::~py_callback ()
{
    Py_XDECREF (function);
    Py_XDECREF (arguments);
}

// calls the python function without argument
void py_callback::callback_func0 ()
{
    PyObject *py_arg = Py_BuildValue ("(O)",arguments);
    PyObject_CallObject (function, py_arg);
    Py_XDECREF (py_arg);
}

// calls the python function returning a boolean
bool py_callback::callback_func0ret ()
{
    int retval = 1;
    
    PyObject *py_arg = Py_BuildValue ("(O)",arguments);
    PyObject* val = PyObject_CallObject (function, py_arg);

    if (val) retval = PyInt_AsLong (val);

    Py_XDECREF (val);
    Py_XDECREF (py_arg);

    return retval != 0;
}

// calls the python function with an integer as argument
void py_callback::callback_func1 (int arg)
{
    PyObject *py_arg;

    if (arguments) py_arg = Py_BuildValue ("(i,O)", arg, arguments);
    else py_arg = Py_BuildValue ("(i)", arg);

    PyObject_CallObject (function, py_arg);
    Py_XDECREF (py_arg);
}

#endif // USE_PYTHON
