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
 * Python object class.
 *
 * Use this class to create instances of Python classes
 * contained in Python modules, then control their execution. You can pass
 * arguments to the class constructor when the script is imported, then to
 * any method you want to run.
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
     * Creates an instance of a Python class.
     * 
     * @param file file name of the module to use.
     * @param classname name of the class to import.
     * @param args Python tuple containing the arguments to pass to the
     *             Python class constructor.
     */
    bool create_instance (string file, string classname, PyObject * args = NULL);

    /**
     * Direct access to the instance object
     *
     * @return the Python class instance
     */
    PyObject *get_instance ()
    {
        return instance;
    }

    /**
     * Returns the module name of this object.
     * 
     * @return module name of this object.
     */
    string object_file () const
    {
        return script_file_;
    }
     
    /** 
     * Call a method of this object.
     * 
     * @param name name of the method to call.
     * @param args Python tuple containing the arguments to pass to the method.
     */
    void call_method (const string & name, PyObject * args = NULL); 
    
    /** 
     * Calls the run () method of this object.
     * Equivalent to call_method ("run", args); 
     *
     * @param args Python tuple containing the arguments to pass to the method.  
     */
    void run (PyObject * args = NULL)
    {
        call_method ("run", args); 
    }

    /**
     * Returns a new reference to an attribute of this object.
     *
     * @param name Name of the attribute to access
     * @return New reference to the attribute or NULL on error
     */
    PyObject* get_attribute (const string & name);

private:
    string script_file_;

    PyObject *instance;
};


#endif
