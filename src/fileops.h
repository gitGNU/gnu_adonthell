/*
   $Id$

   Copyright (C) 2001 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/



/**
 * @file   fileops.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the igzstream, ogzstream and fileops classes.
 * 
 */




#ifndef FILEOPS_H_
#define FILEOPS_H_

#include <zlib.h>
#include <string>
#include "types.h"

using std::string; 

/**
 * Enumeration to know whether a file is read or write opened.
 * 
 */ 
typedef enum { READ, WRITE } gz_type;


/**
 * Base class for igzstream and ogzstream.
 * 
 */ 
class gz_file
{
public:
    /** 
     * Default constructor.
     * 
     */
    gz_file ();

#ifndef SWIG
    /** 
     * 
     * 
     * @param fname name of the file to open.
     * @param t access (READ or WRITE).
     */
    gz_file (string fname, gz_type t);
#endif

    /** 
     * Destructor.
     * 
     */
    ~gz_file ();
    
    /** 
     * Opens a file.
     * 
     * @param fname name of the file to open.
     * @param t access (READ or WRITE).
     * 
     * @return true if succeed, false otherwise.
     */
    bool open (string fname, gz_type t);

    /** 
     * Close the file that was opened.
     * 
     */
    void close ();

    /** 
     * Returns whether the file is opened or not.
     * 
     * 
     * @return true if the file is opened, false otherwise.
     */
    bool is_open () { return opened; }

    /** 
     * Returns whether the file is at it's end or not.
     * 
     * 
     * @return true if the end of file is reached, else otherwise.
     */
    bool eof () 
    {
        return gzeof (file); 
    }
    
protected:
    /** 
     * The actual gzFile.
     * 
     */ 
    gzFile file;

private:
    /// NEVER pass this by value.
    gz_file (gz_file& src); 
    
    /// Opened or not?
    bool opened; 
};


/** 
 * Class to read data from a Gzip compressed file.
 */ 
class igzstream : public gz_file
{
public:
    /**
     * Default constructor.
     * 
     */ 
    igzstream ();

#ifndef SWIG
    /** 
     * Opens a file for read access.
     * 
     * @param fname name of the file to open.
     * 
     */ 
    igzstream (string fname);
#endif
    
    /**
     * Destructor.
     * 
     */ 
    ~igzstream ();

    /** 
     * Opens a file for read access.
     * 
     * @param fname name of the file to open.
     * 
     * @return true if succeed, false otherwise.
     */
    bool open (string fname);

    /** 
     * Reads a block of bytes from the file.
     * 
     * @param to pointer to the buffer where to read.
     * @param size number of bytes to read.
     */
    void get_block (void * to, u_int32 size); 

#ifndef SWIG
    /// Reads a boolean.
    friend bool& operator << (bool& n, igzstream& gfile);

    /// Reads a char.
    friend char& operator << (char& n, igzstream& gfile);

    /// Reads a u_int8.
    friend u_int8& operator << (u_int8& n, igzstream& gfile);

    /// Reads a s_int8.
    friend s_int8& operator << (s_int8& n, igzstream& gfile);

    /// Reads a u_int16.
    friend u_int16& operator << (u_int16& n, igzstream& gfile);

    /// Reads a s_int16.
    friend s_int16& operator << (s_int16& n, igzstream& gfile);

    /// Reads a u_int32.
    friend u_int32& operator << (u_int32& n, igzstream& gfile);

    /// Reads a s_int32.
    friend s_int32& operator << (s_int32& n, igzstream& gfile);

    /// Reads a string.
    friend string& operator << (string& s, igzstream& gfile);
#endif
    
private:
    /// NEVER pass this by value.
    igzstream (igzstream& src);  
};

/** 
 * Class to write data from a Gzip compressed file.
 */ 
class ogzstream : public gz_file
{
public:
    /**
     * Default constructor.
     * 
     */ 
    ogzstream ();

#ifndef SWIG
    /** 
     * Opens a file for write access.
     * 
     * @param fname name of the file to open.
     * 
     */ 
    ogzstream (string fname);
#endif
    
    /**
     * Destructor.
     * 
     */
    ~ogzstream ();

    /** 
     * Opens a file for write access.
     * 
     * @param fname name of the file to open.
     * 
     * @return true if succeed, false otherwise.
     */
    bool open (string fname);

    /** 
     * Writes a block of bytes to the file.
     * 
     * @param to pointer to the buffer to write.
     * @param size number of bytes to write.
     */
    void put_block (void * to, u_int32 size); 

#ifndef SWIG
    /// Writes a boolean.
    friend const bool& operator >> (const bool& n, ogzstream& gfile);

    /// Writes a char.
    friend const char& operator >> (const char& n, ogzstream& gfile);

    /// Writes a u_int8.
    friend const u_int8& operator >> (const u_int8& n, ogzstream& gfile);

    /// Writes a s_int8.
    friend const s_int8& operator >> (const s_int8& n, ogzstream& gfile);

    /// Writes a u_int16.
    friend const u_int16& operator >> (const u_int16& n, ogzstream& gfile);

    /// Writes a s_int16.
    friend const s_int16& operator >> (const s_int16& n, ogzstream& gfile);

    /// Writes a u_int32.
    friend const u_int32& operator >> (const u_int32& n, ogzstream& gfile);

    /// Writes a s_int32.
    friend const s_int32& operator >> (const s_int32& n, ogzstream& gfile);

    /// Writes a string.
    friend string& operator >> (const string& s, ogzstream& gfile);
#endif
    
private:
    /// NEVER pass this by value.
    ogzstream (ogzstream& src);   
};

/// File version control class.
class fileops
{
public:
    /** 
     * Sets the version number of a file.
     * 
     * @param file file where to write the version number.
     * @param version version number to write.
     */
    static void put_version (ogzstream& file, u_int16 version);  // Set version of a file

    /** 
     * 
     * 
     * @param file file to check version.
     * @param min minimum expected version number.
     * @param max maximum expected version number.
     * @param name filename of the passed file.
     * 
     * @return true if 
     */
    static bool get_version (igzstream& file, u_int16 min, u_int16 max, string name); // Check version
};


#endif // __FILEOPS_H__
