/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   listener.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the listener class.
 * 
 * 
 */


#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input/event.h"
#include "callback.h"
#include "python/base.h"
#include "types.h"

namespace input
{
    /**
     * Class that has the ability to get notified of input events by the input_manager
     * and launch callback functions when such events are triggered.
     * 
     */
    class listener
    {    
    public:
        /**
         * Constructor.
         * 
         */
        listener ();

        /**
         * Destructor.
         * 
         */
        ~listener ();    

        /** 
         * Returns whether this listener will react to a certain kind of events.
         * 
         * @param t event type to test
         * 
         * @return \e true if this listener listens to events of type t, \e false otherwise.
         */
        bool is_listening_to (event::input_type t) 
        {
            return Listen_to[t]; 
        }

        /** 
         * Connect a callback function to a type of event for this listener.
         * The listener will automatically listen to this kind of events.
         * 
         * @param t type of event to listen to
         * @param f callback function to call when an event of type \e t is raised
         */
        void connect_function(event::input_type t, Functor1wRet<event *, int> f);

        /** 
         * Connect a Python callback function to a type of event for this listener.
         * The listener will automatically listen to this kind of events.
         * 
         * @param t type of event to listen to
         * @param f callback function to call when an event of type \e t is raised
         */
        void connect_py_function(int t, PyObject * f);

        /** 
         * Stops listening to events of type \e t. 
         * 
         * @param t type of events to stop listening to.
         */
        void disconnect_function(event::input_type t);

        /** 
         * Raise the \e ev event with the listener. This will call the
         * appropriate callback function with the appropriate parameters
         * if this listener listens to events of the type of \e ev.
         * 
         * @warning You should never have to call this function by yourself. If
         *          you want to manually raise an event, use 
         *          input_manager::raise_event() instead.
         * 
         * @param ev event to raise
         * 
         * @return value returned by the callback function
         */
        int raise_event (event * ev);

    private:
        typedef enum callback_type { NO_CALLBACK, C_CALLBACK, PY_CALLBACK };

        bool Listen_to[event::NBR_INPUT_TYPES];
        callback_type Callback_set[event::NBR_INPUT_TYPES];
        Functor1wRet<event *, int> Callbacks[event::NBR_INPUT_TYPES];
        PyObject * Py_callbacks[event::NBR_INPUT_TYPES];
    };
}

#endif
