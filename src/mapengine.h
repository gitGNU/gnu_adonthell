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

#ifndef _mapengine_h
#define _mapengine_h

#include "SDL.h"
#include "SDL_thread.h"
#include "prefs.h"

class mapengine
{
 public:
  static void map_engine(landmap * amap);
  static void update_and_show(landmap * amap);
  static void fade_out(landmap * amap, u_int16 depx=56, u_int16 depy=12); 
  static void fade_in(landmap * amap, u_int16 depx=56, u_int16 depy=12); 
};

#endif
