/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <string.h>
#include <hash_map>
#include <map>

#include "types.h"


// Checks two strings for equality
struct equal_key
{
    bool operator() (const char* s1, const char* s2) const
    {
        return strcmp (s1, s2) == 0;
    } 
};


// Base storage class. If you want to access attributes of an object of yours
// you have to derive that object's class from 'storage' and store the attributes
// in the hash_map
class storage
{
public:
    void set (const char*, s_int32);
    s_int32 get (const char*);
    s_int32& operator[] (const char*);
    
private:
    hash_map <const char*, s_int32, hash<const char*>, equal_key> data;
};


// The global container for access to all the different game objects 
// from within a script
class objects
{
public:
    static void set (const char*, storage*);
    static storage* get (const char*);
    static void erase (const char*);

private:
    static map<const char*, storage*> data;
};

#endif // __STORAGE_H__
