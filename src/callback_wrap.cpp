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

#include "callback_wrap.h"
#include "data.h"
#include "win_types.h"
#include "win_base.h"

// Enables us to pass a python function to the window system's
// callback mechanism
callback_wrap::callback_wrap (win_base *wnd)
{
    window = wnd;
}

// destructor
callback_wrap::~callback_wrap ()
{
    for (vector<Callback *>::iterator i = callbacks.begin (); i != callbacks.end (); i++)
        delete *i;
}

// Connect a signal to our window
void callback_wrap::signal_connect (PyObject *pyfunc, int signal, PyObject *args)
{
    // create the callback
    Callback *callback = new Callback (pyfunc, args);
    callbacks.push_back (callback);

    // connect the signal
    switch (signal)
    {
        case WIN_SIG_CLOSE:
        {
            window->set_callback_quit (
                makeFunctor (*callback, &Callback::callback_func1));
            break;
        }
        case WIN_SIG_DESTROY:
        {
            window->set_callback_destroy (
                MemberTranslator0wRet<bool,Callback,bool (Callback::*)()> (
                    *callback, &Callback::callback_func0ret));
                // makeFunctor (*callback, &Callback::callback_func0ret));
            break;
        }
        default:
        {
            window->set_signal_connect (
                makeFunctor (*callback, &Callback::callback_func0), signal);
        }
    }
}

Callback::Callback (PyObject *func, PyObject *args)
{
    function = func;
    arguments = args;
    Py_XINCREF (function);
    Py_XINCREF (arguments);
}

Callback::~Callback ()
{
    Py_XDECREF (function);
    Py_XDECREF (arguments);
}

// calls the python function without argument
void Callback::callback_func0 ()
{
    PyObject *py_arg = Py_BuildValue ("(O)",arguments);
    PyObject_CallObject (function, py_arg);
    Py_XDECREF (py_arg);
}

// calls the python function returning a boolean
bool Callback::callback_func0ret ()
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
void Callback::callback_func1 (int arg)
{
    PyObject *py_arg;

    if (arguments) py_arg = Py_BuildValue ("(i,O)", arg, arguments);
    else py_arg = Py_BuildValue ("(i)", arg);

    PyObject_CallObject (function, py_arg);
    Py_XDECREF (py_arg);
}

#endif // USE_PYTHON