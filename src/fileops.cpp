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
    if (t == READ) file = gzopen (fname.c_str (),"r");
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

bool& operator << (bool& n, igzstream& gfile)
{
    gzread (gfile.file, &n, sizeof (n));
    return n;
}
char& operator << (char& n, igzstream& gfile)
{
    gzread (gfile.file, &n, sizeof (n));
    return n;
}

u_int8& operator << (u_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

s_int8& operator << (s_int8& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

u_int16& operator << (u_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

s_int16& operator << (s_int16& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

u_int32& operator << (u_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

s_int32& operator << (s_int32& n, igzstream& gfile)
{
    gzread(gfile.file, &n, sizeof (n));
    return n;
}

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

bool& operator >> (bool& n, ogzstream& gfile)
{
    gzwrite (gfile.file, &n, sizeof (n));
    return n;
}
char& operator >> (char& n, ogzstream& gfile)
{
    gzwrite (gfile.file, &n, sizeof (n));
    return n;
}

u_int8& operator >> (u_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

s_int8& operator >> (s_int8& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

u_int16& operator >> (u_int16& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

s_int16& operator >> (s_int16& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

u_int32& operator >> (u_int32& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

s_int32& operator >> (s_int32& n, ogzstream& gfile)
{
    gzwrite(gfile.file, &n, sizeof (n));
    return n;
}

string& operator >> (string& s, ogzstream& gfile)
{
    u_int16 strl = s.length ();
    string::iterator i; 
    strl >>  gfile;
    for (i = s.begin (); i != s.end (); i++) 
        (*i) >>  gfile;
    return s;
}

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

// get string from a file
string fileops::get_string2 (gzFile file)
{
    string res;
    u_int16 length;

    gzread (file, &length, sizeof (length));
    while (length)
    {
	char c;
        gzread (file, &c, sizeof (c));
	res+=c;
	length--;
    }

    return res;
}

// write version info to a file
void fileops::put_version (gzFile file, u_int16 version)
{
    gzputc (file, 'v');
    gzwrite (file, &version, sizeof (version));
}

void fileops::put_version (ogzstream& file, u_int16 version)
{
    char c = 'v'; 
    c >> file;
    version >> file; 
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
	cerr << "File \"" << name << "\" has version number " << version << ",\n";
        cerr <<  "but I was expecting " << min << " <= version <= " << max << endl;
	
        // file is newer than code
        if (version > max)
            cerr << "You should get an up-to-date version of this program.\n";
        // file is older than code
        else
            cerr << "You should get a more recent data package.\n";

        return false;
    } 
    return true;
}
