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
 * @file   event.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the %event class.
 * 
 */

#ifndef EVENT_H__
#define EVENT_H__

#include "fileops.h"
#include "callback.h"
#include "py_object.h"
#include "py_callback.h"

using std::string;

/**
 * Directory where %event scripts reside.
 */ 
#define EVENTS_DIR "game_events."

#ifndef SWIG
/**
 * Available %event types.
 */ 
enum
{
    ENTER_EVENT     = 0,            // Characters reach a new tile
    LEAVE_EVENT     = 1,            // Characters leave a tile
    TIME_EVENT      = 2,            // Certain point in gametime reached
    ACTION_EVENT    = 3,            // Character "acts" on a square 
    MAX_EVENTS      = 4
};

/**
 * Available 'actions', i.e. what happens when the event occurs
 */
enum
{
    ACTION_NOTHING  = 0,
    ACTION_SCRIPT   = 1,
    ACTION_PYFUNC   = 2,
    ACTION_CPPFUNC  = 3
};
#endif // SWIG
    
/**
 * Base class for events. You can create your own %event types that can
 * be handled by the event_list and event_handler by inheriting from
 * this class.
 *
 * Events are used to notify when certain things happen during the game.
 * They may either execute the "run" method of an exclusive python script
 * or a simple python callback defined elsewhere.
 */ 
class event
{
public:
    /**
     * Constructor. Needs to be called by any derived class!
     */
    event ();

    /** 
     * Destructor.
     */ 
    virtual ~event ();

    /**
     * Cleanup. Clears script and its arguments. 
     */
    void clear ();    
    
    /**
     * Get the event's type.
     *
     * @return type of the event
     */
    u_int8 type () const
    { 
        return Type;
    }
     
    /**
     * Return whether this event should be repeated.
     *
     * @return the number of times this event should be repeated or
     *      -1 in case it should be repeated unlimited times.
     */
    s_int32 repeat ()
    {
        if (Repeat > 0) Repeat--;
        
        return Repeat;
    }

    /**
     * Set whether this event should be repeated. A number greater than 0
     * will repeat the event that many times, a number less than 0 will
     * repeat the event forever. A number equal to 0 won't repeat the event.
     *
     *  @param count How often the event should be repeated.
     */
    void set_repeat (s_int32 count)
    {
        Repeat = count;
    }

    /**
     * @name Event Handling
     */
    //@{
    
    /**
     * Execute the associated python script or callback.
     * 
     * @param evnt The %event that triggered the execution.
     */ 
    virtual void execute (const event& evnt) = 0;

    /** 
     * Compare two events for equality.
     * 
     * @param evnt pointer to the %event to compare with.
     * @return \e true if the events are equal, \e false otherwise.
     */
    virtual bool equals (const event& evnt) = 0;

    //@}
    
    /** 
     * Sets a script to be executed whenever the event occurs.
     * 
     * @param filename filename of the script to set.
     * @param args The arguments to pass to the script's constructor
     */
    void set_script (string filename, PyObject * args = NULL);
    
    /**
     * Sets a python function/method to be executed whenever the
     * %event occurs.
     *
     * @warning the callback won't be saved with the %event. It
     * must be restored by the event's owner.
     *
     * @param callback The function or method to call.
     * @param args Additional arguments to pass to the callback.
     */
    void set_callback (PyObject *callback, PyObject *args = NULL);
     
#ifndef SWIG
    /**
     * Sets a C function/C++ method to be executed whenever the
     * %event occurs.
     *
     * @warning the callback won't be saved with the %event. It
     * must be restored by the event's owner.
     *
     * @param callback The callback, a function with no arguments
     *      returning void
     */
    void set_callback (const Functor0 & callback);
#endif // SWIG

    /**
     * @name Loading / Saving
     */
    //@{

    /** 
     * Saves the basic %event %data (such as the type or script data)
     * to a file. Call this method from the derived class.
     * 
     * @param out file where to save the %event.
     */ 
    virtual void put_state (ogzstream& out) const;
    
    /** 
     * Loads the basic %event %date from a file. Call this method from 
     * the derived class.
     * 
     * @param in file to load the %event from.
     * @return \e true if the %event could be loaded, \e false otherwise
     */
    virtual bool get_state (igzstream& in);

    //@}
    
protected:
#ifndef SWIG
    /**
     * @name Basic Event Data
     */
    //@{
    
    /**
     * Event type - see enum above.
     */ 
    u_int8 Type;

    /**
     * What happens if the event occurs - see enum above.
     */
    u_int8 Action;
    
    /**
     * Defines how often the %event should be repeated. <b>0</b> means
     * never, <b>-1</b> means infinitely and <b>n</b> (n > 0) means 
     * exactly n times.
     */
    s_int32 Repeat;
    
    /**
     * The Python script accociated with this %event. It is executed
     * whenever the %event gets triggered.
     */
    py_object *Script; 

    /**
     * The arguments passed to the script. This needs to be a PyTuple
     * or NULL if there are no arguments.
     */
    PyObject *Args;
    
    /**
     * Python callback that may be executed instead of the script.
     */
    py_callback *PyFunc;
    
    /**
     * C++ callback that may be executed when the %event gets triggered.
     */
    Functor0 Callback;
    //@}
#endif // SWIG
};

#endif // EVENT_H__
