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
 * @file gamedate.h
 *
 * @author Kai Sterker
 * @brief Declares the gamedate class.
 */

#ifndef GAMEDATE_H__
#define GAMEDATE_H__

#include "fileops.h"

#ifndef SWIG
/**
 * The number of hours that make one gameworld day.
 */
#define HOURS_PER_DAY 24

/**
 * The number of days that make one gameworld week.
 */
#define DAYS_PER_WEEK 7
#endif // SWIG

/**
 * Keeps track of the time the player spent within the game so far. This
 * time span is given in %game time minutes, not real time.
 * %gamedate further includes functions to retrieve those minutes as day,
 * weekday, hour and minute values.
 */
class gamedate
{
public:

    /**
     * Update the %game date. Whenever a minute of %gametime has 
     * passed, a time event will be raised. This function needs to
     * be called from the main loop and uses 
     */        
    static void update ();

    /**
     * Get the current %gametime.
     * @return %gametime in minutes since start of the game.
     */
    static u_int32 time ()      { return Time; }

    /**
     * Get the current weekday.
     * @return weekday as a number between 0 and DAYS_PER_WEEK - 1
     */
    static u_int16 weekday ();
    /**
     * Returns the current day in the gameworld.
     * @return number of days spent in the gameworld, beginning with day 0.
     */
    static u_int16 day ();
    /**
     * Return the hour of the current day.
     * @return hour of the current day between 0 and HOURS_PER_DAY - 1
     */
    static u_int16 hour ();
    /**
     * Return the minute of the current hour.
     * @return minute of the current hour between 0 and 59.
     */
    static u_int16 minute ();

    /**
     * convert the time string to gametime minutes. The time string
     * has the format "<number>X", where X may be (w)eek, (d)ay,
     * (h)our or (m)inute. Several such pairs can be concatenated.
     * Valid examples are "1w1d1h", "30m1h" but also "1h1h".
     *
     * @param time The time format string.
     * @return The time represented by the string in minutes.
     */
    static u_int32 parse_time (const string & time);

    /**
     * Load the state of the %gamedate class from disk
     * @param in stream to read the state from
     * @return <b>true</b> if the state was successfully retrieved,
     *      <b>false</b> otherwise.
     */
    static bool get_state (igzstream &in);
    /**
     * Save the state of the %gamedate class to disk
     * @param out stream to write the state to
     */
    static void put_state (ogzstream &out);
    
private:
#ifndef SWIG
    // Time spent in the game in gametime minutes
    static u_int32 Time;

    // number of game cycles since the last gametime minute passed
    static float Ticks;
#endif // SWIG
};

#endif // GAMEDATE_H__
