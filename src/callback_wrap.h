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

#ifndef __CALLBACK_WRAP_H__
#define __CALLBACK_WRAP_H__

#include <vector>
#include "callback.h"
#include "Python.h"

class win_base;

#ifndef SWIG
// This class stores the C++ <-> Python callback binding
class Callback
{
public:
    Callback (PyObject *, PyObject *);   
    ~Callback ();

    void callback_func0 ();             // calls the python function w/o argument
    bool callback_func0ret ();          // calls the python function and returns bool
    void callback_func1 (int);          // calls the python function with an integer

private:
    PyObject *function;                 // the function to be called
    PyObject *arguments;                // additional arguments passed to the func
};
#endif // SWIG

// Enables us to pass a python function to the window system's
// callback mechanism
class callback_wrap
{
public:
    callback_wrap (win_base*);          // constructor
    ~callback_wrap ();                  // destructor

    // Connect a signal to the window
    void signal_connect (PyObject *pyfunc, int signal, PyObject *args = NULL); 

private:
#ifndef SWIG
    vector<Callback *> callbacks;       // the callbacks connected to the window
    win_base *window;                   // the window we want connect the signal to
#endif // SWIG
};

#endif // __CALLBACK_WRAP_H__
