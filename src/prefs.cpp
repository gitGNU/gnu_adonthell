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
    alt_configs = NULL;                     // Alternative Configurations
    defaults = "adonthell";                 // Default configuration
    
    // set some default values where possible
    datadir = "/usr/local/share/adonthell"; // Directory containing the gamedata
    mapname = "maptest.map";                // Map to load on startup
    screen_resolution = 0;                  // 320x240
    screen_mode = 1;                        // Fullscreen
    window_theme = "original";              // Default theme
    audio_channels = 1;                     // Stereo
    audio_resolution = 1;                   // 16 bit
    audio_interpolation = 1;                // Interpolation on
    audio_sample_rate = 2;                  // 11025, 22050 or 44100 Hz
    audio_volume = 50;                      // 0 - 100%

    // set the path to the adonthellrc file:
    adonthellrc = string (getenv ("HOME")) + "/.adonthell";
}

// That's more or less a move operator, as the source is destroyed
config& config::operator =(const config *c)
{
    alt_configs = c->alt_configs;
    defaults = c->defaults;
    section = c->section;
    datadir = c->datadir;
    mapname = c->mapname;
    screen_resolution = c->screen_resolution;
    screen_mode = c->screen_mode;
    window_theme = c->window_theme;
    audio_channels = c->audio_channels; 
    audio_resolution = c->audio_resolution;
    audio_interpolation = c->audio_interpolation;
    audio_sample_rate = c->audio_sample_rate;
    audio_volume = c->audio_volume;
    adonthellrc = c->adonthellrc;

    delete c;
    return *this;
}

char *config::get_adonthellrc ()
{
    char *string = strdup (adonthellrc.c_str ());
    return string;
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
       << "Default [" << defaults << "]\n\n";

    save_section (rc);
    rc.close ();
}

void config::save_section (ofstream &rc)
{
    rc << "Section [" << section << "]\n\n"
       << "# Data [path]\n#   Path to the games data directory\n"
       << "    Data [" << datadir << "]\n\n"
       << "# Map [file]\n#   Filename of the standard map to load\n"
       << "    Map [" << mapname << "]\n\n"
// Currently unused
//       << "# Screen-resolution num\n#   0  Low (320x240) resolution\n"
//       << "#   1  High (640x480) resolution\n"
//       << "    Screen-resolution " << (int) screen_resolution << "\n\n"
       << "# Screen-mode num\n#   0  Windowed mode\n"
       << "#   1  Fullscreen mode\n    Screen-mode " << (int) screen_mode << "\n\n"
       << "# Window-theme [theme]\n#   original   - default Adonthell theme by Cirrus"
       << "\n#   silverleaf - by Kaiman\n    Window-theme [" << window_theme << "]\n\n"
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
       << "End\n\n";

    // recusively save all the other sections :)
    if (alt_configs != NULL) alt_configs->save_section (rc);
}

int config::read_adonthellrc ()
{
    int n, i = 1, got_it = 0;
    string s, fname = adonthellrc + "/adonthellrc";
    config *c;

    // try to create that directory in case it dosn't exist
    mkdir (adonthellrc.data (), 0700);

    // prefsin is declared in lex.prefs.c
    prefsin = fopen (fname.c_str (), "r");

    // open failed -> try to write new configuration 
    if (!prefsin)
    {
        // when writing a new configuration, defaults has to match section
        if (section != "") defaults = section;
        else section = defaults;
        
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
                if (parse_adonthellrc (n, s) == PREFS_STR)
                    defaults = s;

                if (section == "")
                    section = defaults;
                        
                break;
            }

            case PREFS_SECTION:
            {
                if (parse_adonthellrc (n, s) == PREFS_STR)
                {
                    // In case no section and no defaults given, load the
                    // first one
                    if (section == "") section = s;
                        
                    // This is the section we have been given to load, 
                    if (section == s)
                    {
                        load_section ();
                        got_it = 1;
                    }
                    // Load the rest and add them to the list o' sections
                    else
                    {
                        c = new config (s);
                        c->load_section ();
                        c->alt_configs = alt_configs;
                        alt_configs = c;
                    }
                }

                break;
            }

            default: break;
        }
    } 

    if (!got_it)
    {
        // See if we've got any other section we might use
        if (alt_configs != NULL)
        {
            cout << "\nCan't find the \"" << section << "\" section of the"
                 << " adonthellrc file.\nLoading \"" << alt_configs->section
                 << "\" instead.\n" << flush;

            *this = alt_configs;
            return 1;
        }
    
        // If we arrive here, no configuration has been loaded
        cout << "\nSorry, could not load the configuration \"" << section << "\".\n"
             << "Please check the " << adonthellrc << "/adonthellrc file\n"
             << "for available configurations.\n" << flush;
        return 0;
    }
    
    return 1;
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

            case PREFS_WINDOW_THEME:
            {
                if (parse_adonthellrc (n, s) == PREFS_STR) window_theme = s;
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
