/*
   $Id$
   
   Copyright (C) 2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   storage.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the storage and objects classes.
 * 
 * 
 */


#ifndef STORAGE_H_
#define STORAGE_H_

#include <string.h>
#include <map>
#include <vector>

#include "types.h"
#include "str_hash.h"

#ifndef SWIG
using namespace std; 
#endif


/** 
 * Base storage class. If you want to access attributes of an object of yours
 * you have to derive that object's class from 'storage' and store the attributes
 * in the hash_map.
 *
 */ 
class storage
{
public:
    /** 
     * Default constructor.
     * 
     */
    storage () { changed = 1; }

    /** 
     * Destructor.
     * 
     */
    ~storage (); 

    /** 
     * Sets key to value.
     * 
     * @param key key.
     * @param value value.
     */
    void set_val (string key, s_int32 value);

    /** 
     * Returns the value of a key.
     * 
     * @param key key to return.
     * 
     * @return value of key.
     */
    s_int32 get_val (string key);

    /** 
     * Returns the next (key, value) pair of the storage.
     * 
     * 
     * @return Next element.
     */
    pair<string, s_int32> next ();

#ifndef SWIG
    /** 
     * Returns the value of a key.
     *
     * @attention Not available from Python. From Python, use get ()
     * instead.
     * 
     * @param key key to return
     * 
     * @return value of key.
     */
    s_int32& operator[] (string key);
#endif
    
private:
#ifndef SWIG
    hash_map<string, s_int32> data;
    hash_map<string, s_int32>::iterator i;
    u_int8 changed;
#endif

public:
#ifndef SWIG
    /**
     * Storage iterator, similar to STL iterator.
     * 
     */ 
    typedef hash_map<string, s_int32>::iterator iterator;
    
    /** 
     * Returns an iterator to the beginning of the storage.
     * 
     * 
     * @return iterator to the beginning of the storage.
     */
    iterator begin () 
    {
        return data.begin (); 
    }
    
    /** 
     * Returns an iterator to the end of the storage.
     * 
     * 
     * @return iterator to the end of the storage.
     */
    iterator end () 
    {
        return data.end (); 
    }

    /** 
     * Returns the size (number of elements) of the storage.
     * 
     * 
     * @return size of the storage.
     */
    u_int32 size () const
    {
        return data.size (); 
    }
#endif
};


/**
 * The global container for access to all the different %game objects 
 * from within a script
 */ 
class objects
{
public:
    /** 
     * Default constructor.
     * 
     */
    objects () { changed = 1; }
    
    /** 
     * Associates an object to a key.
     * 
     * @param key key.
     * @param val storage associated to key.
     */
    void set_val (const char * key, storage* val);

    /** 
     * Returns a storage associated to a key.
     * 
     * @param key key to return.
     * 
     * @return storage associated to key.
     */
    storage* get_val (const char * key);

    /** 
     * Erases a storage from it's key.
     * 
     * @param key key to erase.
     */
    void erase (const char * key);

    /** 
     * Returns the next storage in the object.
     * 
     * 
     * @return next storage in the object.
     */
    storage* next ();

private:
#ifndef SWIG
    /*
     * Checks two strings for their order (needed for the map)
     *
     */
    struct ltstr
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp (s1, s2) < 0;
        }
    };

    map<const char*, storage*, ltstr> data;
    map<const char*, storage*, ltstr>::iterator i;
    u_int8 changed; 
#endif
};

#ifndef SWIG

/**
 * Stores %objects of any kind.
 *
 * Please see the hash_map documentation in STL documentation for a detailed
 * description of this class.
 * 
 */
template <class mytype>
class dictionary : public hash_map<string, mytype>
{
};

#endif

#endif // STORAGE_H_
