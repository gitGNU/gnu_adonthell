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
#ifdef _DEBUG_
    cout << "storage::set \"" << key << "\" = " << value << endl;
#endif
    if (!value) data.erase (key);
    else data[key] = value;

    changed = 1;
}

// Get the value of a variable; if key not found then variable is zero
s_int32 storage::get (const char *key)
{
#ifdef _DEBUG_
    if (data.find (key) != data.end ())
        cout << "storage::get \"" << key << "\" = " << data[key] << endl;
    else
        cout << "storage::get no such key \"" << key << "\"" << endl;
#endif
    if (data.find (key) == data.end ()) return 0;
    else return data[key];
}

// [] Operator
s_int32& storage::operator[] (const char *key)
{
    return data[key];
}

// Iterate over the array
pair<const char*, s_int32> storage::next ()
{
    if (changed)
    {
        changed = 0;
        i = data.begin ();
    }
        
    if (i == data.end ()) 
    {
        changed = 1;
        return pair<const char*, s_int32> (NULL, 0);
    }

    return *i++;
}


// Insert a new object for access from the interpreter
void objects::set (const char* key, storage *val)
{
    // Check wether that key already exists -> if so, that is bad!
    if (data.find (key) != data.end ())
    {
#ifdef _DEBUG_
        cout << "*** objects::set: key already exists: " << key << endl;
        cout << "*** container contents: ";

        for (map<const char*, storage*>::iterator i = data.begin (); i != data.end (); i++)
            cout << (*i).first << ", ";

        cout << "\n\n" << flush;
#endif // _DEBUG_

        return;
    }
  
    data[key] = val;
    changed = 1;
}

// Retrieve a object from the map
storage* objects::get (const char* key)
{
    // Check wether the key exists
    if (data.find (key) == data.end ())
    {
#ifdef _DEBUG_
        cout << "*** objects::get: key does not exist: " << key << endl;
        cout << "*** container contents: ";

        for (map<const char*, storage*>::iterator i = data.begin (); i != data.end (); i++)
            cout << (*i).first << ", ";

        cout << "\n\n" << flush;
#endif // _DEBUG_

        // That probably causes a segfault, but if we can't get the
        // required object, we are in trouble anyway.
        return NULL;
    }

    return data[key];
}

// Delete a key from the array
void objects::erase (const char *key)
{
    // Check wether the key exists
    if (data.find (key) != data.end ())
    {
        data.erase (key);
        changed = 1;
    }
}

// Iterate over the array
storage *objects::next ()
{
    if (changed)
    {
        changed = 0;
        i = data.begin ();
    }
    
    if (i == data.end ()) 
    {
        changed = 1;
        return NULL;
    }

    return (*i++).second;
}
