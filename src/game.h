/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "prefs.h"

/** Responsible for game initialisation and finalisation.
 *  It has only a few methods, however they are critical as they
 *  are responsible for all the game initialisation and cleanup - so they must
 *  ABSOLUTELY be called as the first and last functions called in the main
 *  program.
 */ 
class game
{
public:
    /** @param argc The argc that has been passed to the main program.
     *  @param argv The argv that has been passed to the main program.
     */
    game (int argc, char** argv);

    /** Cleanup everything and quit.
     *  Performs the following:
     *         -# Destroy the window manager (win_manager).
     *         -# Write the %configuration file.
     *         -# Cleanup data.
     *         -# Shutdown audio.
     *         -# Shutdown Python.
     *         -# Shutdown SDL and video.
     */ 
    ~game ();                           // Cleanup everything

    /** %Game initialisation function.
     *  Reads the configuration file,  
     *  check the validity of the data directory and
     *  initialize the display, input, sound, data and Python systems.
     *  It MUST be called before ANY other function in the game.
     *  @return
     *      - true Initialisation sucessfull.
     *      - false Initialisation failure - don't go any further and
     *              quit.
     */ 
    bool init ();                       // Init everything

private:
    config *configuration;              // The game's configuration data
};

#endif // __GAME_H__
