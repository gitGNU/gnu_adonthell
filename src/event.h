/*
   $Id$

   Copyright (C) 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
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
 * @brief  Declares the event_list, event and event_handler class.
 * 
 * 
 */
 

#ifndef EVENT_H_
#define EVENT_H_

#include <vector> 
#include "fileops.h" 
#include "py_script.h"

/**
 * Directory where events scripts resides.
 * 
 */ 
#define EVENTS_DIR "scripts/events/"


class event_handler;

/**
 * Events types.
 * 
 */ 
enum
{
    ENTER_EVENT = 0,                            // Characters reach a new tile
    LEAVE_EVENT = 1,                            // Characters leave a tile
    TIME_EVENT = 2,                             // A minute of gametime passed
    MAX_EVENT = 3
};

/**
 * Base class for events. You can create your own event types that can
 * be handled by the event list and event handler by inheriting them from
 * this class.
 * 
 */ 
class event
{
public:

    /** 
     * Destructor.
     *  
     */ 
     virtual ~event ();

    /** 
     * Returns the file name of the event's script.
     * 
     * 
     * @return file name of the script.
     */
    string script_file () const
    {
        return script.script_file ();
    }
    
    /** 
     * Save the event to a file.
     * 
     * @param out file where to save the event.
     */ 
    virtual void save (ogzstream& out) const = 0;
    
    /** 
     * Loads an event from a file.
     * 
     * @param in file to load the event from.
     *
     * @return \e true if the event could be loaded, \e false otherwise
     */
    virtual bool load (igzstream& in) = 0;

    /** 
     * Sets the script for an event.
     * 
     * @param filename filename of the script to set.
     */
    void set_script (string filename);
    
protected:
    /**
     * Event type - see enum above.
     * 
     */ 
    u_int8 type;

    /**
     * Script object.
     * 
     */
    py_script script; 

    /**
     * Execute the script.
     * 
     */ 
    virtual void execute (event& e) = 0;

    /** 
     * Compare two events for equality.
     * 
     * @param ev pointer to the event to compare with.
     * 
     * @return \e true if the events are equal, \e false otherwise.
     */
    virtual bool equals (event& ev) = 0;
    
#ifndef SWIG
    friend class event_handler;
#endif
    
};

/**
 * Pointer to a function returning a newly allocated event
 *
 */
typedef event* (*new_event)();


/**
 * Base class for objects that want to register events
 *
 */ 
class event_list
{
public:
    /**
     * Destructor - unregisters and deletes all events owned by this list.
     * 
     */ 
    virtual ~event_list ();

    /**
     * Unregisters and deletes all events owned by this list.
     * 
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
     * @param e a callback returning a new instance of an event of the given type.
     *
     * @sa load ()
     */
    static void register_event (int type, new_event e);
    
    /** 
     * Save the event_list to a file.
     * 
     * @param out file where to save the event_list.
     */ 
    void save (ogzstream& out) const;
    
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
    bool load (igzstream& in);

#ifndef SWIG
protected:
    /**
     * List of events.
     * 
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

/**
 * Keeps track of registered scripts, recieves triggered events 
 * and executes scripts handling those events
 *
 */ 
class event_handler
{
public:
    /** 
     * Registers an event.
     * 
     * @param ev pointer to the event to register.
     */
    static void register_event (event* ev);

    /** 
     * Unregister an event.
     * 
     * @param event* pointer to the event to unregister.
     */
    static void remove_event (event* ev);

    /** 
     * Check if an event corresponding to ev exists, and execute it. 
     * 
     * @param ev event to raise.
     */
    static void raise_event (event& ev);
    
private:
#ifndef SWIG
    static vector<event*> handlers[MAX_EVENT];      // registered events storage
#endif
};

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

#endif // EVENT_H_
