/*
   $Id$

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
#include "globals.h"

int audio_init() {

  // Open the audio device
  audio_in = new audio;

  // Load our background music and SFX in
  audio_in->load_background(0, "audio/at-2.it");
  audio_in->load_background(1, "audio/at-1.it");
  audio_in->load_wave(0, "audio/at0.wav");
  audio_in->load_wave(1, "audio/at1.wav");

  // Start the background music playing
  audio_in->fade_in_background(0, 500);
  return(0);
}

int audio_update(void *data)
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
    SDL_Delay(1000);
  }
}

void audio_cleanup() {
  free(audio_in);
}
#endif
