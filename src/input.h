/*
   $Id$

   Copyright (C) 1999/2000/2001  Alexandre Courbot.
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   input.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the input class.
 * 
 * 
 */



#ifndef INPUT_H__
#define INPUT_H__

#include "types.h"
#include "SDL_keysym.h"


/**
 * Handles keyboard and mouse input.
 *
 * @todo Rewrite it!
 * 
 */ 
class input
{
public:

    /** 
     * Initialise the input system.
     * 
     */
    static void init();

    /** 
     * Free resources occupied by the input system.
     * 
     */
    static void shutdown();

    /** 
     * Update the input state.
     * 
     */
    static void update();

    /** 
     * Returns whether a key is currently pushed or not.
     * 
     * @param key key to test.
     * 
     * @return true if key is currently pushed, false otherwise.
     */
    static bool is_pushed(SDLKey key);

    /** 
     * Returns whether a key has been pushed since last function call, false otherwise.
     * 
     * @param key key to test.
     * 
     * @return true if the key has been pushed since last call, false otherwise.
     */
    static bool has_been_pushed(SDLKey key);
    
    
    /** 
     * Returns the code of the next key on the input queue.
     * 
     * 
     * @return Code of the next key that has been pushed.
     */
    static s_int32 get_next_key();
     
    /** 
     * Returns the next unicode on the input queue.
     * 
     * 
     * @return Unicode of the next key that has been pushed.
     */
    static s_int32 get_next_unicode();
    
    /** 
     * Sets whether the key repeat is active or not.
     * 
     * @param delay delay (in ms) before repetition.
     * @param interval interval (in ms) between repetitions.
     */
    static void set_key_repeat(int delay=SDL_DEFAULT_REPEAT_DELAY, int interval=SDL_DEFAULT_REPEAT_INTERVAL);

    /** 
     * Totally clears the key queue.
     * 
     */
    static void clear_keys_queue();
    

private:
    static u_int16 last_key;

    static int filterevents(const SDL_Event *event);
    
    static u_int16 mouse_posx, mouse_posy;
    static u_int8 * keystate;
    static u_int8 * p_keystate;
    static s_int32 keystatelength;
#ifndef SWIG
    static bool mouse_button[3];
#endif
    
};

#endif
