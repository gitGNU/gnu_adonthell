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
 * @file gametime.h
 *
 * @author Kai Sterker
 * @brief Declares the gametime class.
 */

#ifndef GAMETIME_H_
#define GAMETIME_H_

#include "types.h"

/**
 * Length of a %game cycle, in milliseconds. Decrease it to speed up
 * the %game, increase it to slow the %game down.  This constant 
 * defines how often the state of the game world is updated. 
 * A cycle length of 13 means 1000/13 = 76.9 updates per second. 
 */ 
#define CYCLE_LENGTH 13

/**
 * Number of maximum displayed frames per second. This value affects
 * the renderer only, not the speed of the game itself. Limiting the 
 * frame rate prevents Adonthell from using all the CPU of the
 * machine it runs on (as long as the machine is fast enough).
 */ 
#define FRAME_RATE 50

/** 
 * Defines the maximum number of frames to skip in order to keep the %game's
 * speed constant on slow machines. If updating the %game state and
 * drawing everything on %screen takes longer than CYCLE_LENGTH, we
 * skip frames so that the correct number of updates can be performed,
 * thus keeping the speed constant. However, we can't skip too many
 * frames, since that would result in jerky animations and eventually
 * render the %game unplayable.
 */ 
#define FTS_LIMIT 20

/**
 * Tehe %gametime class makes the speed of the %game independent of 
 * the machine it runs on. This is achieved by keeping the number of 
 * updates to the %game state constant, no matter how fast or slow 
 * the machine. This won't work for very slow machines of course, 
 * but Adonthell will still be playable on a 100 Ghz CPU.
 */
class gametime
{
public:
    /**
     * Initialize the gametime class.
     *
     * @param rt_minutes Defines how many real life minutes make one
     *      gametime day. 
     */
    static void init (u_int16 rt_minutes);

    /**
     * Return the in-game time that passed since the last call to
     * this method. 
     *
     * @return %gametime in minutes.
     */
    static u_int32 minute ()
    {
        return Minute;
    }

    /**
     *
     */
    static void start_action ()
    {
        fts = 0;
        running = true; 
    }
    
    /**
     *
     */
    static void stop_action () 
    {
        running = false; 
    }

    /**
     * @name Methods to sync the %game speed to the machine it runs on
     */
    //@{
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
      * second, for example, but the main loop takes 26ms to perform,
      * every second frame will be skipped to keep the %game' speed
      * constant.
      * It also updates the internal clock.
      * 
      * @see frames_to_skip ()
      */
    static void update (); 
    //@}
    
private:
#ifndef SWIG
    // One minute of gametime in game cycles
    static u_int32 Minute;

    static bool running; 
    
    // Frames to skip.
    static u_int8 fts; 
    
    // Timers used to calculate the delay between 2 update() calls.
    static u_int32 timer1, timer2;
#endif // SWIG
};

#endif // GAMETIME_H_
