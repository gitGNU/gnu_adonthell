/*
   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef SDL

#include "SDL_mixer.h"

class audio
{
public:
   int background_volume;
   Mix_Music *music;
   bool background_on;
   int current_background;
   bool background_paused;
   bool audio_initialized;

  // Use these to initialize/destroy sound connection
  audio();
  ~audio();
  void audio_cleanup(void);

  // Background Music functions:
    // Use these to load/unload background music
    void load_background(int);
    void unload_background(void);

    void set_background_volume(int);

    // Use only when music is loaded
    void pause_music(void);
    void unpause_music(void);

  // Sound Effect functions:

};

#endif
#endif