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
 * @brief  Declares the event class.
 * 
 */

#ifndef EVENT_H__
#define EVENT_H__

#include "fileops.h" 
#include "py_object.h"

/**
 * Directory where event scripts reside.
 */ 
#define EVENTS_DIR "events."

/**
 * Available Event types.
 */ 
enum
{
    ENTER_EVENT     = 0,            // Characters reach a new tile
    LEAVE_EVENT     = 1,            // Characters leave a tile
    TIME_EVENT      = 2,            // Certain point in gametime reached
    ACTION_EVENT    = 3,            // Character "acts" on a square 
    MAX_EVENT       = 4
};

/**
 * Base class for events. You can create your own event types that can
 * be handled by the event list and event handler by inheriting them from
 * this class.
 */ 
class event
{
public:
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
     * @name Event Handling
     */
    //@{
    
    /**
     * Execute the associated python script.
     * 
     * @param evnt The event that triggered the execution.
     */ 
    virtual void execute (const event& evnt) = 0;

    /** 
     * Compare two events for equality.
     * 
     * @param evnt pointer to the event to compare with.
     * @return \e true if the events are equal, \e false otherwise.
     */
    virtual bool equals (const event& evnt) = 0;

    //@}
    
    /** 
     * Sets the script to be executed whenever the event occurs.
     * 
     * @param filename filename of the script to set.
     * @param args The arguments to pass to the script's constructor
     */
    void set_script (string filename, PyObject * args = NULL);
    
    /**
     * Sets the script to be executed whenever the event occurs.
     * This method allows to specify a script that is already
     * in use elsewhere.
     *
     * @warning After calling this method, both the event and the 
     * original owner share the same script instance. Therefore, the 
     * event will <b>not</b> save the script. When loading a game,
     * the original owner has to supply the event with the script.
     *
     * @param script reference to a script initialized elsewhere.
     */
    void set_shared_script (py_object & script);
    
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
     * For events that share their script with another class, Shared
     * has to be set <b>true</b>. This prevents the script from getting
     * saved to file.
     */
    bool Shared;
    
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
    py_object Script; 

    /**
     * The arguments passed to the script. This needs to be a PyTuple
     * or NULL if there are no arguments.
     */
    PyObject *Args;
    
    //@}
#endif // SWIG
};

#endif // EVENT_H__
