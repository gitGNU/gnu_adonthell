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
time_event::time_event (const string & time, bool absolute) : event ()
{
    Repeat = 1;
    Type = TIME_EVENT;
    Absolute = absolute;
    Time = gamedate::parse_time (time);
    if (!absolute) Time += gamedate::time ();
}

// specify the interval between two occurances of the event
void time_event::set_repeat (const string & interval, s_int32 count)
{
    Interval = gamedate::parse_time (interval);
    Repeat = count;
}

// execute the time event
s_int32 time_event::execute (const event * evnt)
{
    // nothing needs be passed to the script; it can get the
    // current time from the gametime class if it is needed.
    switch (Action)
    {
        case ACTION_SCRIPT:
        {
            Script->run ();
            break;
        }
        
        case ACTION_PYFUNC:
        {
            PyFunc->callback_func0 ();
            break;
        }
        
        case ACTION_CPPFUNC:
        {
            Callback ();
            break;
        }
        
        default: break;
    }
    
    // when the script needs be repeated, do so.
    if (Repeat > 1) Time += Interval;
    
    return do_repeat ();
}

// disable the event temporarily
void time_event::pause ()
{
    event::pause ();
    
    // save time 'til relative event is raised
    if (!Absolute) Time -= gamedate::time ();
}

// enable a previously paused event
void time_event::resume ()
{
    event::resume ();
    
    // restore alarm time for relative event
    if (!Absolute) Time += gamedate::time ();
}

// Save time event to file
void time_event::put_state (ogzstream& out) const
{
    // save basic event data first
    event::put_state (out);
    
    // save time event data
    Time >> out;
    Interval >> out;
    Absolute >> out;
}

// load time event from file
bool time_event::get_state (igzstream& in)
{
    // get basic event data
    event::get_state (in);
    
    // get time event data
    Time << in;
    Interval << in;
    Absolute << in;
    
    return true;
}
