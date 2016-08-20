/*
   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kai.sterker@gmail.com>
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


/**
 * @file   event_list.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the event_list class.
 * 
 */
 

#ifndef EVENT_LIST_H__
#define EVENT_LIST_H__

#include <vector>
#include "event.h"

using std::string;

#ifndef SWIG
/**
 * Pointer to a function returning a newly allocated %event
 */
typedef event* (*new_event)();

#endif // SWIG

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
     * Constructor - creates an empty, unpaused %event_list
     */
    event_list ();
    
    /**
     * Destructor - unregisters and deletes all events owned by this list.
     */ 
    virtual ~event_list ();

    /**
     * Unregisters and deletes all events owned by this list.
     */ 
    void clear ();

    /**
     * @name List Operations
     */
    //@{
    /** 
     * Adds an %event to this list. The %event will be
     * registered with the %event_handler and the list will then
     * take care of it's deletion.
     * 
     * @param ev pointer to the %event to add.
     */
    void add_event (event* ev);

    /**
     * Removes an %event from the list. This is usually called when an
     * %event is destroyed.
     *
     * @param ev pointer to the %event to remove.
     */
    void remove_event (event* ev);
    
    /**
     * Try to retrieve the %event with given id from the list.
     *
     * @return a pointer to the %event, or \b NULL if it's not in the list.
     */
    event *get_event (const string & id);
    //@}
    
    /**
     * @name Pausing / Resuming execution
     */
    //@{
    /**
     * Disable any events associated with this %event_list. This will
     * effectively stop all actions of the %object the %event_list
     * belongs to, e.g. a NPC.
     */
    void pause ();
    
    /**
     * Re-enable the events associated with the %event_list, thus
     * 'awaking' the %object to life again. 
     */
    void resume ();

    /**
     * Check whether the %event list is temporarily disabled or not.
     * @return \b true if it is paused, \b false otherwise.                     
     */
    bool is_paused () const
    {
        return Paused;
    }
    //@}
    
#ifndef SWIG    
    /**
     * Register an %event for loading. Before the %event_list can load
     * an %event from file, it needs a callback function that returns
     * a new instance of the %event of the given type.
     *
     * @param type the type of the %event to register
     * @param e a callback returning a new instance of an %event of the 
     *      given type.
     *
     * @sa get_state ()
     */
    static void register_event (u_int8 type, new_event e);
#endif // SWIG

    /**
     * @name Loading / Saving
     */
    //@{
    /** 
     * Save the %event_list to a file.
     * 
     * @param out file where to save the %event_list.
     */ 
    void put_state (ogzstream& out) const;
    
    /** 
     * Loads the %event_list from a file and registers all loaded events.
     * @warning Before the %event_list can load an %event from file, it needs
     *     a callback function that returns a new instance of that %event.
     * 
     * @param in file to load the %event_list from.
     * 
     * @return \e true if the %event_list was loaded successfully, \e false
     *     otherwise.
     * @sa register_event ()
     */
    bool get_state (igzstream& in);
    //@}

#ifndef SWIG
protected:
    /**
     * List of events.
     */ 
    mutable std::vector<event*> Events;

private:
    /**
     * Whether this %event_list is paused or not. Events that are added
     * to a paused list will be paused as well.
     */
    bool Paused;

    /**
     * Array with callbacks that return a newly allocated instance of an %event.
     * The event's type is the postion of the according callback in the array.
     */
    static new_event instanciate_event[MAX_EVENTS];
#endif // SWIG
};

#ifndef SWIG
/**
 * Registers an %event with the %event_list, allowing it to load this %event
 * without knowing about it at compile time.
 */
#define REGISTER_EVENT(type,evt)\
    event_list::register_event (type, (new_event) &new_ ## evt);

/**
 * A function that returns a new instance of an %event.
 */
#define NEW_EVENT(evt)\
    event* new_ ## evt () { return (event*) new evt; }

#endif // SWIG
#endif // EVENT_LIST_H__
