/*
   $Id$

   Copyright (C) 2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file gametime.cc
 *
 * @author Kai Sterker
 * @brief Implements the gametime class.
  */

#include "gametime.h"
#include <SDL/SDL.h>

double gametime::Minute;
u_int32 gametime::timer1;
u_int32 gametime::timer2;
u_int8 gametime::fts;
bool gametime::running; 

// initialize the gametime class
void gametime::init (u_int16 rt_minutes)
{     
    fts = 0; 
    timer1 = 0;
    timer2 = 0;
    
    running = false;
    
    // Number of game cycles during rt_minutes realtime minutes
    double cycles = (60000 * rt_minutes) / (double) CYCLE_LENGTH;
    
    // Calculate how many game cycles make one gametime minute,
    // so that one gametime day lasts rt_minutes realtime minutes.
    Minute = cycles / 1440;
}

// Synchronize the game's speed to the machine it's running on.
void gametime::update () 
{
    // We declare this variable as static to avoid having to
    // perform the division every time.
    // Its value corresponds to the minimum delay between
    // two displayed frames (see FRAME_RATE).
    static u_int16 gfx_cycle_length = 1000 / FRAME_RATE;
    
    // Wait a bit if our machine performed too fast!
    while (1)
    {
        timer2 = SDL_GetTicks () - timer1;
        
        // if the mainloop was performed faster than one frame
        // should take, we sleep for the time remaining
        if (timer2 >= gfx_cycle_length) break;
        else SDL_Delay (3);
    }
    
    timer1 = SDL_GetTicks () - (timer2 % CYCLE_LENGTH);
    
    // Calculate the number of frames to skip (if the mainloop
    // takes longer than allowed, we drop frames (up to a certain
    // limit) to keep the game's speed constant.)
    fts = timer2 / CYCLE_LENGTH;
    if (fts > FTS_LIMIT) fts = FTS_LIMIT;
}
