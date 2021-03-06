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
 * @file   event.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the %event class.
 * 
 */

#ifndef EVENT_H__
#define EVENT_H__

#include "callback.h"
#include "py_object.h"
#include "py_callback.h"

class event_list;

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
 * They may either execute the "run" method of an exclusive %python script
 * or a simple %python callback defined elsewhere.
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
     * @name Member access
     */
    //@{
    /**
     * Get the event's type.
     *
     * @return type of the %event
     */
    u_int8 type () const
    { 
        return Type;
    }

    /**
     * Get the event's id.
     *
     * @return id of the %event.
     */
    const string & id () const
    {
        return Id;
    }
    
    /**
     * Assign an id to the %event, so it may be retrieved from an
     * event_list later on, without having a pointer to it.
     *
     * @param id a string to identify the %event.
     */
    void set_id (const string & id)
    {
        Id = id;
    }
    
    /**
     * Test whether the %event is registered with the %event handler.
     *
     * @return \c true if this is the case, \c false otherwise.
     */
    bool registered () const
    {
        return Registered;
    }
#ifndef SWIG    
    /**
     * Set whether the %event is registered with the %event handler.
     *
     * @param reg Set to \c true if it is, to \c false otherwise.
     */
    void set_registered (bool reg)
    {
        Registered = reg;
    }
    
    /**
     * Tell the whether it is kept in an %event_list.
     *
     * @param list The %event_list this event has been added to.
     */
    void set_list (event_list *list);
#endif // SWIG    
    /**
     * Return whether this event should be repeated.
     *
     * @return the number of times this event should be repeated or
     *      -1 in case it should be repeated unlimited times.
     */
    s_int32 repeat () const
    {
        return Repeat;
    }
    
    /**
     * Set whether this event should be repeated. A number greater than 0
     * will execute the event that many times, a number less than 0 will
     * repeat the event forever. A number equal to 0 won't repeat the event.
     *
     * @param count How often the event should be repeated.
     */
    void set_repeat (s_int32 count)
    {
        Repeat = count;
    }
    //@}
    
    /**
     * @name Event Handling
     */
    //@{
    
    /**
     * Execute the associated python script or callback.
     * 
     * @param evnt The %event that triggered the execution.
     *
     * @return The number of times the %event needs to be repeated.
     */ 
    virtual s_int32 execute (const event* evnt) = 0;

    /** 
     * Compare two events for equality.
     * 
     * @param evnt pointer to the %event to compare with.
     * @return \e true if the events are equal, \e false otherwise.
     */
    virtual bool equals (const event* evnt) = 0;

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
     * @name Pausing / Resuming execution
     */
    //@{
    
    /**
     * Disable the %event temporarily. As long as it in this state, the
     * event will neither be executed, nor will its repeat-count change.
     * As long as the %event is paused, it will be removed from its
     * %event handler.
     */
    virtual void pause ();
    
    /**
     * Re-enable an %event that has been paused. Re-registers it with
     * its %event handler.
     */
    virtual void resume ();

    /**
     * Check whether the %event is temporarily disabled or not.
     * @return \b true if it is paused, \b false otherwise.                     
     */
    bool is_paused () const
    {
        return Paused;
    }
    //@}

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
     * Decrease the event's repeat count and return the number of repeats 
     * left. If the repeat-count reaches 0, the %event will be destroyed.
     *
     * @return the number of times this event should be repeated or
     *      -1 in case it should be repeated unlimited times.
     */
    s_int32 do_repeat ();

    /**
     * @name Basic Event Data
     */
    //@{
    
    /**
     * Event type - see enum above.
     */ 
    u_int8 Type;

    /**
     * (Optional) Id of the event
     */
    string Id;
    
    /**
     * What happens if the event occurs - see enum above.
     */
    u_int8 Action;
    
    /**
     * Whether the %event is registered with the %event handler.
     */
    bool Registered;
    
    /**
     * Whether the %event temporarily disabled or not. 
     */
    bool Paused;
    
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
    
    /**
     * The event_list this event is kept in.
     */
    event_list *List;
    //@}
#endif // SWIG
};

#endif // EVENT_H__
