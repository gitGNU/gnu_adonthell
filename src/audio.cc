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

#include <iostream.h>
#include "audio.h"
#include "SDL.h"
#include "SDL_mixer.h"

audio::audio() {

  int i;  // Generic counter variable

  audio_rate = 22050;        // Sample at 22050Hz
  audio_format = AUDIO_S16;  // Output in signed 16-bit form
  audio_channels = 2;        // 1 is mono, 2 is stereo
  background_volume = 128;   // 128... seems to work well ;>
  effects_volume = 128;	     // Still figuring this one out...
  background_on = false;     // No background music is playing
  current_background = -1;   // No song currently playing
  background_paused = false; // Music isn't paused
  audio_initialized = false; // No audio connection yet

  // Unload any junk in memory (if there is any)
  for (i = 0; i < NUM_WAVES; i++) unload_wave(i);
  for (i = 0; i < NUM_MUSIC; i++) unload_background(i);

  // Try opening the audio device at our defaults
  i=Mix_OpenAudio(audio_rate, audio_format,
    audio_channels, background_volume);

  // Now see what we got when opening the audio device
  // If we COULDN'T open the audio...
  if ( i < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");

  // If we COULD open the audio...
  } else {
    audio_initialized = true;
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    printf("Audio thread started in %d Hz %d bit %s format.\n ", audio_rate,
     (audio_format&0xFF), (audio_channels > 1) ? "stereo" : "mono");
  }
}

audio::~audio() {
  audio_cleanup();
}

void audio::audio_cleanup(void) {
  int i;
  background_on = false;    // no music is playing
  current_background = -1;  // No music is queued to play

  // Null out those tunes and sound effects
  for (i = 0; i < NUM_WAVES; i++) unload_wave(i);
  for (i = 0; i < NUM_MUSIC; i++) unload_background(i);

  // Close out audio connection
  Mix_CloseAudio();
  audio_initialized = false;
}

int audio::load_background(int new_background) {

  // Check for bad input
  if ((new_background >= NUM_MUSIC) || (new_background < 0))
    return(1);

  // If we are loading background music into the slot
  // the current background music is in...
  if (current_background == new_background) {

    // This warns the audio thread to stop updating
    background_on = false;

    current_background = -1;
    Mix_HaltMusic();  // Just a precaution
    unload_background(new_background);
    background_on = false;

    // Wow! Recursive call!
    load_background(new_background);

  // If we aren't loading background music over
  // music currently playing ...
  } else {

    // Music already occupies that slot
    if (music[new_background] != NULL)
      unload_background(new_background);

    // No music in slot, load new tune in
    music[new_background] = Mix_LoadMUS("audio/water.it");
  }
  return(0);
}

void audio::unload_background(int tune) {
  if (music[tune] != NULL) Mix_FreeMusic(music[tune]);
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

// This should be done better, but I'll wait until
// I have enough sound effects to play with ;>
void audio::load_wave(void) {
  sounds[0] = Mix_LoadWAV("audio/at0.wav");
  sounds[1] = Mix_LoadWAV("audio/at1.wav");
}

void audio::unload_wave(int wave) {
  if (sounds[wave] != NULL) Mix_FreeChunk(sounds[wave]);
}

void audio::play_wave(int channel, int sound) {
  if (sounds[sound] != NULL) Mix_PlayChannel(channel, sounds[sound], 0);
}

void audio::play_background(int background) {
  if (music[background] != NULL) {
    current_background = background;
    background_on = true;
  }
}

#endif
