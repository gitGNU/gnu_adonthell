/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

template <class T> class Array
{
public:
    Array();
    ~Array();

    u_int8 add_element (T);             // Add Element, resize Array if neccessary 
    T get_element (u_int32);            // Return Element
    u_int8 remove_element (u_int32);    // Delete Element from Array
    u_int8 remove_data (T);             // Remove Element
    u_int32 search_element (T);         // Search for Element
    u_int32 length ();                  // Return number of Elements
    T* get_array ();                    // Pointer to internal array
    void clear ();                      // Remove all elements
    
protected:
    T       *array;                     // Element Storage
    u_int32 size;                       // Array size
    u_int32 count;                      // Element count
};
