/*
   $Id$

   Copyright (C) 1999/2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   game.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the game class.
 * 
 * 
 */



#ifndef GAME_H__
#define GAME_H__

#include "prefs.h"
#include "gamedata.h"
#include "python.h"

/** Responsible for game initialisation and finalisation.
 *  It has only a few methods, however they are critical as they
 *  are responsible for all the game initialisation and cleanup - so they must
 *  ABSOLUTELY be called as the first and last functions called in the main
 *  program.
 */ 
class game
{
public:
    /**
     * %Game initialisation function.
     * Reads the configuration file,  
     * check the validity of the data directory and
     * initialize the display, input, sound, data and Python systems.
     * It MUST be called before ANY other function in the game.
     *
     * @param argc The argc that has been passed to the main program.
     * @param argv The argv that has been passed to the main program.
     * @return
     *     - true Initialisation sucessfull.
     *     - false Initialisation failure - don't go any further and
     *             quit.
     */ 
    static bool init (int argc, char** argv);

    /** Cleanup everything and quit.
     *  Performs the following:
     *         -# Destroy the window manager (win_manager).
     *         -# Write the %configuration file.
     *         -# Cleanup data.
     *         -# Shutdown audio.
     *         -# Shutdown Python.
     *         -# Shutdown SDL and video.
     */ 
    static void cleanup (); 
    
private:
    /**
     * The game's configuration data
     * 
     */ 
    static config *configuration;
    
    /**
     * Initialise the game's data.
     * 
     */ 
    static void init_data (); 

    /**
     * Cleanup the game's data.
     * 
     */ 
    static void cleanup_data (); 
    
    /**
     * The Adonthell Python wrapper.
     * 
     */ 
    static PyObject *py_module;
};


/**
 * @namespace data
 * Namespace that holds game data that are publicly available.
 * 
 */


#endif // GAME_H__
