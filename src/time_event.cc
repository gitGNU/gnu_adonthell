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
 * @file time_event.cc
 *
 * @author Kai Sterker
 * @brief Implements the time_event class.
 */

#include "stdlib.h"
#include "time_event.h"
#include "gamedate.h"

// create a new time event
time_event::time_event (const string & time, bool absolute)
{
    Repeat = 0;
    Type = TIME_EVENT;
    Time = parse_time (time);
    if (!absolute) Time += gamedate::time ();
}

// specify the interval between two occurances of the event
void time_event::repeat (const string & interval, s_int32 count)
{
    Interval = parse_time (interval);
    Repeat = count;
}

// execute the time event
void time_event::execute (const event & evnt)
{
    // nothing needs be passed to the script; it can get the
    // current time from the gametime class if it is needed.
    Script.run ();
    
    if (Repeat > 0) Repeat--;
     
    // when the script needs be repeated, do so.
    if (Repeat != 0) Time += Interval;
}

// Save time event to file
void time_event::put_state (ogzstream& out) const
{
    // save basic event data first
    event::put_state (out);
    
    // save time event data
    Time >> out;
    Interval >> out;
}

// load time event from file
bool time_event::get_state (igzstream& in)
{
    // get basic event data
    event::get_state (in);
    
    // get time event data
    Time << in;
    Interval << in;
    
    return true;
}

// convert the time string to gametime minutes
u_int32 time_event::parse_time (const string & time)
{
    u_int32 minutes = 0, number = 0;
    char num[2] = "0";
    
    for (u_int32 i = 0; i < time.length (); i++)
    {
        // got a number
        if (isdigit (time[i]))
        {
            num[0] = time[i];
            number = 10 * number + atoi (num);
        }
        // got a letter
        else
        {
            switch (time[i])
            {
                // weeks
                case 'w':
                {
                    minutes += number * DAYS_PER_WEEK * HOURS_PER_DAY * 60;
                    break;
                }
                // days
                case 'd':
                {
                    minutes += number * HOURS_PER_DAY * 60;
                    break;
                }
                // hours
                case 'h':
                {
                    minutes += number * 60;
                    break;
                }
                // minutes
                case 'm':
                {
                    minutes += number;
                    break;
                }
                // error
                default:
                {
                    fprintf (stderr, "*** time_event::parse_date: Unknown time specifier '%c'\n", time[i]);
                    break;
                }
            }

            number = 0;
        }
    }

    return minutes;
}
