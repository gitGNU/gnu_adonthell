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

#ifndef __PY_CALLBACK_H__
#define __PY_CALLBACK_H__

#include "Python.h"

// This class stores the C++ <-> Python callback binding
class py_callback
{
public:
    py_callback (PyObject *, PyObject *);   
    ~py_callback ();

    void callback_func0 ();             // calls the python function w/o argument
    bool callback_func0ret ();          // calls the python function and returns bool
    void callback_func1 (int);          // calls the python function with an integer

private:
    PyObject *function;                 // the function to be called
    PyObject *arguments;                // additional arguments passed to the func
};

#endif // __PY_CALLBACK_H__
