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

#include "types.h"
#include "fileops.h"

// write a string to a file
void fileops::put_string (gzFile file, const char *string)
{
    u_int16 length = 0;

    if (string != NULL)
    {
        length = strlen (string);
        gzwrite (file, &length, sizeof (length));    
        gzwrite (file, (void*)string, length);
    } 
    else gzwrite (file, &length, sizeof (length));   
}

// get string from a file
char *fileops::get_string (gzFile file)
{
    char *string = NULL;
    u_int16 length;

    gzread (file, &length, sizeof (length));
    if (length != 0)
    {
        string = new char[length+1];
        gzread (file, string, length);
        string[length] = 0;
    }

    return string;
}

// write version info to a file
void fileops::put_version (gzFile file, u_int16 version)
{
    gzputc (file, 'v');
    gzwrite (file, &version, sizeof (version));
}

// read version info from file and check whether we can handle it
bool fileops::get_version (gzFile file, u_int16 min, u_int16 max, const char *name = NULL)
{
    char vinfo = gzgetc (file);
    u_int16 version;

    if (name == NULL) name = "<unknown>";

    // file contains no version info
    if (vinfo != 'v')
    {
        fprintf (stderr, "Version information missing in file \"%s\"\n", name);
        fprintf (stderr, "You should get a more recent data package.\n");
        return false;
    }

    // check whether file has the version we expect
    gzread (file, &version, sizeof (version));

    if (version < min || version > max)
    {
        fprintf (stderr, "File \"%s\" has version number %i,\n", name, version);
        fprintf (stderr, "but I was expecting %i <= version <= %i.\n", min, max);

        // file is newer than code
        if (version > max)
            fprintf (stderr, "You should get an up-to-date version of this program.");
        // file is older than code
        else
            fprintf (stderr, "You should get a more recent data package.\n");

        return false;
    } 

    return true;
}
