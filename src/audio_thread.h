/*
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __AUDIO_THREAD_H__
#define __AUDIO_THREAD_H__
#ifdef SDL_MIXER
// This is our audio 'hook' so audio events can be called
// from different places
#include "audio.h"
static audio *audio_in = NULL;

int audio_init();
void audio_update();
void audio_cleanup();
#endif
#endif
