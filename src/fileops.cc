/*
   $Id$

   Copyright (C) 1999/2000/2001/2002 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/



/**
 * @file   fileops.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the igzstream, ogzstream and fileops classes.
 * 
 */


#include <iostream>
#include <SDL/SDL_endian.h>
#include "fileops.h"


using namespace std;


gz_file::gz_file ()
{
    opened = false;
}

gz_file::gz_file (string fname, gz_type t)
{
    open (fname, t);
}

gz_file::~gz_file ()
{
    if (is_open ()) gzclose (file);
}

bool gz_file::open (string fname, gz_type t)
{
    if (t == READ) file = gzopen (fname.c_str (),"rb6");
    else file = gzopen (fname.c_str (),"wb6");
    if (!file) return false;
    opened = true;
    return true;
}

void gz_file::close ()
{
    if (is_open ()) gzclose (file);
    opened = false;
}

igzstream::igzstream () : gz_file ()
{
}

igzstream::igzstream (string fname) : gz_file (fname, READ)
{
}

igzstream::~igzstream ()
{
}

bool igzstream::open (string fname)
{
    return gz_file::open (fname, READ);
}

/// Reads a boolean.
bool& operator << (bool& n, igzstream& gfile)
{
    u_int8 b;
    gzread (gfile.file, &b, sizeof (b));
    return (n = b);
}

/// Reads a char.
char& operator << (char& n, igzstream& gfile)
{
    gzread (gfile.file, &n, sizeof (n));
    return n;
}

void igzstream::get_block (void * to, u_int32 size)
{
    gzread (file, to, size); 
}

/// Reads a u_int8.
u_int8& operator << (u_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

/// Reads a s_int8.
s_int8& operator << (s_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

/// Reads a u_int16.
u_int16& operator << (u_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SDL_SwapLE16(n);
    return n;
}

/// Reads a s_int16.
s_int16& operator << (s_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SDL_SwapLE16(n);
    return n;
}

/// Reads a u_int32.
u_int32& operator << (u_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SDL_SwapLE32(n);
    return n;
}

/// Reads a s_int32.
s_int32& operator << (s_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    n = SDL_SwapLE32(n);
    return n;
}

/// Reads a string.
string& operator << (string& s, igzstream& gfile)
{
    u_int16 strl;
    char c;
    s = ""; 
    strl << gfile;
    while (strl)
    {
	c << gfile;
	s += c;
	strl --;
    }
    return s;
}

ogzstream::ogzstream () : gz_file ()
{
}

ogzstream::ogzstream (string fname) : gz_file (fname, WRITE)
{
}

ogzstream::~ogzstream ()
{
}

bool ogzstream::open (string fname)
{
    return gz_file::open (fname, WRITE);
}

void ogzstream::put_block (void * to, u_int32 size)
{
    gzwrite (file, to, size); 
}

/// Writes a boolean.
const bool& operator >> (const bool& n, ogzstream& gfile)
{
    u_int8 b = n;
    gzwrite (gfile.file, &b, sizeof (b));
    return n;
}

/// Writes a char.
const char& operator >> (const char& n, ogzstream& gfile)
{
    gzwrite (gfile.file, (char *) &n, sizeof (n));
    return n;
}

/// Writes a u_int8.
const u_int8& operator >> (const u_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, (u_int8 *) &n, sizeof (n));
    return n;
}

/// Writes a s_int8.
const s_int8& operator >> (const s_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, (s_int8 *) &n, sizeof (n));
    return n;
}

/// Writes a u_int16.
const u_int16& operator >> (const u_int16& n, ogzstream& gfile)
{
    u_int16 s = SDL_SwapLE16(n);
    gzwrite(gfile.file, (u_int16 *) &s, sizeof (n));
    return n;
}

/// Writes a s_int16.
const s_int16& operator >> (const s_int16& n, ogzstream& gfile)
{
    s_int16 s = SDL_SwapLE16(n);
    gzwrite(gfile.file, (s_int16 *) &s, sizeof (n));
    return n;
}

/// Writes a u_int32.
const u_int32& operator >> (const u_int32& n, ogzstream& gfile)
{
    u_int32 s = SDL_SwapLE32(n);
    gzwrite(gfile.file, (u_int32 *) &s, sizeof (n));
    return n;
}

/// Writes a s_int32.
const s_int32& operator >> (const s_int32& n, ogzstream& gfile)
{
    s_int32 s = SDL_SwapLE32(n);
    gzwrite(gfile.file, (s_int32 *) &s, sizeof (n));
    return n;
}

/// Writes a string.
string& operator >> (const string& s, ogzstream& gfile)
{
    u_int16 strl = s.length ();
    string::iterator i; 
    strl >>  gfile;
    
    for (i = ((string&) s).begin (); i != ((string&) s).end (); i++) 
        (*i) >>  gfile;
    return (string&) s;
}

void fileops::put_version (ogzstream& file, u_int16 version)
{
    char c = 'v'; 
    c >> file;
    version >> file; 
}

// read version info from file and check whether we can handle it
bool fileops::get_version (igzstream& file, u_int16 min, u_int16 max, string name)
{
    char vinfo;
    u_int16 version;

    vinfo << file;

    if (name == "") name = "<unknown>";

    // file contains no version info
    if (vinfo != 'v')
    {
        cerr << "Version information missing in file \"" << name << endl;
        cerr << "You should get a more recent data package.\n";
        return false;
    }
    
    // check whether file has the version we expect
    version << file;

    if (version < min || version > max)
    {
        cerr << "File \"" << name << "\" has\nversion number " << version << ", ";
        cerr <<  "but I was expecting " << min << " <= version <= " << max << endl;
	
        // file is newer than code
        if (version > max)
            cerr << "You should get an up-to-date version of this program.\n\n";
        // file is older than code
        else
            cerr << "You should probably get a more recent data package.\n";

        return false;
    } 
    return true;
}
