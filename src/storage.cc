/*
   Copyright (C) 2000/2001 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file   storage.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Defines the storage and objects classes.
 * 
 * 
 */

#ifdef _DEBUG_
#include <iostream>
#endif

#include "storage.h"


storage::~storage () 
{
}


// Set a variable to a new value; delete key if value is zero to save space
void storage::set_val (string key, s_int32 value)
{
#ifdef _DEBUG_
    std::cout << "storage::set_val \"" << key << "\" = " << value << std::endl;
#endif
    if (!value) data.erase (key);
    else
        data[key] = value;
    
    changed = 1;
}

// Get the value of a variable; if key not found then variable is zero
s_int32 storage::get_val (string key)
{
#ifdef _DEBUG_
    if (data.find (key) != data.end ())
        std::cout << "storage::get_val \"" << key << "\" = " << data[key] << std::endl;
    else
        std::cout << "storage::get_val no such key \"" << key << "\"" << std::endl;
#endif
    if (data.find (key) == data.end ()) return 0;
    else return data[key];
}

// [] Operator
s_int32& storage::operator[] (string key)
{
    return data[key];
}

// Iterate over the array
pair<string, s_int32> storage::next ()
{
    if (changed)
    {
        changed = 0;
        i = data.begin ();
    }
        
    if (i == data.end ()) 
    {
        changed = 1;
        return pair<string, s_int32> (NULL, 0);
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
            std::cout << "*** objects::set: key already exists: '" << key << "'\n";
            std::cout << "*** container contents: ";

            for (j = data.begin (); j != data.end (); j++)
                std::cout << "'" << (*j).first << "', ";

            std::cout << "\n\n" << flush;
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
    std::cout << "*** objects::get: key does not exist: '" << key << "'\n";
    std::cout << "*** container contents: ";

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

