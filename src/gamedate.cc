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
 * @file gamedate.cc
 *
 * @author Kai Sterker
 * @brief Implements the gamedate class.
 */

#include "gamedate.h"
#include "gametime.h"
#include "time_event.h"
#include "event_handler.h"

// gametime minutes spent in the gameworld so far
u_int32 gamedate::Time = 0;

// number of game cycles since the last gametime minute passed
float gamedate::Ticks = 0.0;

// Increase gametime 
void gamedate::update ()
{
    // fts contains the number of cycles that passed since the last
    // call to gamedate::update
    Ticks += gametime::frames_to_skip ();

    // check whether a in-game minute has passed
    while (Ticks >= gametime::minute ())
    {
        Ticks -= gametime::minute ();
        Time++;
        
        // raise time event
        event_handler::raise_event (time_event (Time));
    }
}

// load state from disk
bool gamedate::get_state (igzstream &in)
{
    // read the current date as (gametime) minutes since start of the game
    Time << in;
    
    return true;
}

// save state to disk
void gamedate::put_state (ogzstream &out)
{
    // write the time to disk
    Time >> out;
}

// calculate the current weekday
u_int16 gamedate::weekday ()
{
    return day () % DAYS_PER_WEEK;
}

// calculate the current day
u_int16 gamedate::day ()
{
    // how many minutes make one day
    static u_int day_in_minutes = 60 * HOURS_PER_DAY;

    return Time / day_in_minutes;
}

// calculate the hour of the current day
u_int16 gamedate::hour ()
{
    return (Time / 60) % HOURS_PER_DAY;
}

// calculate minute of the hour
u_int16 gamedate::minute ()
{
    return Time % 60; 
}

// convert the time string to gametime minutes
u_int32 gamedate::parse_time (const string & time)
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
