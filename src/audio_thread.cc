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

#include "audio_thread.h"
#include "SDL.h"
#include "SDL_mixer.h"

Mix_Music *music;

int audio_init() {

  // Open the audio device
  if ( Mix_OpenAudio(22050, AUDIO_S16, 1, 128) < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");
    return(1);
  }

  music = Mix_LoadMUS("audio/water.it");
  if (music == NULL) {
    fprintf(stderr, "Background music couldn't be loaded: %s\n", SDL_GetError());
    return(1);
  }

  // Keep looping music
  while (1==1) {
    if ( ! Mix_PlayingMusic() ) Mix_PlayMusic(music, 0);
    sleep(100);
  }

  return(0);
}

void audio_cleanup() {
  Mix_FreeMusic(music);
  Mix_CloseAudio();
}
#endif
