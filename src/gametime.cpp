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

#include <iostream.h>
#include "gametime.h"

gametime::gametime (u_int32 start, float ratio)
{
    ticks = 0;
    time = start;

    // This decides how many realtime milliseconds make one gametime minute
    minute = (int) (60000 * ratio);
}

// Increase gametime 
void gametime::tick (u_int32 val)
{
    ticks += val;

    if (ticks >= minute)
    {
        ticks -= minute;
        time++;

        // Test
        cout << "Gametime: " << time << ":00 minute(s)\n" << flush;
    }
}
