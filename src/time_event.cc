/*
   $Id$

   Copyright (C) 2002/2003/2004 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
    if (Repeat != 0) Time += Interval;
    
    return do_repeat ();
}

// disable the event temporarily
void time_event::pause ()
{
    // save time 'til relative event is raised
    if (!Absolute) Time -= gamedate::time ();

    event::pause ();
}

// enable a previously paused event
void time_event::resume ()
{
    // restore alarm time for relative event
    if (!Absolute) Time += gamedate::time ();

    event::resume ();
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
