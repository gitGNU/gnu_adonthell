/*
   $Id:

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _game_h
#define _game_h

#include "SDL.h"
#include "SDL_thread.h"
#include "prefs.h"

class game
{
  static SDL_Thread *input_thread;
  static SDL_Thread *audio_thread;
  
 public:
  static void init(config&);
  static void cleanup();
};

#endif
