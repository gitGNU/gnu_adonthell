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

#include <sys/stat.h>
#include <fstream.h>

#include "prefs.h"

config::config (string s) : section (s)
{
    // set some default values where possible
    datadir = "/usr/local/share/adonthell"; // Directory containing the gamedata
    mapname = "maptest.map";                // Map to load on startup
    screen_resolution = 0;                  // 320x240
    screen_mode = 1;                        // Fullscreen
    audio_channels = 1;                     // Stereo
    audio_resolution = 1;                   // 16 bit
    audio_interpolation = 1;                // Interpolation on
    audio_sample_rate = 2;                  // 11025, 22050 or 44100 Hz
    audio_volume = 50;                      // 0 - 100%

    // set the path to the adonthellrc file:
    adonthellrc = string (getenv ("HOME")) + "/.adonthell";

    // try to create that directory in case it dosn't exist
    mkdir (adonthellrc.data (), 0700);
}


// write a default configuration file
void config::write_adonthellrc ()
{
    string fname = adonthellrc + "/adonthellrc";
    
    ofstream rc (fname.c_str ());

    rc << "# Sample Adonthell configuration file;\n"
       << "# edit to your needs!\n\n"
       << "# Default [section]\n#   Section to load if engine is called"
       << " without parameter\n"
       << "Default [adonthell]\n\n"
       << "Section [adonthell]\n\n"
       << "# Data [path]\n#   Path to the games data directory\n"
       << "    Data [" << datadir << "]\n\n"
       << "# Map [file]\n#   Filename of the standard map to load\n"
       << "    Map [" << mapname << "]\n\n"
       << "# Screen-resolution num\n#   0  Low (320x240) resolution\n"
       << "#   1  High (640x480) resolution\n"
       << "    Screen-resolution " << (int) screen_resolution << "\n\n"
       << "# Screen-mode num\n#   0  Windowed mode\n"
       << "#   1  Fullscreen mode\n    Screen-mode " << (int) screen_mode << "\n\n"
       << "# Audio-channels num\n#   0  Mono\n#   1  Stereo\n"
       << "    Audio-channels " << (int) audio_channels << "\n\n"
       << "# Audio-resolution num\n#   0  8 bit\n#   1  16 bit\n"
       << "    Audio-resolution " << (int) audio_resolution << "\n\n"
       << "# Audio-sample-rate num\n#   0  11025 Hz\n#   1  22050 Hz\n#   2  44100 Hz\n"
       << "    Audio-sample-rate " << (int) audio_sample_rate << "\n\n"
       << "# Audio-interpolation num\n#   0  Off\n#   1  On\n"
       << "    Audio-interpolation " << (int) audio_interpolation << "\n\n"
       << "# Audio-volume num\n#   0 - 100 %\n"
       << "    Audio-volume " << (int) audio_volume << "\n\n"
       << "End\n";

    rc.close ();
}

int config::read_adonthellrc ()
{
    int n, i = 1;
    string s, fname = adonthellrc + "/adonthellrc";

    // prefsin is declared in lex.prefs.c
    prefsin = fopen (fname.c_str (), "r");

    // open failed -> try to write new configuration 
    if (!prefsin)
    {
        write_adonthellrc ();

        // now try again
        if (!(prefsin = fopen (fname.c_str (), "r"))) return 0;
    }

    // adonthellrc opened -> read configuration
    // if we've got something in section, then try to find this section,
    // else use the section specified in 'Default' or use the first section
    // we find.
    while (i)
    {
        switch (i = parse_adonthellrc (n, s))
        {
            case PREFS_DEFAULT:
            {
                if (section == "") 
                    if (parse_adonthellrc (n, s) == PREFS_STR)
                        section = s;
                    
                break;
            }

            case PREFS_SECTION:
            {
                // In case we have no explicit section to load, take this as it
                // is the first
                if (section == "")
                {
                    load_section ();
                    return 1;
                }
                
                // If we have been given a (default) section to load, look if
                // we found it
                else
                {
                    if (parse_adonthellrc (n, s) == PREFS_STR)
                        if (s == section)
                        {
                            load_section ();
                            return 1;
                        }
                }

                break;
            }

            default: break;
        }
    } 

    // If we arrive here, no configuration has been loaded
    cout << "\nSorry, could not load the configuration \"" << section << "\".\n"
         << "Please check the " << adonthellrc << "/adonthellrc file\n"
         << "for available configurations.\n" << flush;

    return 0;
}

void config::load_section ()
{
    int n, i = 1;
    string s;

    while (i)
    {
        switch (i = parse_adonthellrc (n, s))
        {
            case PREFS_END: return;
            
            case PREFS_DATA:
            {
                if (parse_adonthellrc (n, s) == PREFS_STR) datadir = s;
                break;
            }

            case PREFS_MAP:
            {
                if (parse_adonthellrc (n, s) == PREFS_STR) mapname = s;
                break;
            }

            case PREFS_SCREEN_RESOLUTION:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) screen_resolution = n;
                break;
            }

            case PREFS_SCREEN_MODE:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) screen_mode = n;
                break;
            }

            case PREFS_AUDIO_RESOLUTION:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_resolution = n;
                break;
            }

            case PREFS_AUDIO_CHANNELS:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_channels = n;
                break;
            }

            case PREFS_AUDIO_SAMPLE_RATE:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_sample_rate = n;
                break;
            }

            case PREFS_AUDIO_INTERPOLATION:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_interpolation = n;
                break;
            }

            case PREFS_AUDIO_VOLUME:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_volume = n;
                break;
            }

            default: break;
        }
    }    
}
