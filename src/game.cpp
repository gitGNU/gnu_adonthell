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

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "image.h"
#include "input.h"
#include "prefs.h"
#include "game.h"
#ifdef SDL_MIXER
#include "audio_thread.h"
#include "audio.h"
#endif

SDL_Thread *game::input_thread;
SDL_Thread *game::audio_thread;

void game::init(config &myconfig)
{
  screen::init_display ();
    
  input_thread = SDL_CreateThread(input_update, NULL);
  if ( input_thread != NULL) {
    fprintf(stderr, "User input thread started.\n");
  } else {
    fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
    exit(1);
  }
  
#ifdef SDL_MIXER
  
  audio_init(myconfig);
  audio_thread = SDL_CreateThread(audio_update, NULL);
  if ( audio_thread == NULL) {
    fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used\n");
  }
#endif  
}

void game::cleanup()
{
  fprintf(stderr, "Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);
  
#ifdef SDL_MIXER
  if (audio_thread != NULL) {
    SDL_KillThread(audio_thread);
    audio_cleanup();
  }
#endif
  SDL_Quit();
}
