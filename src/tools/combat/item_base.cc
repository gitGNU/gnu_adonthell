/*
   $Id$
   
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   item_base.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Superclass for all items.
 */

#include "item_base.h"
#include "python_class.h"

item_base::item_base ()
{
    abilities = 0;
    self = python::pass_instance (this, "item_base");
}

item_base::item_base (string n, string d, string s, u_int16 a)
{
    init (n, d, s, a);
}

void item_base::init (string n, string d, string s, u_int16 a)
{
    name = n;
    description = d;
    abilities = a;
    
    string file = ACTION_DIR;
    file += s;
    
    actions.create_instance (file, s);
    self = python::pass_instance (this, "item_base");
}

bool item_base::equip (character_base *c)
{
    if (abilities & EQUIP == 0) return false;
    
    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance (c, "character_base"));
    actions.call_method ("equip", args);  
    Py_DECREF (args);

    return true;
}

bool item_base::combine (item_base *i)
{
    if (abilities & COMBINE == 0) return false;
    
    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance (i, "item_base"));
    actions.call_method ("combine", args);  
    Py_DECREF (args);

    return true;
}

bool item_base::use ()
{
    if (abilities & USE == 0) return false;
    
    PyObject* args = PyTuple_New (1);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    actions.call_method ("use", args);  
    Py_DECREF (args);

    return true;
}
