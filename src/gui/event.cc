/*
   $Id$

   (C) Copyright 2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#include "event.h"
#include "python/callback.h"

using namespace gui;

void event::py_signal_connect (PyObject *pyfunc, int signal, PyObject *args ) 
{
    // create the callback
  python::callback *callback = new python::callback (pyfunc, args);
  py_callbacks.push_back (callback);

    // connect the signal
    switch (signal)
    {
    	case CLOSE:
	    {
	        set_callback_quit (makeFunctor (*callback, &python::callback::callback_func1));
	        break;
	    }
	
	    case DESTROY:
	    {
	        set_callback_destroy (
	        MemberTranslator0wRet<bool, python::callback, bool (python::callback::*)()> (
	            *callback, &python::callback::callback_func0ret));
	        // makeFunctor (*callback, &py_callback::callback_func0ret));
	        break;
	    }
	
	    default:
	    {
	        set_signal_connect (makeFunctor (*callback, &python::callback::callback_func0), signal);
	    }
    }
}
  

bool event::update()
{
  if(callback_destroy_ && !callback_destroy_()) return false;
  return true;
}


event::~event()
{
  //notify that window is closing 
  if (callback_quit_) (callback_quit_) (return_code_);
  
  //delete any python callbacks
  for (std::vector<python::callback *>::iterator i = py_callbacks.begin (); i != py_callbacks.end (); i++)
    delete *i;
}
