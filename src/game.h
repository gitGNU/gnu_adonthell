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

#ifndef _game_h
#define _game_h

#include "SDL.h"
#include "SDL_thread.h"

#include "prefs.h"
#include "Python.h"
#include "storage.h"
#include "gametime.h"

class game_engine
{
public:
    virtual void realtime_tasks () = 0;
    virtual void gametime_tasks () = 0;
    
    virtual ~game_engine () { }
};

class game
{
#ifdef SDL_MIXER
    static SDL_Thread *audio_thread;
#endif  
  
public:
    static void init(config&);
    static void cleanup();

    static void load (const char*, const char*);// Load a game
    static void save (const char*);             // Save the game

    static PyObject *globals;                   // Global namespace for use in scripts
    static objects characters;                  // All the characters 
    static objects quests;                      // All the quests
  
    static char *theme;                         // Put somewhere in window code!?    
    static game_engine *engine;                 // The engine having input focus
    static gametime *time;                      // The gametime object
};

#endif
