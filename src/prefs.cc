/*
   $Id$

   Copyright (C) 2000/2001/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file prefs.cc
 *
 * @author Kai Sterker
 * @brief Adonthell's configuration
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <iostream> 
#include <fstream> 
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "prefs.h"
#include "python_class.h"
#include "game.h"

config::config () 
{
    // set some default values where possible
#if defined (WIN32) || defined (__APPLE__)
    screen_mode = 1;                // Fullscreen
#else
    screen_mode = 0;                // Fullscreen
#endif
#if defined (QTOPIA) || defined (WIN32)
    double_screen = 0;              // Double screen
#else
    double_screen = 1;              // Double screen
#endif
    quick_load = 0;                 // Quick-load disabled
    audio_channels = 1;             // Stereo
    audio_resolution = 1;           // 16 bit
    audio_sample_rate = 2;          // 11025, 22050 or 44100 Hz
    audio_volume = 100;             // 0 - 100%
    language = "";                  // Let the user's environment decide

    // set the path to the adonthellrc file:
#ifndef SINGLE_DIR_INST
    adonthellrc = string (getenv ("HOME")) + "/.adonthell";
#else
    adonthellrc = string (".");
#endif
}
 
/**
 * Displays the help message - for internal use only.
 * 
 */ 
void print_help_message (char * argv[]) 
{
    cout << "Usage: " << argv[0] << " [OPTIONS] GAME" << endl;
    cout << endl;
    cout << "Where [OPTIONS] can be:\n";
    cout << "-h         print this help message" << endl; 
    cout << "-d         print the data directory and exit" << endl; 
    cout << "-v         print version and exit" << endl; 
    cout << "-l         list installed games and exit" << endl;
    cout << "-g dir     play the game contained in dir (for development only)" << endl;
    cout << "-c         byte-compile all Python scripts in this directory (for " << endl;
    cout << "           development only)" << endl; 
}

/**
 * Displays the available games - for internal use only.
 * 
 */ 
void print_available_games () 
{
    struct dirent * d;
    DIR * mydir = opendir ((game::global_data_dir() + "/games").c_str()); 
    bool nogames = true; 
 
    if (!mydir) 
    {
        cerr << "Cannot open directory " << game::global_data_dir() + "/games!" << endl;
        exit (1); 
    }

    while ((d = readdir (mydir)) != NULL)
    {
        string s (d->d_name); 
        if (s != "." && s != "..")
        {
            if (nogames) 
            {
                nogames = false;
                cout << "Installed games (Suitable for the GAME parameter):\n"; 
            }
            cout << " - " << d->d_name << endl; 
        }
    }

    if (nogames) cout << "No games available.\n"; 
    
    closedir (mydir); 
}

void config::parse_arguments (int argc, char * argv[])
{     
    int c;
    
    // Check for options
    while (1)
    {
        c = getopt (argc, argv, "lcdhvg:");
        if (c == -1)
            break;
        
        switch (c)
        { 
            case 'l':
                print_available_games (); 
                exit (0); 
                break;
                
            case 'd':
                cout << game::global_data_dir() << endl;
                exit (0); 
                break;
                
            case 'v':
                cout << VERSION << endl;
                exit (0); 
                break;
                
            case 'c':
            {
                python::init (); 
                python::exec_string ("import compileall; compileall.compile_dir (\".\", 0);");  
                python::cleanup (); 
                exit (0); 
                break;
            }

            case 'g':
            {
                gamedir = optarg;
                if (gamedir[gamedir.size () - 1] == '/')
                    gamedir.erase (gamedir.size () - 1);
                
                // Check whether the requested game directory exists
                DIR * mydir = opendir (gamedir.c_str ()); 
                
                if (!mydir) 
                {
                    cerr << "Cannot open directory " << gamedir << "!" << endl;
                    exit (1); 
                }
                closedir (mydir);
                
                break;
            }
            
            case '?':
            case 'h':
                print_help_message (argv);
                exit (0);
                break;                 
        }
    }

    // Check whether the GAME parameter is needed
    if (gamedir == "")
    {
        // Check whether the GAME parameter is given
        if (argc - optind != 1)
        {
            print_help_message (argv);
            exit (0);
        }
        
        // Check whether the requested game exists
        struct dirent * d;
        DIR * mydir = opendir ((game::global_data_dir() + "/games").c_str()); 
        bool found = false; 
        
        if (!mydir) 
        {
            cerr << "Cannot open directory " << game::global_data_dir() + "/games!" << endl;
            exit (1); 
        }
        
        while ((d = readdir (mydir)) != NULL)
        {
            if (string (d->d_name) == argv[optind]) found = true; 
        }
        
        closedir (mydir); 
        
        if (!found) 
        {
            cerr << "Game '" << argv[optind] << "' can't be found.\n"
                 << "Run '" << argv[0] << " -l' for a list of available games.\n"; 
            exit (1); 
        }
        
        // The game exists, so let the show continue...
        gamedir = game::global_data_dir() + "/games/"; 
        gamedir += argv[optind];
    }

    // Now check whether the directory is a valid game directory
    string tfile = gamedir + "/gamename.txt"; 
    ifstream f (tfile.c_str ()); 
    if (!f.is_open ()) 
    {
        cerr << "Directory " << gamedir << " is not a valid game directory.\n";
        exit (1); 
    }
    char t[256];
    f.getline (t, 256); 
    game_name = t;
    f.close ();
}



