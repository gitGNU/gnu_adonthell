/*
   $Id$

   Copyright (C) 2000/2001   Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file py_inc.h
 * @author Kai Sterker <ksterker@topmail.de>
 *
 * @brief Declares some usefull low-level functions for Python.
 */


#ifndef _py_inc_h
#define _py_inc_h

#include "Python.h"
#include "compile.h"
#include "eval.h"
#include "node.h"

/** 
 * Adds a directory to Python's include path.
 * 
 * @param name directory to add to Python's include path.
 */
extern void insert_path( char * name);

/** 
 * Execute Python statements contained in a string.
 * 
 * @param s string containing Python statements to execute.
 */
extern void exec_string(char * s);

/** 
 * Executes a Python file.
 * 
 * @param filename name of the file to execute.
 * 
 * @return true in case of success, false otherwise.
 */ 
extern bool exec_file( char *filename );

/** 
 * Imports a Python module.
 * 
 * @param filename file name of the module to import.
 * 
 * @return pointer to the imported module.
 */
extern PyObject *import_module( char *filename);

/** 
 * Dumps any error information to stderr.
 * 
 */
extern void show_traceback( void );

/** 
 * Magic function that makes any C object available from Python!
 * 
 * @param instance pointer to the instance to pass.
 * @param class_name name of the class of the passed instance.
 * 
 * @return pointer to the passed object.
 */
extern PyObject *pass_instance (void* instance, const char* class_name);

#endif
