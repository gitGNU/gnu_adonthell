/*
   $Id$

   Copyright (C) 2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream>

#include "Python.h"

#include "py_inc.h"

/*
 * SWIG init prototypes. Should we use dynamic linking??? 
 */
extern "C"
{
	void initplayerc(void);
	void initmapenginec(void);
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
	//exec_file("scripts/init.py");

	initplayerc();
	initmapenginec();

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

/* Import a module, return TRUE if successful */
bool import_module( char *filename )
{
	PyObject *result = PyImport_ImportModule( filename );

	if ( PyErr_Occurred() )
		PyErr_Print();

	Py_XDECREF(result);

	return true;
}