// That's more or less a move operator, as the source is destroyed
config& config::operator =(const config *c)
{
    screen_mode = c->screen_mode;
    audio_channels = c->audio_channels; 
    audio_resolution = c->audio_resolution;
    audio_sample_rate = c->audio_sample_rate;
    audio_volume = c->audio_volume;
    adonthellrc = c->adonthellrc;

    delete c;
    return *this;
}

char *config::get_adonthellrc ()
{
    return (char *) adonthellrc.c_str ();
}

// write a default configuration file
void config::write_adonthellrc ()
{
    string fname;

#ifndef WIN32
    fname = adonthellrc + "/adonthellrc";
#else
    fname = adonthellrc + "/adonthell.ini";
#endif
    
    ofstream rc (fname.c_str ());

    rc << "# Sample Adonthell configuration file;\n"
       << "# edit to your needs!\n\n"
       << "# Screen-mode num\n#   0  Windowed mode\n"
       << "#   1  Fullscreen mode\n    Screen-mode " << (int) screen_mode 
       << "\n\n" << "# Double-size num\n#   0  320x240 mode\n"
       << "#   1  640x480 (double) mode\n    Double-size " 
       << (int) double_screen << "\n\n"
       << "# Language [locale]\n# Where locale has the form fr_FR or de_DE, etc.\n    Language [" << language << "]\n\n"
       << "# Quick-load num\n#   0  off\n#   1  on\n    Quick-load "
       << (int) quick_load << "\n\n"
       << "# Audio-channels num\n#   0  Mono\n#   1  Stereo\n"
       << "    Audio-channels " << (int) audio_channels << "\n\n"
       << "# Audio-resolution num\n#   0  8 bit\n#   1  16 bit\n"
       << "    Audio-resolution " << (int) audio_resolution << "\n\n"
       << "# Audio-sample-rate num\n#   0  11025 Hz\n#   1  22050 Hz\n#   2  44100 Hz\n"
       << "    Audio-sample-rate " << (int) audio_sample_rate << "\n\n"
       << "# Audio-volume num\n#   0 - 100 %\n"
       << "    Audio-volume " << (int) audio_volume << "\n\n"
       << "# Version number of this file. Please don't edit\n    Version [" << VERSION << "]\n";

    rc.close ();
}

bool config::read_adonthellrc ()
{
    int n, i = 1;
    u_int32 major = 0, minor = 0, micro = 0, MAJOR, MINOR, MICRO;
    char suffix[16] = "\0", SUFFIX[16] = "\0"; 
    string s, fname;

#ifndef WIN32
    fname = adonthellrc + "/adonthellrc";
#else
    fname = adonthellrc + "/adonthell.ini";
#endif

    // try to create that directory in case it dosn't exist
#ifdef WIN32
    mkdir (adonthellrc.c_str ());
#else
    mkdir (adonthellrc.c_str (), 0700);
#endif
    
    // prefsin is declared in lex.prefs.c
    prefsin = fopen (fname.c_str (), "r");

    // open failed -> try to write new configuration 
    if (!prefsin)
    {
        write_adonthellrc ();

        // now try again
        if (!(prefsin = fopen (fname.c_str (), "r"))) return false;
    }

    // adonthellrc opened -> read configuration
    while (i)
    {
        switch (i = parse_adonthellrc (n, s))
        {
            case PREFS_LANGUAGE:
            {
                if (parse_adonthellrc (n, s) == PREFS_STR) language = s;
                break;
            }
                
            case PREFS_SCREEN_MODE:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) screen_mode = n;
                break;
            }

            case PREFS_DOUBLE_SCREEN:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) double_screen = n;
                break;
            }

            case PREFS_QUICK_LOAD:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) quick_load = n;
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

            case PREFS_AUDIO_VOLUME:
            {
                if (parse_adonthellrc (n, s) == PREFS_NUM) audio_volume = n;
                break;
            }

            case PREFS_VERSION:
            {
                // get config file version number
                if (parse_adonthellrc (n, s) == PREFS_STR) 
                    sscanf (s.c_str (), "%d.%d.%d%15s", &major, &minor, &micro, suffix);
                break;
            }
            default: break;
        }
    }    

    fclose (prefsin);

    // get engine version numbers
    sscanf (VERSION, "%d.%d.%d%15s", &MAJOR, &MINOR, &MICRO, SUFFIX);
    
    // compare version of config file and engine
    if (major < MAJOR || 
        (major == MAJOR && minor < MINOR) ||
        (major == MAJOR && minor < MINOR && micro < MICRO) ||
        strcmp (suffix, SUFFIX) != 0)
    {
        // update config file if engine is newer
        write_adonthellrc ();
    }

    return true;
}
