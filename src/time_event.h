/*
   Copyright (C) 2002/2003 Kai Sterker <kai.sterker@gmail.com>
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
 * @file time_event.h
 *
 * @author Kai Sterker
 * @brief Declares the time_event class.
 */

#ifndef TIME_EVENT_H__
#define TIME_EVENT_H__

#include "event.h"

/**
 * The time %event executes the attached script or callback at a certain 
 * point in %game-time. This point can either be relative to the current
 * time, or absolute in time. In any case, this point should be in the
 * future. Time %event with an alarm time in the past will be triggered
 * at once.
 */
class time_event : public event
{
public:
    /**
     * @name Initialization
     */
    //@{

    /**
     * Create a new time %event.
     *
     * @param time The time when the %event should be raised. The string
     *      specifies week, day, hour, minute and 1/10 minute in the format
     *      "<number>w<number>d<number>h<number>m<number>t". If a number is 
     *      0, it can be omitted.
     * @param absolute Decides whether the given time is relative from now
     *      on, or an absolute time
     */
    time_event (const string & time, bool absolute = false);

#ifndef SWIG
    /**
     * Standard constructor.
     */
    time_event () : event ()
    {
        Type = TIME_EVENT;
        Repeat = 1;
    }
     
    /**
     * Create a new time %event. This constructor is primarily used for
     * raising time events.
     *
     * @param time The "alarm" time in %gametime minutes.
     */
    time_event (const u_int32 & time) : event ()
    {
        Type = TIME_EVENT;
        Time = time;
        Repeat = 1;
    }
#endif // SWIG
    
    /**
     * Set whether the %event should be raised at fixed intervals.
     *
     * @param interval The time between two occurences of the %event.
     * @param count The number of times the %event shall be repeated.
     *      Specify -1 to repeat it an unlimited number of times.
     */
    void set_repeat (const string & interval, s_int32 count = -1);
    //@}
    
    /**
     * @name Event Handling
     */
    //@{
    
    /**
     * Compare two time events for equality.
     *
     * @param evnt The time event to compare this to.
     * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
     */
    bool equals (const event * evnt)
    {
        time_event *e = (time_event *) evnt;
        return Time <= e->time ();
    }
    
    /**
     * Executes the script associated with this time %event. If the
     * event repeats it is re-registered with the %event handler.
     *
     * @param evnt The %event that triggered this time %event.
     *
     * @return The number of times the %event needs to be repeated.
     */
    s_int32 execute (const event * evnt);
    //@}
    
    /**
     * @name Loading / Saving
     */
    //@{
    
    /** 
     * Saves the basic %event %data (such as the type or script data)
     * to a file.
     * 
     * @param out file where to save the %event.
     */ 
    void put_state (ogzstream& out) const;
    
    /** 
     * Loads the basic %event %date from a file.
     * 
     * @param in file to load the %event from.
     * @return \e true if the %event could be loaded, \e false otherwise
     */
    bool get_state (igzstream& in);

    //@}

    /**
     * @name Pausing / Resuming execution
     */
    //@{
    
    /**
     * Disable the %event temporarily. As long as it in this state, the
     * event will neither be executed, nor will its repeat-count change.
     *
     * The alarm time of relative time events will be prolongued be the
     * time the event was paused. Absolute events will only be deferred
     * until they are resumed.
     */
    void pause ();
    
    /**
     * Re-enable an %event that has been paused. 
     *
     * Absolute events that are past their alarm time are executed at once.
     */
    void resume ();
    
    //@}
    
    /**
     * Get the event's "alarm" time, i.e. the time when it needs to be
     * executed.
     *
     * @return the "alarm" time in 1/10 %gametime minutes.
     */
    u_int32 time () const
    {
        return Time;
    }

private:
#ifndef SWIG
    // the time when the event shall be triggered
    u_int32 Time;

    // time that lies between two occurances of the event
    u_int32 Interval;
    
    // whether the alarm time is relative or absolute
    bool Absolute;
#endif // SWIG
};

#endif // TIME_EVENT_H__
