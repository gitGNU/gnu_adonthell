/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file time_event.h
 *
 * @author Kai Sterker
 * @brief Declares the time_event class.
 */

#ifndef TIME_EVENT_H__
#define TIME_EVENT_H__

#include <string>
#include "event.h"

using std::string;

/**
 * The time %event executes the attached python script at a certain point in
 * %game-time. This point can be specified relative to the current %gametime,
 * or absolute. Further, an interval can be given that determines when the
 * event will be repeated.
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
     *      specifies week, day, hour and minute in the format
     *      "<number>w<number>d<number>h<number>m". If a number is 0, it
     *      can be omitted.
     * @param absolute Decides whether the given time is relative from now
     *      on, or an absolute time
     */
    time_event (const string & time, bool absolute = false);

#ifndef SWIG
    /**
     * Create a new time %event. This constructor is primarily used for
     * raising time events.
     *
     * @param time The "alarm" time in %gametime minutes.
     */
    time_event (const u_int32 & time)
    {
        Type = TIME_EVENT;
        Time = time;
    }
#endif // SWIG
    
    /**
     * Set whether the %event should be raised at fixed intervals.
     *
     * @param interval The time between two occurences of the %event.
     * @param count The number of times the %event shall be repeated.
     *      Specify -1 to repeat it an unlimited number of times.
     */
    void repeat (const string & interval, s_int32 count = -1);
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
    bool equals (event & evnt)
    {
        return Time == event->time ();
    }
    
    /**
     * Executes the script associated with this time %event. If the
     * event repeats it is re-registered with the %event handler.
     *
     * @param evnt The %event that triggered this time %event.
     */
    void execute (event & evnt);
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
     * Get the event's "alarm" time, i.e. the time when it needs to be
     * executed.
     *
     * @return the "alarm" time in %gametime minutes.
     */
    u_int32 time ()
    {
        return Time;
    }
    
private:
#ifndef SWIG
    // convert the time string to gametime minutes
    u_int32 parse_time (const string & time);

    // the time when the event shall be triggered
    u_int32 Time;

    // time that lies between two occurances of the event
    u_int32 Interval;
#endif // SWIG
};

#endif // TIME_EVENT_H__
