%module audio

%{

#include "types.h"
#include "prefs.h"
#include "audio.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

class audio
{
public:
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

    static bool is_initialized() { return audio_initialized; }
};

