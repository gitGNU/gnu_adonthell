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
#include "SDL_thread.h"

class game
{
public:
    game (int, char**);
    ~game ();                           // Cleanup everything
    bool init ();                       // Init everything

private:
    config *configuration;              // The game's configuration data
};

#endif // __GAME_H__
