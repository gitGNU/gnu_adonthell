/*
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef SDL_MIXER

#include "SDL_mixer.h"

// We'll only load two waves into memory
#define NUM_WAVES 2
// We'll only load one .it file into memory
#define NUM_MUSIC 1

class audio
{
public:
  int background_volume;
  int effects_volume;
  Mix_Music *music[NUM_MUSIC];
  Mix_Chunk *sounds[NUM_WAVES];
  bool background_on;
  int current_background;
  bool background_paused;
  bool audio_initialized;
  int audio_rate;
  Uint16 audio_format;
  int audio_channels;

  // Use these to initialize/destroy sound connection
  audio();
  ~audio();
  void audio_cleanup(void);

  // Background Music functions:
    // Use these to load/unload background music
    int load_background(int);
    void unload_background(int);

    void set_background_volume(int);

    // Use only when music is loaded
    void pause_music(void);
    void unpause_music(void);

  // Sound Effect functions:
    // Use these to load/unload wave files
    void load_wave(void);
    void unload_wave(int wave);

    void play_wave(int channel, int sound);
    void play_background(int background);
};

#endif
#endif
