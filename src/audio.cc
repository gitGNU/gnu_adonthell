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
#ifdef SDL

#include "audio.h"
#include "SDL.h"
#include "SDL_mixer.h"

#define NUM_WAVES 2

Mix_Chunk *sounds[NUM_WAVES];

audio::audio() {

  music = NULL;
  background_volume = 128;
  background_on = false;
  current_background = 0;
  background_paused = false;
  audio_initialized = false;

  // Open the audio device
  if ( Mix_OpenAudio(22050, AUDIO_S16, 1, 128) < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");
  } else {
    audio_initialized = true;
    load_waves();
  }
}

audio::~audio() {
  audio_cleanup();
}

void audio::audio_cleanup(void) {
  current_background = 0;
  background_on = false;
  unload_waves();
  unload_background();
  Mix_CloseAudio();
  audio_initialized = false;
}

void audio::load_background(int new_background) {
  unload_background();
  if (audio_initialized == true) {

    switch (new_background) {
      case 1:   // Default music
        music = Mix_LoadMUS("audio/water.it");
        current_background = 1;
        background_on = true;
        break;
      case 0:   // No music
      default:
        current_background = 0;
        background_on = false;
        break;
    }
  }
}

void audio::unload_background(void) {
  if (music != NULL) Mix_FreeMusic(music);
}

void audio::pause_music(void) {
  Mix_PauseMusic();
}

void audio::unpause_music(void) {
  Mix_ResumeMusic();
}

void audio::set_background_volume(int volume) {
  Mix_VolumeMusic(volume);
  background_volume = volume;
}

void audio::load_waves(void) {
  sounds[0] = Mix_LoadWAV("audio/at0.wav");
  sounds[1] = Mix_LoadWAV("audio/at1.wav");
}

void audio::unload_waves(void) {
  Mix_FreeChunk(sounds[0]);
  Mix_FreeChunk(sounds[1]);
}

void audio::play_wave(int channel, int sound) {
  Mix_PlayChannel(channel, sounds[sound], 0);
}

#endif
