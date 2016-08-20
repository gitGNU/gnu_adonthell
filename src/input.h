/*
   Copyright (C) 1999/2000/2001  Alexandre Courbot.
   Copyright (C) 2016  Kai Sterker
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   input.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker
 * 
 * @brief  Declares the input class.
 * 
 * 
 */



#ifndef INPUT_H__
#define INPUT_H__

#include "types.h"
#include <string>
#include <queue>
#include <SDL_keycode.h>

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
    static bool is_pushed(SDL_Keycode key);
#ifdef SWIG
    static bool is_pushed(char key)
    {
    	is_pushed((SDL_Keycode)key);
    }
#endif

    /** 
     * Returns whether a key has been pushed since last function call, false otherwise.
     * 
     * @param key key to test.
     * 
     * @return true if the key has been pushed since last call, false otherwise.
     */
    static bool has_been_pushed(SDL_Keycode key);
#ifdef SWIG
    static bool has_been_pushed(char key)
    {
    	has_been_pushed((SDL_Keycode)key);
    }
#endif
    
    /** 
     * Returns the code of the next key on the input queue.
     * 
     * 
     * @return Code of the next key that has been pushed.
     */
    static SDL_Keycode get_next_key();
     
    /** 
     * Returns the next text input on the input queue encoded as utf8.
     * 
     * 
     * @return utf8 representation of the next character that has been input.
     */
    static std::string get_next_unicode();

    static void start_text_input();
    static void stop_text_input();
    static bool is_text_input() { return text_input; }

    /** 
     * Totally clears the key queue.
     * 
     */
    static void clear_keys_queue();
    
private:
    static bool text_input;

    static u_int16 mouse_posx, mouse_posy;
    static u_int8 * keystate;
    static u_int8 * p_keystate;
    static s_int32 keystatelength;
#ifndef SWIG
    static bool mouse_button[3];
#endif
    static std::queue<std::string> input_queue;
};

#endif
