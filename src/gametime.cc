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
        event_handler::raise_event (&t);
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
