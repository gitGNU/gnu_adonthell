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
    Script = NULL;
    PyFunc = NULL;
    Args = NULL;
    Action = ACTION_NOTHING;
}

// destructor
event::~event ()
{
    clear ();
}

// cleanup
void event::clear ()
{
    switch (Action)
    {
        // script attached
        case ACTION_SCRIPT:
        {
            if (!Shared)
            {
                delete Script;
                Py_XDECREF (Args);
                Args = NULL;
            }
            else Script = NULL;
            
            break;
        }
        
        // python callback attached
        case ACTION_PYFUNC:
        {
            delete PyFunc;
            PyFunc = NULL;
            
            break;
        }
        
        default: break;
    }
    
    Action = ACTION_NOTHING;
}

// set a script as event's action
void event::set_script (string filename, PyObject * args = NULL)
{
    // cleanup
    clear ();
    
    if (filename == "") return;
    
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
    
    Script = new py_object;
    Script->create_instance (EVENTS_DIR + filename, filename, theargs);
    Py_DECREF (theargs);
        
    Action = ACTION_SCRIPT;
}

// make the event script a pointer to an existing script
void event::set_shared_script (py_object * script)
{
    // cleanup
    clear ();

    // attach the given script
    Script = script;
    
    // tell the script not to save any arguments
    Shared = true;

    // tell the event what to do    
    Action = ACTION_SCRIPT;
}

// set a callback as event's action
void event::set_callback (PyObject *callback, PyObject *args)
{
    // cleanup
    clear ();
    
    // create the callback
    PyFunc = new py_callback (callback, args);
    
    // tell the event what to do    
    Action = ACTION_PYFUNC;
}

// save the state of the script associated with the event
void event::put_state (ogzstream & file) const
{
    Type >> file;
    Repeat >> file;
    Action >> file;
    
    if (Action != ACTION_SCRIPT) return;
    
    Shared >> file;
    
    if (Shared) return;
    
    Script->class_name () >> file;
    
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
    Action << file;
    
    if (Action != ACTION_SCRIPT) return true;
    
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
