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


// Checks two strings for equality (needed for the hash_map)
struct equal_key
{
    bool operator() (const char* s1, const char* s2) const
    {
        return strcmp (s1, s2) == 0;
    } 
};

// Checks two strings for their order (needed for the map)
struct ltstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp (s1, s2) < 0;
    }
};

// Base storage class. If you want to access attributes of an object of yours
// you have to derive that object's class from 'storage' and store the attributes
// in the hash_map
class storage
{
public:
    storage () { changed = 1; }
    
    void set (const char*, s_int32);
    s_int32 get (const char*);
    s_int32& operator[] (const char*);
    pair<const char*, s_int32> next ();
    
protected:
    hash_map <const char*, s_int32, hash<const char*>, equal_key> data;
    hash_map<const char*, s_int32, hash<const char*>, equal_key>::iterator i;
    u_int8 changed;
};


// The global container for access to all the different game objects 
// from within a script
class objects
{
public:
    objects () { changed = 1; }
    
    void set (const char*, storage*);
    storage* get (const char*);
    void erase (const char*);
    storage* next ();

private:
    map<const char*, storage*, ltstr> data;
    map<const char*, storage*>::iterator i;
    u_int8 changed;
};

#endif // __STORAGE_H__
