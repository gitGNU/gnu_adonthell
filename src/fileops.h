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
#include <string>
#include "types.h"

typedef enum { READ, WRITE } gz_type;

class gz_file
{
public:
    gz_file ();
    gz_file (string fname, gz_type t);
    ~gz_file ();
    
    bool open (string fname, gz_type t);
    void close ();

    bool is_opened () { return opened; }

    gzFile file;
protected:
    bool opened;
};

class igzstream : public gz_file
{
public:
    igzstream ();
    igzstream (string);
    ~igzstream ();

    bool open (string);

    friend char& operator << (char&, gz_file&);
    friend u_int8& operator << (u_int8&, gz_file&);
    friend s_int8& operator << (s_int8&, gz_file&);
    friend u_int16& operator << (u_int16&, gz_file&);
    friend s_int16& operator << (s_int16&, gz_file&);
    friend u_int32& operator << (u_int32&, gz_file&);
    friend s_int32& operator << (s_int32&, gz_file&);
    friend string& operator << (string& s, igzstream& gfile);
};

// some file related functions
class fileops
{
public:
    static void put_string (gzFile, const char*);     // Save a string
    static char* get_string (gzFile);           // Load a string
    static string get_string2 (gzFile);           // Load a string

    static void put_version (gzFile, u_int16);  // Set version of a file
    static bool get_version (gzFile, u_int16, u_int16, const char*); // Check version
    static bool get_version (igzstream, u_int16, u_int16, string&); // Check version
};


#endif // __FILEOPS_H__
