/*
   $Id$

   Copyright (C) 2001/2002 Kai Sterker <kaisterker@linuxgames.com>
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
 * Grant simplified access to the Python interpreter.
 * 
 */ 
class python
{  
public:
    /** 
     * Initialise Python and insert the Adonthell include paths.
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
     * Executes a Python script.
     * 
     * @param filename name of the file to execute.
     * 
     * @return true in case of success, false otherwise.
     */ 
    static bool exec_file (string filename);
    
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
     * Loads a Python tuple previously saved with put_tuple ().
     *
     * @param file Opened file where to load the tuple from.
     *
     * @return Restored Python tuple.
     */
    static PyObject * get_tuple (igzstream & file);

    /**
     * Save a Python tuple into a file.
     *
     * @warning The Python tuple MUST ONLY be composed of Python strings
     * or integers!
     *
     * @param tuple Python tuple to save.
     * @param file Opened file where to save the tuple to.
     */
    static void put_tuple (PyObject * tuple, ogzstream & file);

    /**
     * Loads a Python dictionary previously saved with put_dict ().
     *
     * @param file Opened file where to load the dictionary from.
     *
     * @return Restored Python dictionary.
     */
    static PyObject * get_dict (igzstream & file);

    /**
     * Save a Python dictionary into a file.
     *
     * @warning Only integer and string values of the dictionary
     * are saved!
     *
     * @param dict Python dict to save.
     * @param file Opened file where to save the tuple to.
     */
    static void put_dict (PyObject * dict, ogzstream & file);

private:
    /**
     * Convert a Pointer to a String, like SWIG 1.3.7 does
     *
     */
    static char *ptr_to_string (char *c, void *ptr, int sz);

    /**
     * Saves basic Python types, like integers or strings to file.
     *
     * @param item Python object to save.
     * @param file Opened file where to save the tuple to.
     */
    static bool put_object (PyObject *item, ogzstream & file);

    /**
     * Loads a Python object previously saved with put_object ().
     *
     * @param file Opened file where to load the object from.
     *
     * @return Restored Python object.
     */
    static PyObject * get_object (igzstream & file);
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
