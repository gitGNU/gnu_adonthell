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

#include "types.h"
#include "SDL_mixer.h"
#include "prefs.h"
#include "py_object.h"
#include <string>
// #include "audio_loop.h"

// We'll only load five waves into memory
#define NUM_WAVES 5
// We'll only load three .ogg files into memory
#define NUM_MUSIC 3
// We can play four SFX at once
#define NUM_CHANNELS 4

class audio
{
public:
    static void init(config*);
    static void cleanup(void);

    // state saving/loading
    static s_int8 put_state (ogzstream& file);
    static s_int8 get_state (igzstream& file);

    // Background Music functions:
    // Use these to load/unload background music
    static int load_background(int slot, char *filename);
    static void unload_background(int slot);

    // All input is clamped from 0 to 100
    static void set_background_volume(int);

    // Use only when music is loaded
    static void pause_music(void);
    static void unpause_music(void);

    // Use these to load/unload wave files
    static int load_wave(int slot, char *filename);
    static void unload_wave(int slot);

    // Used to just start sounds playing
    static void play_wave(int channel, int slot);
    static void play_background(int slot);

    // Fade in and fade out background music (time in ms)
    // Fadeout unselects current tune when done
    static void fade_in_background(int slot, int time);
    static void fade_out_background(int time);

    // Temporary convience function to change background
    static void change_background(int slot, int time);

    static bool is_initialized () { return audio_initialized; }
    static bool is_schedule_activated () { return schedule_active; }
    static py_object& get_schedule () { return schedule; }
    static void set_schedule_active (bool a) { schedule_active = a; }
    static int get_current_background () { return current_background; }
    
    static void set_schedule (string file, PyObject * args = NULL);

#ifdef OGG_MUSIC
    // static loop_info *loop[NUM_MUSIC];

    // static int get_loop_start() { return loop[current_background]->start; }
    // static int get_loop_end() { return loop[current_background]->end; }
    // static int get_start_page_pcm() { return loop[current_background]->start_page_pcm; }
    // static int get_start_page_raw() { return loop[current_background]->start_page_raw; }
    // static OggVorbis_File* get_vorbisfile();
#endif

private:
#ifndef SWIG
    static bool schedule_active;
    static bool audio_initialized;
    static int background_volume;
    static int effects_volume;
    static Mix_Music *music[NUM_MUSIC];
    static string music_file[NUM_MUSIC];
    static Mix_Chunk *sounds[NUM_WAVES];
    static bool background_on;
    static int current_background;
    static bool background_paused;
    static int audio_rate;
    static Uint16 buffer_size;
    static Uint16 audio_format;
    static int audio_channels;
    static py_object schedule;
    static PyObject *schedule_args;
#endif
};

#endif
