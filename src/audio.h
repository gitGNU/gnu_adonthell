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
#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef SDL_MIXER

#include "SDL_mixer.h"

// We'll only load two waves into memory
#define NUM_WAVES 2
// We'll only load two .it files into memory
#define NUM_MUSIC 2
// We can play four SFX at once
#define NUM_CHANNELS 4

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
  Uint16 buffer_size;
  Uint16 audio_format;
  int audio_channels;

  // Use these to initialize/destroy sound connection
  audio();
  ~audio();
  void audio_cleanup(void);

  // Background Music functions:
    // Use these to load/unload background music
    int load_background(int slot, char *filename);
    void unload_background(int slot);

    // All input is clamped from 0 to 100
    void set_background_volume(int);

    // Use only when music is loaded
    void pause_music(void);
    void unpause_music(void);

    // Use these to load/unload wave files
    int load_wave(int slot, char *filename);
    void unload_wave(int slot);

    // Used to just start sounds playing
    void play_wave(int channel, int slot);
    void play_background(int slot);

    // Fade in and fade out background music (time in ms)
    // Fadeout unselects current tune when done
    void fade_in_background(int slot, int time);
    void fade_out_background(int time);

    // Temporary convience function to change background
    void change_background(int slot, int time);
};

extern audio *audio_in;

#endif
#endif
