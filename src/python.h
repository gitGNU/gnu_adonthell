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
 * @file   python.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the python class.
 * 
 * 
 */
 

#ifndef PYTHON_H__
#define PYTHON_H__

#include "Python.h"
#include "compile.h"
#include "eval.h"
#include "node.h"

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
    static PyObject *import_module( char *filename);
    
    /** 
     * Dumps any error information to stderr.
     * 
     */
    static void show_traceback( void );
    
    /** 
     * Magic function that makes any C object available from Python!
     * 
     * @param instance pointer to the instance to pass.
     * @param class_name name of the class of the passed instance.
     * 
     * @return pointer to the passed object.
     */
    static PyObject *pass_instance (void* instance, const char* class_name); 
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

#endif // PYTHON_H__
