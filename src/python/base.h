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
 * @file   base.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the python basic functions. This file is named this way
 *         so it doesn't conflicts with Python.h Python's include
 *         file on non-case aware systems.
 * 
 * 
 */
 

#ifndef PYTHON_BASE_H__
#define PYTHON_BASE_H__

#include <string>

#include "Python.h"
#include "compile.h"
#include "eval.h"
#include "node.h"
#include "fileops.h"

namespace python
{

    /** 
     * Initialise Python and insert the Adonthell include paths.
     * 
     * 
     * @return true in case of success, false otherwise.
     */
    void init (void);
    
    /**
     * Cleanup Python.
     * 
     */ 
    void cleanup (); 
    
    /** 
     * Adds a directory to Python's include path.
     * 
     * @param name directory to add to Python's include path.
     */
    void insert_path (const char * name);
    
    /** 
     * Execute Python statements contained in a string.
     * 
     * @param s string containing Python statements to execute.
     */
    void exec_string (char * s);
    
    /** 
     * Executes a Python script.
     * 
     * @param filename name of the file to execute.
     * 
     * @return true in case of success, false otherwise.
     */ 
    bool exec_file (const std::string filename);
    
    /** 
     * Imports a Python module.
     * 
     * @param filename file name of the module to import.
     * 
     * @return pointer to the imported module.
     */
    PyObject *import_module (const std::string filename);
    
    /** 
     * Dumps Python error information to stderr.
     * 
     */
    void show_traceback (void);
    
    /** 
     * Magic function that makes any C object available to Python!
     * 
     * @param instance pointer to the instance to pass.
     * @param class_name name of the class of the passed instance.
     * 
     * @return pointer to the passed %object.
     */
    PyObject *pass_instance (void* instance, const char* class_name);
    
    /**
     * Loads a Python tuple previously saved with put_tuple ().
     *
     * @param file Opened file where to load the tuple from.
     *
     * @return Restored Python tuple.
     */
    PyObject * get_tuple (igzstream & file);
    
    /**
     * Save a Python tuple into a file.
     *
     * @warning The Python tuple MUST ONLY be composed of Python strings
     * or integers!
     *
     * @param tuple Python tuple to save.
     * @param file Opened file where to save the tuple to.
     */
    void put_tuple (PyObject * tuple, ogzstream & file);
    
    /**
     * Loads a Python dictionary previously saved with put_dict ().
     *
     * @param file Opened file where to load the dictionary from.
     *
     * @return Restored Python dictionary.
     */
    PyObject * get_dict (igzstream & file);
    
    /**
     * Save a Python dictionary into a file.
     *
     * @warning Only integer and string values of the dictionary
     * are saved!
     *
     * @param dict Python dict to save.
     * @param file Opened file where to save the tuple to.
     */
    void put_dict (PyObject * dict, ogzstream & file);
    


    /// @fixme This is normally private stuff!

    /**
     * Convert a Pointer to a String, like SWIG 1.3.7 does
     *
     */
    char *ptr_to_string (char *c, void *ptr, u_int32 sz);
    
    /**
     * Saves basic Python types, like integers or strings to file.
     *
     * @param item Python object to save.
     * @param file Opened file where to save the tuple to.
     */
    bool put_object (PyObject *item, ogzstream & file);
    
    /**
     * Loads a Python object previously saved with put_object ().
     *
     * @param file Opened file where to load the object from.
     *
     * @return Restored Python object.
     */
    PyObject * get_object (igzstream & file);
}

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
