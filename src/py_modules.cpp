/*
   $Id$

   Copyright (C) 2000 The Adonthell Team
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "py_inc.h"

/*
 * SWIG init prototypes. Should we use dynamic linking??? 
 */
extern "C"
{
	void initdialog_enginec (void);
	void initcharacterc (void);
	void initquestc (void);
}

/*
 * Start Python
 */
bool init_python (void)
{
	Py_Initialize();

	insert_path("scripts");
	insert_path("scripts/modules");

	/* Initialise SWIG modules. This should go if we ever switch to dynamic 
	   link */
	initcharacterc ();
    initdialog_enginec ();
	initquestc ();

	return true;
}

