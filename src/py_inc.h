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

#ifndef _py_inc_h
#define _py_inc_h

#ifdef USE_PYTHON

#include "Python.h"
#include "compile.h"
#include "eval.h"
#include "node.h"

extern void insert_path( char * );
extern void exec_string(char * s);
extern bool exec_file( char *filename );
extern PyObject *import_module( char *);
extern void show_traceback( void );
extern PyObject *pass_instance (void*, const char*);

#endif
#endif
