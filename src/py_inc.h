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

#include "Python.h"

extern void insert_path( char * );
extern bool init_python( void );
extern void kill_python( void );
extern bool exec_file( char *filename );
extern PyObject *import_module( char *);
#endif
