/*
   $Id$

   Copyright (C) 2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   event.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the base event class.
 * 
 */

#include "event.h"

// constructor
event::event ()
{
    Repeat = -1;
    Shared = false;
    Args = NULL;
}

// destructor
event::~event ()
{
    clear ();
}

// cleanup
void event::clear ()
{
    Script.clear ();
    Py_XDECREF (Args);
    Args = NULL;
}

// set the script asspciated with the event
void event::set_script (string filename, PyObject * args = NULL)
{
    if (filename == "") clear ();
    else 
    {
        Py_XINCREF (args);
        Args = args; 
        Shared = false;
       
        u_int16 argssize = args == NULL ? 1 : PyTuple_Size (args) + 1; 
        PyObject *theargs = PyTuple_New (argssize);
        
        // We can pass_instance directly 'cause PyTuple_SetItem steals a
        // reference to the result of pass_instance.
        PyTuple_SetItem (theargs, 0, python::pass_instance (this, "event"));
        for (u_int16 i = 1; i < argssize; i++)
        {
            PyObject *intref = PyTuple_GetItem (args, i - 1);
            Py_INCREF (intref); 
            PyTuple_SetItem (theargs, i, intref); 
        }
        Script.create_instance (EVENTS_DIR + filename, filename, theargs);
        Py_DECREF (theargs);
    }
}

// make the event script a reference to an existing script
void event::set_shared_script (py_object &script)
{
    // cleanup
    clear ();

    // 'clone' the given script
    Script = script;
    
    // tell the script not to save any arguments
    Shared = true;
}

// save the state of the script associated with the event
void event::put_state (ogzstream & file) const
{
    Type >> file;
    Repeat >> file;
    Shared >> file;
    
    if (Shared) return;
    
    Script.class_name () >> file;
    
    if (Args)
    {
        true >> file;
        python::put_tuple (Args, file);
    }
    else false >> file;
}

// load the state of the script associated with the event 
bool event::get_state (igzstream & file) 
{
    string name;
    bool has_args;
    PyObject * args = NULL;

    // Note that »Type« is already read by event_list::load to
    // determine what event subclass to instanciate
    Repeat << file;
    Shared << file;
    
    // shared scripts have to be restored by the event's owner
    if (Shared) return true;
    
    name << file;
    has_args << file;
    
    if (has_args) args = python::get_tuple (file);
    
    set_script (name, args);
    Py_XDECREF (args);
    
    return true;
}
