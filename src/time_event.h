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

#include "event.h"

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
     * Create a new time %event.
     * @param time The time when the %event should be raised. The string
     *      specifies week, day, hour and minute in the format
     *      "<number>w<number>d<number>h<number>m". If a number is 0, it
     *      can be omitted.
     * @param absolute Decides whether the given time is relative from now
     *      on, or an absolute time
     */
    time_event (const string & time, bool absolute = false);

    /**
     * Set whether the %event should be raised at fixed intervals
     * @param interval The time between two occurences of the %event.
     * @param count The number of times the %event shall be repeated.
     *      Specify -1 to repeat it an unlimited number of times.
     */
    void repeat (const string & interval, s_int32 count = -1);

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