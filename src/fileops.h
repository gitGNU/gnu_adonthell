/*
   $Id$

   Copyright (C) 1999 - 2001 The Adonthell Team
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __FILEOPS_H__
#define __FILEOPS_H__

#include <zlib.h>
#include <string.h>
#include "types.h"

// some file related functions
class fileops
{
public:
    static void put_string (gzFile, const char*);     // Save a string
    static char* get_string (gzFile);           // Load a string

    static void put_version (gzFile, u_int16);  // Set version of a file
    static bool get_version (gzFile, u_int16, u_int16, const char*); // Check version
};

#endif // __FILEOPS_H__
