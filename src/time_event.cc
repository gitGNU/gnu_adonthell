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

#include "time_event.h"
#include "gamedate.h"

// create a new time event
time_event::time_event (const string & time, bool absolute)
{
    Time = parse_date (time);
    if (!absolute) Time += gamedate::time ();
    Repeat = 0;
}

// specify the interval between two occurances of the event
void time_event::repeat (const string & interval, s_int32 count)
{
    Interval = parse_date (interval);
    Repeat = count;
}

// convert the time string to gametime minutes
u_int32 time_event::parse_date (const string & date)
{
    u_int32 minutes = 0, number = 0;

    for (u_int32 i = 0; i < date.length (); i++)
    {
        // got a number
        if (isdigit (date[i])
            number = 10 * number + atoi (date[i]);
        // got a letter
        else
        {
            switch (date[i])
            {
                // weeks
                case w:
                {
                    minutes += number * DAYS_PER_WEEK * HOURS_PER_DAY * 60;
                    break;
                }
                // days
                case d:
                {
                    minutes += number * HOURS_PER_DAY * 60;
                    break;
                }
                // hours
                case h:
                {
                    minutes += number * 60;
                    break;
                }
                // minutes
                case m:
                {
                    minutes += number;
                    break;
                }
                // error
                default:
                {
                    cerr << "*** time_event::parse_date: Unknown time specifier '"
                         << date[i] << "'\n" << flush;
                    break;
                }
            }

            number = 0;
        }
    }

    return minutes;
}