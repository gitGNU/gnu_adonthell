/*
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifdef SDL_MIXER

#include <stdlib.h>
#include "audio_thread.h"
#include "audio.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"

int audio_init() {

  // Open the audio device
  audio_in = new audio;
  audio_in->load_background(0);
  audio_in->load_wave(); // This should be done elsewhere...
  audio_in->play_background(0);
  return(0);
}

void audio_update()
{

  // Keep audio up-to-date
  while (1==1) {

    // Keep busy until the audio device is connected
    while (audio_in == NULL) SDL_Delay(100);

    // Once the audio is connected, keep updating the audio stream
    if (audio_in->background_on == true) {
      if ( ! Mix_PlayingMusic() ) {
        Mix_PlayMusic(audio_in->music[audio_in->current_background], 2);
      }
    }
    SDL_Delay(100);
  }
}

void audio_cleanup() {
  free(audio_in);
}
#endif
