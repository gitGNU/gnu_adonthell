/*
   $Id$

   Copyright (C) 2000/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file prefs.h
 *
 * @author Kai Sterker
 * @brief Adonthell's configuration
 */

#ifndef __PREFS_H__
#define __PREFS_H__

#include <string>
#include <fstream.h>
#include "types.h"

#ifndef SWIG
using namespace std; 
#endif

/**
 * The config file opened by the lexical scanner
 */ 
extern FILE* prefsin;

/**
 * Start the lexical scanner to parse the config file,
 * usually fount at ~/.adonthell/adonthellrc
 */
extern int parse_adonthellrc (int&, string&);

/**
 * Returncodes of the scanner for the different entries
 * of the configuration file.
 */
enum
{
    PREFS_UNKNOWN = 0,
    PREFS_SCREEN_MODE = 1,
    PREFS_LANGUAGE = 2,
    PREFS_QUICK_LOAD = 3,
    PREFS_AUDIO_CHANNELS = 4,
    PREFS_AUDIO_RESOLUTION = 5,
    PREFS_AUDIO_SAMPLE_RATE = 6,
    PREFS_AUDIO_VOLUME = 7,
    PREFS_VERSION = 8,
    PREFS_NUM = 9,
    PREFS_STR = 10,
    PREFS_DOUBLE_SCREEN = 11,
};


/**
 * This class contains the engine's configuration read either from the
 * %config file or from the command line.
 */
class config
{
public:
    /**
     * Constructor. Initializes all configuration options with default
     * values.
     * - Language: English
     * - Screen mode: windowed
     * - Quick load: enabled
     * - Audio: High quality (44.1kHz, 16 bit, stereo)
     * - Volume: 100%
     * - Config file: $HOME/.adonthell/adonthellrc
     */
    config ();
    
    /**
     * See whether any options have been specified on the command line.
     * Possible command line options are
     * - <b>\-h</b> Print help message
     * - <b>\-d</b> Print the data directory
     * - <b>\-v</b> Print the version number 
     * - <b>\-l</b> List games found in the gamedir
     * - <b>\-g dir</b> Play %game contained in dir
     * - <b>\-c</b> Byte-compile all Python scripts in the current directory
     *
     * @param argc argument count
     * @param argv argument vector
     */
    void parse_arguments (int argc, char * argv[]); 
    
    /**
     * Writes a default configuration file with the values set in the
     * constructor
     * @sa config::config ()
     */
    void write_adonthellrc ();
    /**
     * Reads the configuration file. If it cannot find the file, it
     * tries to write a default one.
     * @return <b>true</b> on success, <b>false</b> otherwise.
     */
    bool read_adonthellrc ();
    /**
     * Returns the path to the user's private Adonthell directory.
     * Usually this is $HOME/.adonthell/ and will contain his personal
     * configuration as well as all saved games.
     */    
    char *get_adonthellrc ();
    
    /**
     * @name Configuration options
     */
    //@{
    /**
     * Language to use if NLS was compiled in.
     */
    string language;
    /**
     * Name of the %game that is running at present.
     */
    string game_name;
    /**
     * Path of the directory that contains the %game running at present.
     */
    string gamedir;
    /**
     * Whether the engine shall run in window (0) or fullscreen (1) mode.
     */
    u_int8 screen_mode;
    u_int8 double_screen;
    /**
     * Whether the quick-load feature is enabled (1) or not (0)
     */
    u_int8 quick_load;
    /**
     * The number of channels: mono (0) or stereo (1).
     */
    u_int8 audio_channels;
    /**
     * The resolution: 8 bit (0) or 16 bit (1)
     */
    u_int8 audio_resolution;
    /**
     * The sample rate: 11025 Hz (0), 22050 Hz (1) or 44100 Hz (2)
     */
    u_int8 audio_sample_rate;
    /**
     * The volume: a value betwen 0 and 100. 0 means that audio 
     * is completely off.
     */
    u_int8 audio_volume;
    //@}
    
private:
    config & operator = (const config*);
    
    string adonthellrc;             // Path to the adonthellrc file: $HOME/.adonthell/  
};

#endif // __PREFS_H__
