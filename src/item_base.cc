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

// ctor
item_base::item_base (const std::string & item) : py_object ()
{
    std::cout << "Item instance of '" << item << "' created" << std::endl;

    get_state (item);    
}

// dtor
item_base::~item_base ()
{
    std::cout << "Item instance deleted" << endl;
    
    py_object::clear ();
}

// trigger item's main functionality
bool item_base::use (character_base *character)
{
    std::cout << "Item '" << name () << "' used" << std::endl;
    
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

// save a single item to file
bool item_base::put_state (const std::string & file) const
{
    ogzstream out (file);
    
    // has file been opened?
    if (!out.is_open ())
    {
        fprintf (stderr, "*** item_base::save: cannot write '%s'\n", file.c_str ());
        return false;
    }
    
    return put_state (out);
}

// save item to stream
bool item_base::put_state (ogzstream & file) const
{
    // do we have a valid item?
    if (!Instance) return false;
    
    // save the template this item uses
    class_name () >> file;
    
    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file, "ogzstream"));
    
    // save the actual item data
    call_method ("put_state", args);
    Py_DECREF (args);    
    
    return true;
}

// load a single item from file
bool item_base::get_state (const std::string & file)
{
    igzstream in (file);
    
    // has file been opened?
    if (!in.is_open ()) 
    {
        fprintf (stderr, "*** item_base::load: cannot read '%s'\n", file.c_str ());
        return false;
    }
    
    // load item
    return get_state (in);
}

// load item from stream
bool item_base::get_state (igzstream & file)
{
    std::string tmpl;
    
    // clean up, if neccessary
    if (Instance) clear ();
    
    // get template to use for item
    tmpl << file;
    
    // instanciate
    if (!create_instance (tmpl, tmpl)) return false;

    // pass file
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (&file, "igzstream"));
    
    // load actual item data
    call_method ("get_state", args);
    Py_DECREF (args);
    
    return true;  
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
