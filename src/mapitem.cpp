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

#include "mapevent.h"
#include "mapitem.h"
#include "types.h"

#include "py_inc.h"

// CONSTRUCTOR
mapitem::mapitem()
{
	number = 0;
	pattern = 0;

	event_walk = 0;
	event_action = 0;
	name = '\0';
	status = 0;
}

/*
 * Load the item set 'filename'.
 */
mapitem *loaditems( char *filename )
{
	// HACK
	insert_path( "scripts" );

	exec_file( filename );

	cout << filename << endl;
	return NULL;
}


