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

#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include "types.h"

class gametime
{
public:
    gametime (u_int32, float);      // constructor
    void tick (u_int32);            // Increase the gametime

private:
    u_int32 ticks;                  // Realtime in Milliseconds
    u_int32 minute;                 // 1 min gametime in (realtime) milliseconds
    u_int32 time;                   // Gametime in "minutes"
};

#endif // __GAMETIME_H__
