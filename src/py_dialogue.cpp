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

#include "types.h"

#include "Python.h"

#include "py_inc.h"
#include "py_dialogue.h"


/*
 * Initialise a dialog module.
 *
 * 'name' is the path to the module. We return a pointer to the newly
 * instantiated class object.
 *
 * In case of error, we return NULL.
 */
PyObject *start_dialogue(char *fpath, char *name)
{
	PyObject *module;
	PyObject *classobj;
	PyObject *class_inst;

	/* First, test if the module has already been imported */

	/* Seems not, so import */
	module = import_module(fpath);

	if (!module)
		return NULL;

	/* Extract the class from the dialogue module */
	classobj = PyObject_GetAttrString(module, name);

	if (!classobj)
		return NULL;

	Py_DECREF(module);

	/* 
	 * Instantiate! Will we ever need to pass args to class
	 * constructor here?
	 */
	class_inst = PyObject_CallObject(classobj, NULL);

	Py_DECREF(classobj);

	return class_inst;
}


