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
 * @file   python_class.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the python class. This file is named this way
 *         so it doesn't conflicts with Python.h Python's include
 *         file on non-case aware systems.
 * 
 * 
 */
 

#ifndef PYTHON_CLASS_H__
#define PYTHON_CLASS_H__

#include <string>

#include "Python.h"
#include "compile.h"
#include "eval.h"
#include "node.h"
#include "fileops.h"

/**
 * Grant direct access to the Python interpreter.
 * 
 */ 
class python
{  
public:
    /** 
     * Initialise Python and inserts the Adonthell include paths.
     * 
     * 
     * @return true in case of success, false otherwise.
     */
    static void init (void);

    /**
     * Cleanup Python.
     * 
     */ 
    static void cleanup (); 

    /** 
     * Adds a directory to Python's include path.
     * 
     * @param name directory to add to Python's include path.
     */
    static void insert_path( char * name);
    
    /** 
     * Execute Python statements contained in a string.
     * 
     * @param s string containing Python statements to execute.
     */
    static void exec_string(char * s);
    
    /** 
     * Executes a Python file.
     * 
     * @param filename name of the file to execute.
     * 
     * @return true in case of success, false otherwise.
     */ 
    static bool exec_file( char *filename );
    
    /** 
     * Imports a Python module.
     * 
     * @param filename file name of the module to import.
     * 
     * @return pointer to the imported module.
     */
    static PyObject *import_module (string filename);
    
    /** 
     * Dumps any error information to stderr.
     * 
     */
    static void show_traceback( void );
    
    /** 
     * Magic function that makes any C object available to Python!
     * 
     * @param instance pointer to the instance to pass.
     * @param class_name name of the class of the passed instance.
     * 
     * @return pointer to the passed %object.
     */
    static PyObject *pass_instance (void* instance, const char* class_name);

    /**
     * Return a function defined in the module as a code %object!
     *
     * @param pointer to the module.
     * @param func_name name of the function to grab.
     *
     * @return pointer to the code %object, or NULL if the function does not exist.
     */
    static PyCodeObject *get_function_code (PyObject *module, const char* func_name);

    static PyObject * get_tuple (igzstream & file); 
    static void put_tuple (PyObject * tuple, ogzstream & file);  
};

#ifndef SWIG
namespace data
{
    /**
     * Global namespace to use in scripts.
     * 
     */ 
    extern PyObject *globals;
}
#endif

#endif // PYTHON_CLASS_H__
