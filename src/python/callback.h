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

#ifndef PY_CALLBACK_H__
#define PY_CALLBACK_H__


/**
 * @file   callback.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the callback class.
 * 
 * 
 */

 
#include "Python.h"

namespace python
{

    /**
     * Stores the C++ <-> Python callback binding
     *
     */ 
    class callback
    {
    public:
        /** 
         * Constructor that assigns a function and it's arguments to the callback.
         * 
         * @param func function assigned to this callback.
         * @param args Arguments passed to the function.
         */
        callback (PyObject * func, PyObject * args);   

        /** 
         * Destructor.
         * 
         */
        ~callback ();

        /** 
         * Calls the python function without arguments.
         * 
         */
        void callback_func0 ();

        /**
         * Calls the python function and returns bool.
         * 
         * @return the result of the callback function.
         */
        bool callback_func0ret (); 

        /**
         * Calls the python function with an integer.
         * 
         * @param arg Integer argument to pass to the callback. 
         */ 
        void callback_func1 (int arg);

    private:
        /**
         * The actual function call
         *
         */
        PyObject *make_call (PyObject*);

        /**
         * The function to be called.
         * 
         */ 
        PyObject *function;

        /**
         * Additional arguments passed to the function.
         * 
         */ 
        PyObject *arguments;
    };
}

#endif // PY_CALLBACK_H__
