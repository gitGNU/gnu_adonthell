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
 * Base class for events. You can create your own events types that can
 * be handled by the event handler by making them inherit from this class.
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
     */
    virtual void load (igzstream& in) = 0;

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
 *Base class for objects that want to register events
 *
 */ 
class event_list
{
public:
    /**
     * Destructor - unregisters and delete all event owned by this list.
     * 
     */ 
    virtual ~event_list ();

    /**
     * Unregisters and delete all event owned by this list.
     * 
     */ 
    void clear ();

    /** 
     * Adds an event to this list. The event will be
     * registered into the event_handler and the list will then
     * be responsible for it's deletion.
     * 
     * @param ev pointer to the event to add.
     */
    void add_event (event* ev);

    /** 
     * Save the event_list to a file.
     * 
     * @param out file where to save the event_list.
     */ 
    virtual void save (ogzstream& out) const = 0;
    
    /** 
     * Loads the event_list from a file and registers all loaded events.
     * 
     * @param in file to load the event_list from.
     */
    virtual void load (igzstream& in) = 0;

protected:
    /**
     * List of events.
     * 
     */ 
    mutable vector<event*> events;
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
    static vector<event*> handlers[MAX_EVENT];  // registered events storage
#endif
};

#endif // EVENT_H_
