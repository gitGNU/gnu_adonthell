/*
   $Id$

   Copyright (C) 2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream>

#include "Python.h"
#include "game.h"
#include "py_inc.h"

/*
 * SWIG init prototypes. Should we use dynamic linking??? 
 */
extern "C"
{
	void initplayerc(void);
	void initmapenginec(void);
//	void initbaseitemc(void);
}

/*
 * Designed to handle most of the Python <-> Adonthell interfacing
 */

/*
 * Insert a string into the module search path.
 */
void insert_path( char *name )
{
	char buf[256];

	sprintf( buf, "import sys ; sys.path.insert(0, '%s')", name );
	PyRun_SimpleString( buf );
}

/*
 * Start Python
 */
bool init_python(void)
{
	Py_Initialize();

	insert_path("scripts");
	insert_path("scripts/modules");
	//exec_file("scripts/init.py");

	/* Initialise SWIG modules. This should go if we ever switch to dynamic 
	   link */
	initplayerc();
	initmapenginec();
//	initbaseitemc();

	return true;
}

/* 
 * Shut down Python - DO NOT USE PYTHON API FUNCTIONS AFTER THIS POINT!
 */
void kill_python(void)
{
	Py_Finalize();
}

/*
 * Some convenience functions
 */

/*
 * Execute the file given by 'filename'
 */
bool exec_file( char *filename )
{
	FILE *f;

	f = fopen(filename, "r");

	if (!f)
	{
		cerr << "exec_module : " << filename << " load failed!" << endl;
		return false;
	}

	PyRun_AnyFile( f, filename);

	fclose(f);
	return true;
}

/*
 * Dump any error information to stderr
 */
void show_traceback(void)
{
	if ( PyErr_Occurred() )
		PyErr_Print();
}

/* Import a module, return module ptr */
PyObject *import_module( char *filename )
{
	PyObject *result = PyImport_ImportModule( filename );
	
	show_traceback();
	
	//Py_XDECREF(result);

	return result;
}

// Make a C++ instance available to Python
PyObject *pass_instance (void *instance, const char *class_name)
{
    char class_ptr[256];
    char class_addr[256] = "_";
    char buffer[256];

    // Construct the python shadow class matching the "instance" class 
    strcat (strcpy (class_ptr, class_name), "Ptr");

    // Construct SWIG's representation of the "instance" pointer
    // (unfortunately this has changed from _<address>_<name>_p to 
    // _<address>_p_<name> as of SWIG 1.3 alpha 5)
    sprintf (buffer, "%p_p_%s", instance, class_name);
    strcat (class_addr, buffer+2);

    // Now create the Python object corresponding to "instance"
    PyObject *cls = PyDict_GetItemString(game::globals, class_ptr);
    PyObject *arg = Py_BuildValue ("(s)", class_addr);
    PyObject *res = PyEval_CallObject (cls, arg);

    // Clean up
    Py_DECREF (arg);

    // Voila: "res" is 'identical' to "instance" :)
    return res;
}