/*
   Copyright (C) 2001/2002 Kai Sterker <kai.sterker@gmail.com>
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

#ifndef PY_CALLBACK_H__
#define PY_CALLBACK_H__


/**
 * @file   py_callback.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the py_callback class.
 * 
 * 
 */

 
#include <locale>
#include "Python.h"
#include "fileops.h"

/**
 * Stores the C++ <-> Python callback binding
 *
 */ 
class py_callback
{
public:

    /**
     * Default ctor,
     */
    py_callback ();

    /** 
     * Constructor that assigns a function and its arguments to the callback.
     * 
     * @param func function assigned to this callback.
     * @param args Arguments passed to the function.
     */
    py_callback (PyObject * func, PyObject * args);   

    /** 
     * Destructor.
     */
    ~py_callback ();

    
    /**
     * @name Executing the callback
     */
    //@{
    
    /** 
     * Calls the python function without arguments.
     */
    void callback_func0 ();

    /**
     * Calls the python function and returns bool.
     */
    bool callback_func0ret (); 

    /**
     * Calls the python function with an integer.
     *
     * @param arg Integer value to pass to the callback
     */ 
    void callback_func1 (int arg);
    
    //@}
    
    
    /**
     * @name Loading / Saving
     */
    //@{

    /** 
     * Saves the callback and it's arguments to file.
     * @note Currently, arguments have to be a tuple containing only
     *      integers and/or strings.
     *
     * @param out file where to save the callback.
     */ 
    void put_state (ogzstream& out) const;
    
    /** 
     * Restores the callback from a file. For that to work, the static
     * py_callback::instance member has to point to the python instance 
     * containing the callback.
     * 
     * @param in file to load the callback from.
     *
     * @return \e true if the callback could be restored, \e false otherwise
     *
     * @sa instance
     */
    bool get_state (igzstream& in);

    /**
     * When restoring a callback from file, instance has to point to the
     * python instance (module or class) containing the callback.
     *
     * @sa get_state
     */
    static PyObject *instance;
    //@}

private:

    /**
     * The actual function call.
     *
     * @param args The arguments passed to the callback.
     */
    PyObject *make_call (PyObject *args);

    /**
     * The function to be called.
     */ 
    PyObject *function;

    /**
     * Additional arguments passed to the function.
     */ 
    PyObject *arguments;
};

#endif // PY_CALLBACK_H__
