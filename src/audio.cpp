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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "prefs.h"
#include "audio.h"
#include "audio_loop.h"

extern unsigned short md_mode; // This is declared in MikMod

int audio::background_volume;
int audio::effects_volume;
loop_info *audio::loop[NUM_MUSIC];
Mix_Music *audio::music[NUM_MUSIC];
Mix_Chunk *audio::sounds[NUM_WAVES];
bool audio::background_on;
int audio::current_background;
bool audio::background_paused;
bool audio::audio_initialized=false;
int audio::audio_rate;
Uint16 audio::buffer_size;
Uint16 audio::audio_format;
int audio::audio_channels;

void audio::init (config *myconfig) {

  int i;  // Generic counter variable

  // Sample rate: 11025, 22050 or 44100 Hz
  switch( myconfig->audio_sample_rate ) {
    case 0: {
      audio_rate = 11025;
      break; }
    case 1: {
      audio_rate = 22050;
      break; }
    default: {
      audio_rate = 44100;
      break; }
  }
  
  // Output in signed 8/16-bit form
  audio_format = myconfig->audio_resolution == 0 ? AUDIO_S8 : AUDIO_S16; 

  // 1 is mono, 2 is stereo
  audio_channels = myconfig->audio_channels == 0 ? 1 : 2;  

  // 100... scales to percentages ;>
  background_volume = myconfig->audio_volume;   

  buffer_size = 4096;        // Audio buffer size
  effects_volume = 128;	     // Still figuring this one out...
  background_on = false;     // No background music is playing
  current_background = -1;   // No song currently playing
  background_paused = false; // Music isn't paused
  audio_initialized = false; // No audio connection yet

  // Mark all slots in sound and music arrays as empty
  for (i = 0; i < NUM_WAVES; i++) sounds[i] = NULL;
  for (i = 0; i < NUM_MUSIC; i++) music[i] = NULL; 

  // Try opening the audio device at our defaults
  i=Mix_OpenAudio(audio_rate, audio_format,
    audio_channels, buffer_size);

  // Activate sample interpolation to improve quality
  if (myconfig->audio_interpolation)
    md_mode |= 0x0200; // 0x0200 is DMODE_INTERP in MikMod

  // Now see what we got when opening the audio device
  // If we COULDN'T open the audio...
  if ( i < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");

  // If we COULD open the audio...
  } else {
    audio_initialized = true;
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    fprintf(stderr, "Audio started in %d Hz %d bit %s format.\n", audio_rate,
     (audio_format&0xFF), (audio_channels > 1) ? "stereo" : "mono");
    set_background_volume (background_volume);
  }
}

void audio::cleanup(void) {
  int i;
  background_on = false;    // no music is playing
  current_background = -1;  // No music is queued to play

  // Null out those tunes and sound effects
  for (i = 0; i < NUM_WAVES; i++) {
    unload_wave(i);
    sounds[i] = NULL;
  }
  for (i = 0; i < NUM_MUSIC; i++) {
    unload_background(i);
    music[i] = NULL;
  }

  // Close out audio connection
  Mix_CloseAudio();
  audio_initialized = false;
}

int audio::load_background(int slot, char *filename) {

  if (!audio_initialized) return(1);

  // Check for bad input
  if ((slot >= NUM_MUSIC) || (slot < 0)) {
    fprintf(stderr, "Error: Tried to put music in invalid slot.\n");
    return(1);
  }

  // If we are loading background music into the slot
  // the current background music is in...
  if (current_background == slot) {

    // This warns the audio thread to stop updating
    background_on = false;
    current_background = -1;

    Mix_HaltMusic();  // Just a precaution
    unload_background(slot);

  // If we aren't loading background music over
  // music currently playing ...
  } else {

    // Music already occupies that slot
    if (music[slot] != NULL)
      unload_background(slot);
  }
  
  // No music in slot, load new tune in, ...
  music[slot] = Mix_LoadMUS(filename);
  loop[slot] = new loop_info;

#ifdef OGG_VORBIS
  // read loop points and ...
  if (!loop[slot]->load (filename))
    loop[slot]->end = ov_raw_total (&music[slot]->data.ogg->vf, -1);

  // enable looping
  music[slot]->data.ogg->vf.callbacks.read_func = &ogg_read_callback;
#endif

  return(0);
}

void audio::unload_background(int slot) {
  if (music[slot] != NULL) {
    Mix_FreeMusic(music[slot]);
    music[slot] = NULL;
    delete loop[slot];
  }
}

void audio::pause_music(void) {
  Mix_PauseMusic();
}

void audio::unpause_music(void) {
  Mix_ResumeMusic();
}

// Accepts a percentage of the maximum volume level
// and clips out of bounds values to 0-100.
void audio::set_background_volume(int volume) {

  // Check for bad input
  if (volume < 0) {
    background_volume = 0;
  } else if (volume > 100) {
    background_volume = 100;
  } else
    background_volume = volume;

  // Scales 0-100% to 0-128
  Mix_VolumeMusic(int(background_volume * 1.28));
  //fprintf(stderr, "Volume: %d Mix: %d\n", background_volume, int(background_volume * 1.28));
}

// This should be done better, but I'll wait until
// I have enough sound effects to play with ;>
int audio::load_wave(int slot, char *filename) {

  if (!audio_initialized) return(1);

  // Check for bad input
  if ((slot >= NUM_WAVES) || (slot < 0)) {
    fprintf(stderr, "Error: Tried to put wave in invalid slot.\n");
    return(1);
  } else {
    sounds[slot] = Mix_LoadWAV(filename);
  }
  return(0);
}

void audio::unload_wave(int wave) {
  if (sounds[wave] != NULL) {
    Mix_FreeChunk(sounds[wave]);
    sounds[wave] = NULL;
  }
}

void audio::play_wave(int channel, int slot) {
  if ((slot > -1) && (slot < NUM_CHANNELS))
    if (sounds[slot] != NULL) Mix_PlayChannel(channel, sounds[slot], 0);
}

void audio::play_background(int slot) {
  if (music[slot] != NULL) {
    current_background = slot;
    background_on = true;
    Mix_PlayMusic(music[current_background], 2);
  }
}

void audio::fade_out_background(int time) {
  if (background_on == true) {
    Mix_FadeOutMusic(time);
#ifdef OGG_VORBIS
    music[current_background]->data.ogg->vf.callbacks.read_func = &fread;
#endif
    background_on = false;
    current_background = -1;
  }
}

void audio::fade_in_background(int slot, int time) {
  if ((background_on == false) && (music[slot] != NULL)) {
    current_background = slot;
    background_on = true;
    Mix_FadeInMusic(music[slot], 0, time);
  }
}

// Temporary convience function for testing
void audio::change_background(int slot, int time) {
  fade_out_background(time);
  fade_in_background(slot, time);
}

#ifdef OGG_VORBIS
OggVorbis_File* audio::get_vorbisfile ()
{
    return &music[current_background]->data.ogg->vf;
}
#endif
#endif
