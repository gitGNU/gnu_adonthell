/*
   $Id$
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#ifdef SDL_MIXER

#include "audio.h"

// This is our audio 'hook' that anything in the project
// can get at in order to trigger sounds. The actual
// allocation is at the end of the 'audio.cc' file.
extern audio *audio_in;

#endif
#endif