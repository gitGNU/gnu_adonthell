/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <string>
#include "event.h"
#include "gametime.h"
#include <SDL/SDL.h>

u_int32 gametime::timer1;
u_int32 gametime::timer2;
u_int8 gametime::ftd = 0;
bool gametime::running = false; 

gametime::gametime (u_int32 start, float ratio)
{
    ticks = 0;
    time = start;
    
    ftd = 0; 
    timer1 = 0;
    timer2 = 0;
    
    // This decides how many realtime milliseconds make one gametime minute
    minute = (int) (60000 * ratio);
}

// Increase gametime 
void gametime::tick (u_int32 val)
{
    ticks += val;

    // notify the eventhandler whenever a minute has passed
    if (ticks >= minute)
    {
        ticks -= minute;
        time++;

        time_event t;
        t.time = time;
        event_handler::raise_event (t);
    }
}


void gametime::update () 
{
    // Syncronize the game's speed to the machine it's running on.
    while (1)
    {
        timer2 = SDL_GetTicks () - timer1;
        
        if (timer2 >= CYCLE_LENGTH)
            break;
        else if (timer2 > 3)
            SDL_Delay (timer2 - 2);
    }
    
    timer1 = SDL_GetTicks () - (timer2 % CYCLE_LENGTH);
    
    // Calculate the number of frames to perform.
    ftd = timer2 / CYCLE_LENGTH;
    if (ftd > FTD_LIMIT)
        ftd = FTD_LIMIT;
}

time_event::time_event ()
{
    time =minute = hour = day =  0;
    m_step = h_step = d_step = 1;
    type = TIME_EVENT;
}

// Save a time_event to file
void time_event::save (ogzstream& out) const
{
    type >> out;
    minute >> out;
    m_step >> out;
    hour >> out;
    h_step >> out;
    day >> out;
    d_step >> out;
    script_file () >> out;
}

// Load a time event from file
void time_event::load (igzstream& f)
{
    string s; 
    minute << f;
    m_step << f;
    hour << f;
    h_step << f;
    day << f;
    d_step << f;
    s << f;
    set_script (s); 
}

// Execute time event's script
void time_event::execute (event &e)
{
    time_event t = (time_event&) e; 
    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i}", "minute", (int) t.minute, 
        "hour", (int) t.hour, "day", (int) t.day);
    script.set_locals (locals);
    script.run ();
    script.set_locals (NULL); 
    // Cleanup
    Py_DECREF (locals);
#ifdef _DEBUG_
    show_traceback ();
#endif // _DEBUG_
}

// Check whether this time_even matches a given gametime
bool time_event::equals (event &e)
{
    time_event t = (time_event &) e;
    u_int32 time = t.time % 40320;
    u_int32 d, h, m = time % 60;
    
    if (m_step != 0) {
        if ((m - minute) % m_step != 0 || m < minute) return 0;
    }
    else
        if (m != minute) return 0;

    // this is the current hour
    h = ((time - m) % 1440) / 60;

    if (h_step != 0) {
        if ((h - hour) % h_step != 0 || h < hour) return 0;
    }
    else
        if (h != hour) return 0;
         
    // this is the current day
    d = (time - (m + h * 60)) / 1440;
    
    if (d_step != 0) {
        if ((d - day) % d_step != 0 || d < day) return 0; 
    }
    else
        if (d != day) return 0;

    // if the event matches, we set the actual minute, hour and day,
    // in case it is needed in the script
    t.minute = m;
    t.hour = h;
    t.day = d;
    
    return 1;
}
