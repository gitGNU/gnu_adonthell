/*
   $Id$

   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __PREFS_H__
#define __PREFS_H__

#include <string>
#include <fstream.h>
#include "types.h"

extern FILE* prefsin;
extern int parse_adonthellrc (int&, string&);

enum
{
    PREFS_UNKNOWN = 0,
    PREFS_DEFAULT = 1,
    PREFS_SECTION = 2,
    PREFS_DATA = 3,
    PREFS_MAP = 4,
    PREFS_SCREEN_RESOLUTION = 5,
    PREFS_SCREEN_MODE = 6,
    PREFS_WINDOW_THEME = 7,
    PREFS_AUDIO_CHANNELS = 8,
    PREFS_AUDIO_RESOLUTION = 9,
    PREFS_AUDIO_SAMPLE_RATE = 10,
    PREFS_AUDIO_INTERPOLATION = 11,
    PREFS_AUDIO_VOLUME = 12,
    PREFS_NUM = 13,
    PREFS_STR = 14,
    PREFS_END = 15
};


// Contains all the configuration information
class config
{
public:
    config (string);

    void write_adonthellrc ();      // Writes a default adonthellrc
    int read_adonthellrc ();        // Reads the adonthellrc file
    char *get_adonthellrc ();       // Return the user's private adontell directory

    string datadir;                 // Directory containing the gamedata
    string mapname;                 // Map to load on startup
    string window_theme;            // Theme used by our GUI
    u_int8 screen_resolution;       // 320x240 or 640x480
    u_int8 screen_mode;             // Window or Fullscreen
    u_int8 audio_channels;          // Mono or Stereo
    u_int8 audio_resolution;        // 8 or 16 bit
    u_int8 audio_sample_rate;       // 11025, 22050 or 44100 Hz
    u_int8 audio_interpolation;     // off/on
    u_int8 audio_volume;            // 0 - 100%

private:
    void load_section ();           // Load the actual game settings
    void save_section (ofstream&);  // Save them

    config & operator = (const config*);
    
    string section;                 // Which section of the adonthellrc to read
    string defaults;                // Default section to load
    string adonthellrc;             // Path to the adonthellrc file: $HOME/.adonthell/ 

    config *alt_configs;            // List of alternative configurations
};

#endif // __PREFS_H__
