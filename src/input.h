/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include "types.h"
#include "SDL_keysym.h"

#define LEFT_BUTTON 0
#define MIDDLE_BUTTON 1
#define RIGHT_BUTTON 2

class input
{
  //  static u_int8 keyboard_mode;
  static u_int8 * keystate;
  static u_int8 * p_keystate;
  static s_int32 keystatelength;
#ifndef SWIG
  static bool mouse_button[3];
#endif

  //  static bool is_unicode_on;
 public:
  static int FilterEvents(const SDL_Event *event);
  static void init();
  static void shutdown();
  static void update();
  static u_int16 last_key;
  static bool is_pushed(SDLKey key);
  static bool has_been_pushed(SDLKey key);

  static u_int16 mouse_posx, mouse_posy;
  
  static s_int32 get_next_key();
  static s_int32 get_next_unicode();

  static void set_key_repeat(int delay=SDL_DEFAULT_REPEAT_DELAY, int interval=SDL_DEFAULT_REPEAT_INTERVAL);
  static void clear_keys_queue();
};

#endif
