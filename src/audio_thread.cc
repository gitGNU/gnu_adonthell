/*
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifdef SDL

#include <stdlib.h>
#include "audio_thread.h"
#include "audio.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"

int audio_init() {

  // Open the audio device
  audio_in = new audio;
  audio_in->load_background(1);
  return(0);
}

void audio_update()
{
  // Keep looping music
  while (1==1) {
    if ( ! Mix_PlayingMusic() ) Mix_PlayMusic(audio_in->music, 0);
    SDL_Delay(100);
  }
}

void audio_cleanup() {
  free(audio_in);
}
#endif
