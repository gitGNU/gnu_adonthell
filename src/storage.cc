/*
   $Id$
   
   Copyright (C) 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   storage.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the storage and objects classes.
 * 
 * 
 */

#ifdef _DEBUG_
#include <iostream.h>
#endif

#include "storage.h"


storage::~storage () 
{
    pair <const char *, s_int32> p;
    while (1) 
    {
        p = next ();
        if (p.first == NULL) break;
        delete p.first; 
    }
}


// Set a variable to a new value; delete key if value is zero to save space
void storage::set_val (const char *key, s_int32 value)
{
#ifdef _DEBUG_
    cout << "storage::set_val \"" << key << "\" = " << value << endl;
#endif
    if (!value) data.erase (key);
    else
    {
        hash_map<const char *, s_int32, hash<const char *>, equal_key>::iterator j = data.find (key);

        if (j == data.end ())
        {
            char *k = strdup (key);
            data[k] = value;
        }
        else (*j).second = value;
    }
    
    changed = 1;
}

// Get the value of a variable; if key not found then variable is zero
s_int32 storage::get_val (const char *key)
{
#ifdef _DEBUG_
    if (data.find (key) != data.end ())
        cout << "storage::get_val \"" << key << "\" = " << data[key] << endl;
    else
        cout << "storage::get_val no such key \"" << key << "\"" << endl;
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
void objects::set_val (const char* key, storage *val)
{
    map<const char*, storage*, ltstr>::iterator j;

    // Check whether that key already exists -> if so, that is bad!
    for (j = data.begin (); j != data.end (); j++)
        if (strcmp ((*j).first, key) == 0)
        {
#ifdef _DEBUG_
            cout << "*** objects::set: key already exists: '" << key << "'\n";
            cout << "*** container contents: ";

            for (j = data.begin (); j != data.end (); j++)
            cout << "'" << (*j).first << "', ";

            cout << "\n\n" << flush;
#endif // _DEBUG_

        return;
    }
  
    data[key] = val;
    changed = 1;
}

// Retrieve a object from the map
storage* objects::get_val (const char* key)
{
    map<const char*, storage*, ltstr>::iterator j;

    // Check whether the key exists
    for (j = data.begin (); j != data.end (); j++)
        if (strcmp ((*j).first, key) == 0)
            return (*j).second;

#ifdef _DEBUG_
    cout << "*** objects::get: key does not exist: '" << key << "'\n";
    cout << "*** container contents: ";

    for (j = data.begin (); j != data.end (); j++)
        cout << "'" << (*j).first << "', ";

    cout << "\n\n" << flush;
#endif // _DEBUG_

    // That probably causes a segfault, but if we can't get the
    // required object, we are in trouble anyway.
    return NULL;
}

// Delete a key from the array
void objects::erase (const char *key)
{
    // Check whether the key exists
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
