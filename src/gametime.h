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


/**
 * Length of a %game cycle, in milliseconds. Decrease it to speed up
 * the %game, increase it to slow the %game down.
 * This constant implicitly defines the maximum number of frames per 
 * second (FPS): FPS = 1000 / CYCLE_LENGTH.
 */ 
#define CYCLE_LENGTH 20

/** 
 * Defines the maximum number of frames to skip in order to keep the %game's
 * speed constant on slow machines. If updating the %game state and
 * drawing everything on %screen takes longer than CYCLE_LENGTH, we
 * skip frames so that the correct number of updates can be performed,
 * thus keeping the speed constant. However, we can't skip too many
 * frames, since that would perform in jerky animations and eventually
 * render the %game unplayable.
 */ 
#define FTS_LIMIT 20

class gametime
{
public:
    gametime (u_int32, float);      // constructor
    void tick (u_int32);            // Increase the gametime

    static void start_action ()
    {
        fts = 0;
        running = true; 
    }
    
    static void stop_action () 
    {
        running = false; 
    }

    /** 
     * Returns the number of updates to perform before drawing
     * the next frame. That is, it returns the number of frames to skip.
     * If the box Adonthell runs on is sufficiently fast, this number
     * will be 0. On slower machines, it can be as high as FTS_LIMIT.   
     * 
     * @return number of updates to perform before drawing a frame.
     * @see FTS_LIMIT
     */ 
    static u_int8 frames_to_skip ()
    {
        return fts;
    }

    /**
      * Call this after each run of the main loop to sync the %game's
      * speed to the machine it is running on. On faster machines it
      * delays the execution and for slower boxes it calculates the
      * number of frames to skip. If the engine should do 50 frames per
      * second, for example, but the main loop takes 40ms to perform,
      * every second frame will be skipped to keep the %game' speed
      * constant.
      * It also updates the internal clock.
      * 
      * @see frames_to_skip ()
      */
    static void update (); 
    
private:
    u_int32 ticks;                  // Realtime in Milliseconds
    u_int32 minute;                 // 1 min gametime in (realtime) milliseconds
    u_int32 time;                   // Gametime in "minutes"

    static bool running; 
    
    // Frames to skip.
    static u_int8 fts; 
    
    // Timers used to calculate the delay between 2 update() calls.
    static u_int32 timer1, timer2;
};

// To notify at a certain time
class time_event : public event
{
public:
    time_event ();
    void save (ogzstream&) const;   // Save event data

    u_int8 minute;                  // 0 - 59
    u_int8 m_step;                  // 0, 1, 2, ...
    u_int8 hour;                    // 0 - 23
    u_int8 h_step;                  // 0, 1, 2, ...
    u_int8 day;                     // 0 - 27
    u_int8 d_step;                  // 0, 1, 2, ...
    u_int32 time;                   // the actual gametime in minutes

protected:
    void execute (event& e);        // Run the event's script
    bool equals (event&);           // Compare two events
    bool load (igzstream&);         // Load event data
};

#endif // GAMETIME_H_
