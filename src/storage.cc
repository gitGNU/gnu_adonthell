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

#include <iostream.h>
#include "storage.h"


// Set a variable to a new value; delete key if value is zero to save space
void storage::set (const char *key, s_int32 value)
{
    if (!value) data.erase (key);
    else data[key] = value;
}

// Get the value of a variable; if key not found then variable is zero
s_int32 storage::get (const char *key)
{
    if (data.find (key) == data.end ()) return 0;
    else return data[key];
}

// [] Operator
s_int32& storage::operator[] (const char *key)
{
    return data[key];
}


// The container for access to certain objects via a keyword
map<const char*, storage*> objects::data;

// Insert a new object for access from the interpreter
void objects::set (const char* key, storage *val)
{
    // Check wether that key already exists -> if so, that is bad!
    if (data.find (key) != data.end ())
    {
        cout << "*** objects::set: key already exists: " << key << endl;

#ifdef _DEBUG_
        cout << "*** container contents: ";

        for (map<const char*, storage*>::iterator i = data.begin (); i != data.end (); i++)
            cout << (*i).first << ", ";

        cout << "\n" << flush;
#endif // _DEBUG_

        return;
    }
    
    data[key] = val;
}

// Retrieve a object from the map
storage* objects::get (const char* key)
{
    // Check wether the key exists
    if (data.find (key) == data.end ())
    {
        cout << "*** objects::get: key does not exist: " << key << endl;

#ifdef _DEBUG_
        cout << "*** container contents: ";

        for (map<const char*, storage*>::iterator i = data.begin (); i != data.end (); i++)
            cout << (*i).first << ", ";

        cout << "\n" << flush;
#endif // _DEBUG_

        // That probably causes a segfault, but if we can't get the
        // required object, we are in trouble anyway.
        return NULL;
    }

    return data[key];
}