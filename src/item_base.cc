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

#include <algorithm>
#include "item_base.h"

// ctor
item_base::item_base (std::string item) : py_object ()
{
    std::cout << "Item instance of '" << item << "' created" << endl;
    
    create_instance (item, item);
}

// dtor
item_base::~item_base ()
{
    std::cout << "Item instance deleted" << endl;
    
    py_object::clear ();
}

// init item
void item_base::init (PyObject *item)
{
    std::cout << "Item instance initialized" << endl;
    
    // set derived item instance
    Py_XDECREF (instance);
    instance = item;
    
    // init basic attributes
    charge = 0;
}

// trigger item's main functionality
bool item_base::use (character_base *character)
{
    std::cout << "Item instance used" << endl;
    
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

// make the item member of the given category.
void item_base::add_type (const std::string &type)
{
    // only add if item does not already belong to that category
    if (find (types.begin (), types.end (), type) != types.end ())
        types.push_back (type);
}

// remove the given category from the item.
void item_base::remove_type (const std::string &type)
{
    types.erase (remove (types.begin (), types.end (), type), types.end ());
}

// save state
void item_base::put_state (ogzstream& file) const
{
    /** Move to inventory for efficiency! Means we need to pass_instance
        'file' only once and can use if for all items of the inventory.
     
    // save derived item class' state
    if (has_attribute ("put_state"))
    {
        // pass file
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance (file, "ogzstream"));
    
        // call method
        call_method ("put_state", args);
        
        // cleanup
        Py_XDECREF (args); 
    }
    
    **/
}

// load state
bool item_base::get_state (igzstream& file)
{
    return true;
}
