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
 


#ifndef SWIG
using namespace std; 
#endif


extern FILE* prefsin;
extern int parse_adonthellrc (int&, string&);

enum
{
    PREFS_UNKNOWN = 0,
    PREFS_SCREEN_MODE = 6,
    PREFS_AUDIO_CHANNELS = 8,
    PREFS_AUDIO_RESOLUTION = 9,
    PREFS_AUDIO_SAMPLE_RATE = 10,
    PREFS_AUDIO_VOLUME = 12,
    PREFS_NUM = 13,
    PREFS_STR = 14,
};


// Contains all the configuration information
class config
{
public:
    config ();
    
    void parse_arguments (int argc, char * argv[]); 
    
    void write_adonthellrc ();      // Writes a default adonthellrc
    int read_adonthellrc ();        // Reads the adonthellrc file
    char *get_adonthellrc ();       // Return the user's private adonthell directory

    string game_name;               // Name of the game that will be played
    string gamedir;                 // Directory containing the game that will be played 
    string datadir;                 // Directory containing the gamedata
    u_int8 screen_mode;             // Window or Fullscreen
    u_int8 audio_channels;          // Mono or Stereo
    u_int8 audio_resolution;        // 8 or 16 bit
    u_int8 audio_sample_rate;       // 11025, 22050 or 44100 Hz
    u_int8 audio_volume;            // 0 - 100%

private:
    config & operator = (const config*);
    
    string adonthellrc;             // Path to the adonthellrc file: $HOME/.adonthell/  
};

#endif // __PREFS_H__
