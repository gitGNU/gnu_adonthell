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

#ifndef GAMETIME_H_
#define GAMETIME_H_

#include "event.h"


/** The %game slow down if there are more than FTD_LIMIT numbers of frames
 *  to calculate before displaying, in which case it calculates exactly
 *  FTD_LIMIT frames.
 */ 
#define FTD_LIMIT 100

/**
 * Length of a game cycle, in milliseconds.
 * Decrease it to speed up the game, increase it
 * to slow the game down.
 */ 
#define CYCLE_LENGTH 13


class gametime
{
public:
    gametime (u_int32, float);      // constructor
    void tick (u_int32);            // Increase the gametime

    static void start_action ()
    {
        ftd = 1;
        running = true; 
    }
    
    static void stop_action () 
    {
        running = false; 
    }

    /** 
     * Returns the number of frames that must be calculated before
     * displaying the game.
     * 
     * @return number of frames to calculate before displaying the game.
     */ 
    static u_int8 frames_to_do ()
    {
        return ftd;
    }

    static void update (); 
    
private:
    u_int32 ticks;                  // Realtime in Milliseconds
    u_int32 minute;                 // 1 min gametime in (realtime) milliseconds
    u_int32 time;                   // Gametime in "minutes"

    static bool running; 
    
    /// Frames to do.
    static u_int8 ftd; 
    
    /// Timers used to calculate the delay between 2 show () calls.
    static u_int32 timer1, timer2;

};

// To notify at a certain time
class time_event : public event
{
public:
    time_event ();
    void save (ogzstream&) const;                         // Save event data

    u_int8 minute;                              // 0 - 59
    u_int8 m_step;                              // 0, 1, 2, ...
    u_int8 hour;                                // 0 - 23
    u_int8 h_step;                              // 0, 1, 2, ...
    u_int8 day;                                 // 0 - 27
    u_int8 d_step;                              // 0, 1, 2, ...
    u_int32 time;                               // the actual gametime in minutes

protected:
    void execute (event& e);                      // Run the event's script
    bool equals (event&);                       // Compare two events
    void load (igzstream&);                         // Load event data
};

#endif // GAMETIME_H_
