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
 * @file   event_list.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the event_list class.
 * 
 */
 

#ifndef EVENT_LIST_H__
#define EVENT_LIST_H__

#include <vector>
#include "event.h"

/**
 * Pointer to a function returning a newly allocated %event
 */
typedef event* (*new_event)();


/**
 * Base class for objects that want to register events. It keeps track of
 * all the events an object has registered with the event_handler and can
 * automatically unregister them when the object is deallocated.
 *
 * It also provides the functionality to load and save the states of 
 * events in its list.
 *
 * Objects making use of events should use the %event_list instead of
 * handling events themselves.
 */ 
class event_list
{
public:
    /**
     * Destructor - unregisters and deletes all events owned by this list.
     */ 
    virtual ~event_list ();

    /**
     * Unregisters and deletes all events owned by this list.
     */ 
    void clear ();

    /** 
     * Adds an event to this list. The event will be
     * registered with the event_handler and the list will then
     * take care of it's deletion.
     * 
     * @param ev pointer to the event to add.
     */
    void add_event (event* ev);

    /**
     * Register an event for loading. Before the event_list can load
     * an event from file, it needs a callback function that returns
     * a new instance of the event of the given type.
     *
     * @param type the type of the event to register
     * @param e a callback returning a new instance of an event of the 
     *      given type.
     *
     * @sa load ()
     */
    static void register_event (u_int8 type, new_event e);
    
    /** 
     * Save the event_list to a file.
     * 
     * @param out file where to save the event_list.
     */ 
    void put_state (ogzstream& out) const;
    
    /** 
     * Loads the event_list from a file and registers all loaded events.
     * @warning Before the event_list can load an event from file, it needs
     *          a callback function that returns a new instance of that event.
     * 
     * @param in file to load the event_list from.
     * 
     * @return \e true if the event_list was loaded successfully, \e false otherwise.
     * @sa register_event ()
     */
    bool get_state (igzstream& in);

#ifndef SWIG
protected:
    /**
     * List of events.
     */ 
    mutable vector<event*> events;

private:
    /**
     * Array with callbacks that return a newly allocated instance of an event.
     * The event's type is the postion of the according callback in the array.
     */
    static new_event instanciate_event[MAX_EVENT];
#endif // SWIG
};

#ifndef SWIG
/**
 * A function that returns a new instance of an event.
 */
#define NEW_EVENT(evt)\
    event* new_ ## evt () { return (event*) new evt; }
/**
 * Registers an event with the event_list, allowing it to load this event
 * without knowing about it at compile time.
 */
#define REGISTER_EVENT(type,evt)\
    event_list::register_event (type, (new_event) &new_ ## evt);

#endif // SWIG

#endif // EVENT_LIST_H__
