/*
   $Id$
   
   Copyright (C) 2003 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   item_base.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Superclass for all items.
 */

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "item_base.h"
#include "character_base.h"

struct swig_type_info;
extern "C" {
    swig_type_info *SWIG_Python_TypeQuery (const char*);
    int SWIG_Python_ConvertPtr (PyObject*, void**, swig_type_info*, int);
}

// ctor
item_base::item_base (const std::string & item) : py_object ()
{
    Slot = NULL;

    get_state (item);    
}

// dtor
item_base::~item_base ()
{
    if (Mutable)
    {
        // if the item is still inside an inventory, remove it
        if (Slot) Slot->remove (this);
    
        // if we have a stack of items, delete the whole stack
        if (Next) delete Next;
    }
        
    py_object::clear ();
}

// can the given character equip this item?
bool item_base::can_equip (character_base *character)
{
    // no 
    if (!has_attribute ("equipped")) return false;
    
    int result = 0;
    
    // pass character
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (character, "character_base"));
    
    // call method
    PyObject *retval = call_method_ret ("can_equip", args);
    
    // if we have valid retval retrieve it
    if (PyInt_Check (retval)) result = PyInt_AS_LONG (retval);

    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);
    
    return (result != 0);
}

// tell item it has been equipped
void item_base::equipped (slot *target)
{
    // no effect
    if (!has_attribute ("equipped")) return;
    
    if (!Mutable) Slot = target;

    // call method
    call_method ("equipped");
    
    if (!Mutable) Slot = NULL;
}

// tell item it has been removed
void item_base::unequipped (slot *target)
{
    // no effect
    if (!has_attribute ("unequipped")) return;
    
    if (!Mutable) Slot = target;
    
    // call method
    call_method ("unequipped");
    
    if (!Mutable) Slot = NULL;
}

// trigger item's main functionality
bool item_base::use (character_base *character)
{
    // can't be used
    if (!has_attribute ("use")) return false;

    int result = 1;
    
    // pass character
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (character, "character_base"));
    
    // call method
    PyObject *retval = call_method_ret ("use", args);
    
    // if we have valid retval retrieve it
    if (PyInt_Check (retval)) result = PyInt_AS_LONG (retval);
    
    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);
    
    return (result != 0);
}        

// combine two items
item_base *item_base::combine (item_base *item)
{
    // can't be combined
    if (!has_attribute ("combine")) return NULL;
     
    item_base *result = NULL;
    
    // pass item
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (item, "item_base"));
    
    // call method
    PyObject *retval = call_method_ret ("combine", args);
    
    // try to retrieve result
    SWIG_Python_ConvertPtr (retval, (void **) &result, 
        SWIG_Python_TypeQuery ("_p_item_base"), 1);
    
    // cleanzp
    Py_XDECREF (retval);
    Py_DECREF (args);

    return result;
}

// save a single item to file
bool item_base::put_state (const std::string & file) const
{
    ogzstream out (file);
    
    // has file been opened?
    if (!out.is_open ())
    {
        fprintf (stderr, "*** item_base::put_state: cannot write '%s'\n", file.c_str ());
        return false;
    }
    
    return put_state (out);
}

// save item to stream
bool item_base::put_state (ogzstream & file) const
{
    // do we have a valid item?
    if (!Instance) return false;
    
    // save the attributes
    Mutable >> file;
    MaxStack >> file;
        
    // save the template this item uses
    class_name () >> file;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file, "ogzstream"));
    
    // save the actual item data
    call_method ("put_state", args);
    Py_DECREF (args);    
    
    // recursively save stack
    if (this->Next)
    {
        true >> file;
        Next->put_state (file);
    }
    else false >> file;
    
    return true;
}

// load a single item from file
bool item_base::get_state (const std::string & file)
{
    igzstream in (file);
    
    // has file been opened?
    if (!in.is_open ()) 
    {
        fprintf (stderr, "*** item_base::get_state: cannot read '%s'\n", file.c_str ());
        return false;
    }
    
    // load item
    return get_state (in);
}

// load item from stream
bool item_base::get_state (igzstream & file)
{
    std::string tmpl;
    bool more;
    
    // clean up, if neccessary
    if (Instance) clear ();
    
    // get attributes
    Mutable << file;
    MaxStack << file;
    
    // get template to use for item
    tmpl << file;
    
    // instanciate
    if (!create_instance (ITEM_PACKAGE + tmpl, tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file, "igzstream"));
    
    // load actual item data
    call_method ("get_state", args);
    Py_DECREF (args);
    
    // add reference to item_base
    set_attribute ("this", python::pass_instance (this, "item_base"));
    
    // recursively get stack
    more << file;
    
    if (more)
    {
        Next = new item_base (true);
        Next->get_state (file);
    }
    else Next = NULL;
    
    return true;  
}

// return list of categories this item belongs to
std::vector<string> item_base::categories () const
{
    std::vector<string> categories;
    PyObject *list = get_attribute ("Categories");
    PyObject *str;
            
    if (!PySequence_Check (list)) return categories;
    
    // get all category strings
    for (int i = 0; i < PySequence_Size (list); i++)
    {
        str = PySequence_GetItem (list, i);
        if (PyString_Check (str))
            categories.push_back (PyString_AsString (str));
        Py_XDECREF (str);
    }
    
    // cleanup
    Py_DECREF (list);
    
    return categories;
}


/*
// recharge item
u_int16 item_base::recharge (u_int16 &charge)
{
    Charge += charge;

    if (Charge > MaxCharge)
    {
        charge = Charge - MaxCharge;
        Charge = MaxCharge;
    }
    else charge = 0;
    
    return charge;
}
*/

// compare two items for equality
bool item_base::equals (item_base *item)
{
    if (!Mutable) return this == item;
    else return name () == item->name ();
}
        
// add item(s) to this stack
void item_base::stack (item_base *item)
{
    item_base *bottom = this;
    
    // add given stack on top of our own stack
    while (bottom->Next) bottom = bottom->Next;
    bottom->Next = item;
}

// remove item(s) from this stack
item_base *item_base::split (u_int32 count)
{
    // in case of count = 0, this is a noop
    if (count == 0) return this;
    
    item_base *remaining, *stack = this;
    
    // retrieve the topmost items
    while (--count) stack = stack->Next;
    remaining = stack->Next;
    
    // split this stack
    stack->Next = NULL;
    set_slot (NULL);
    
    return remaining;    
}
