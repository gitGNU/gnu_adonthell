/*
   $Id$

   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
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
 */

#include "event.h"
#include "event_handler.h"

// constructor
event::event ()
{
    Action = ACTION_NOTHING;
    Registered = false;
    Paused = false;
    Repeat = -1;
    Script = NULL;
    PyFunc = NULL;
    Args = NULL;
    List = NULL;
}

// destructor
event::~event ()
{
    // automatically remove myself from the event_handler
    if (Registered) event_handler::remove_event (this);
    
    // ... and from the event_list
    if (List) List->remove_event (this);
        
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
            delete Script;
            Py_XDECREF (Args);
            Args = NULL;
            Script = NULL;
            
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
void event::set_script (std::string filename, PyObject * args)
{
    // cleanup
    clear ();
    
    if (filename == "") return;
    
    Py_XINCREF (args);
    Args = args; 
       
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

// set a python callback as event's action
void event::set_callback (PyObject *callback, PyObject *args)
{
    // cleanup
    clear ();
    
    // create the callback
    PyFunc = new py_callback (callback, args);
    
    // tell the event what to do    
    Action = ACTION_PYFUNC;
}

// set a C/C++ callback as event's action
void event::set_callback (const Functor0 & callback)
{
    // cleanup
    clear ();

    Callback = callback;
    Action = ACTION_CPPFUNC;
}

// save the state of the script associated with the event
void event::put_state (ogzstream & file) const
{
    Type >> file;
    Repeat >> file;
    Paused >> file;
    Action >> file;
    
    switch (Action)
    {
        // save script
        case ACTION_SCRIPT:
        {
            Script->class_name () >> file;
    
            if (Args)
            {
                true >> file;
                python::put_tuple (Args, file);
            }
            else false >> file;
            
            return;
        }
        
        // save python callback
        case ACTION_PYFUNC:
        {
            PyFunc->put_state (file);
            return;
        }
        
        default: return;
    }
}

// load the state of the script associated with the event 
bool event::get_state (igzstream & file) 
{
    // Note that »Type« is already read by event_list::load to
    // determine what event subclass to instanciate
    Repeat << file;
    Paused << file;
    Action << file;
    
    switch (Action)
    {
        // load script from file
        case ACTION_SCRIPT:
        {
            std::string name;
            bool has_args;
            PyObject * args = NULL;

            name << file;
            has_args << file;
    
            if (has_args) args = python::get_tuple (file);
    
            set_script (name, args);
            Py_XDECREF (args);
    
            return true;
        }
        
        // load python callback from file
        case ACTION_PYFUNC:
        {
            PyFunc = new py_callback;
            return PyFunc->get_state (file); 
        }
        
        default: return true;
    }
    
    return false;
}

// the event_list this event is kept in
void event::set_list (event_list *l)
{
    List = l;
}

// repeat an event
s_int32 event::do_repeat ()
{
    if (Repeat > 0) Repeat--;
    
    return Repeat;
}
