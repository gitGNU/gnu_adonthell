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


#include "win_event.h"
#include "py_callback.h"


void win_event::py_signal_connect (PyObject *pyfunc, int signal, PyObject *args = NULL) 
{
    // create the callback
    py_callback *callback = new py_callback (pyfunc, args);
    py_callbacks.push_back (callback);

    // connect the signal
    switch (signal)
    {
    	case CLOSE:
	    {
	        set_callback_quit (makeFunctor (*callback, &py_callback::callback_func1));
	        break;
	    }
	
	    case DESTROY:
	    {
	        set_callback_destroy (
	        MemberTranslator0wRet<bool, py_callback, bool (py_callback::*)()> (
	            *callback, &py_callback::callback_func0ret));
	        // makeFunctor (*callback, &py_callback::callback_func0ret));
	        break;
	    }
	
	    default:
	    {
	        set_signal_connect (makeFunctor (*callback, &py_callback::callback_func0), signal);
	    }
    }
}
  

bool win_event::update()
{
  if(callback_destroy_ && !callback_destroy_()) return false;
  return true;
}


win_event::~win_event()
{
  //notify that window is closing 
  if (callback_quit_) (callback_quit_) (return_code_);
  
  //delete any python callbacks
  for (vector<py_callback *>::iterator i = py_callbacks.begin (); i != py_callbacks.end (); i++)
    delete *i;
}






