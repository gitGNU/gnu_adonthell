/*
   $Id$

   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "types.h"
#include "array_tmpl.h"
#include "interpreter.h"

struct _dlgnode;

// Instanciate some Arrays
template class Array<u_int32>;
template class Array<char*>;
template class Array<_dlgnode*>;
template class Array<CmdNew>;

// Create Array with space for 5 elements
template <class T> Array<T>::Array()
{
    u_int32 i;

    count = 0;
    size = 5;

    array = new T[size];
    for (i = 0; i < size; i++)
        array[i] = 0;
}

// Delete Array BUT NOT the elements
template <class T> Array<T>::~Array ()
{
    delete[] array;
}

// Append another element
template <class T> inline u_int8 Array<T>::add_element (T data)
{
    u_int32 i;

    // If Array is full, we have to enlarge it first
    if (count == size)
    {
        // Double array size each time => faster :)
        T *tmp = new T[size + size];

	    // Was the space sufficient?
        if (!tmp) return 0;

        for(i = 0; i < size; i++)
            tmp[i] = array[i];

        size += size;

        for (i = count; i < size; i++)
            tmp[i] = 0;

        delete[] array;

        array = tmp;
    }

    // Insert element at first free space 
    array[count] = data;
    count++;

    return 1;
}

// Remove element from Array
template <class T> inline u_int8 Array<T>::remove_element (u_int32 index)
{
    u_int32 i;

    // Check for valid index
    if (index >= count || index < 0) 
        return 0;

    count--;

    // Move elements to fill gap
    for (i = index; i < count; i++)
        array[i] = array[i+1];

    array[count] = 0;

    return 1;
}

// Remove Element from Array
template <class T> inline u_int8 Array<T>::remove_data (T data)
{
    return remove_element (search_element (data) - 1);
}

// get Element by index
template <class T> inline T Array<T>::get_element(u_int32 index)
{
    if (index >= count || index < 0) 
        return 0;
  
    return array[index];
}

// search for element 
template <class T> inline u_int32 Array<T>::search_element(T data)
{
  u_int32 i;

    for (i = 0; i < count; i++)
        if (array[i] == data)
            return i+1;

    return 0;
}

// query array-length
template <class T> inline u_int32 Array<T>::length ()
{
    return count;
}

// pointer to internal array
template <class T> inline T* Array<T>::get_array ()
{
    return array;
}

// remove all elements
template <class T> inline void Array<T>::clear ()
{
    count = 0;
}
