/*
   $Id$

   Copyright (C) 1999/2000/2001    Kai Sterker
   Copyright (C) 2001    Alexandre Courbot
Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   py_object.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the py_object class.
 * 
 * 
 */


#ifndef PY_OBJECT_H_
#define PY_OBJECT_H_


#include <string>
#include "python_class.h"

using namespace std; 


/**
 * Python instance class.
 *
 * Use this class to give scripting - abilities to your
 * own class. The loaded script is byte-compiled for
 * faster execution, and can then be launched on a regular
 * basis.
 * 
 */ 
class py_object
{
public:
    
    /** 
     * Default constructor.
     * 
     */
    py_object ();
    
    /** 
     * Destructor.
     * 
     */
    ~py_object (); 

    /** 
     * Resets the script to it's post-constructor state.
     * 
     */
    void clear (); 
 
    /** 
     * Sets the object instance.
     * 
     * @param file file name of the module to use.
     */
    bool set_instance (string file, string modname, PyObject * args = NULL);

    /**
     * Returns the file name of this script.
     * 
     * 
     * @return file name of this script.
     */
    string script_file () const
    {
        return script_file_;
    }
     
    void call_method (const string & name, PyObject * args = NULL); 
    
    /** 
     * Runs the script.
     *
     * The script will only be run if it exists (set_script has been called)
     * and is activated.
     * 
     */
    void run (PyObject * args = NULL)
    {
        call_method ("run", args); 
    }

private:
    string script_file_;

    PyObject *instance;
};


#endif
