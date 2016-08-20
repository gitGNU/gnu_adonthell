/*
   (C) Copyright 2001 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "win_event.h"
#include "py_callback.h"


void win_event::py_signal_connect (PyObject *pyfunc, int signal, PyObject *args) 
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
		Functor0wRet<bool> func0ret;
	        set_callback_destroy (
	        makeFunctor (&func0ret, *callback, &py_callback::callback_func0ret));
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






