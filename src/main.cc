/*
   $Id
 
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "keyboard.h"
#include "gfx.h"
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"
#include "globals.h"
#ifdef SDL
#include "SDL.h"
#include "SDL_thread.h"
#include "keyboard.h"
#endif
#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#endif
#include "animation.h"

int main(int argc, char * argv[])
{
  map * map1 = new map;
  if((argc!=2)&&(argc!=3)) {printf("Usage: %s mapname\n",argv[0]); exit(0);}
  if(argc==3)
    {
      if(!strcmp(argv[2],"-m1")) screen::init_display(1);
      if(!strcmp(argv[2],"-m0")) screen::init_display(0);
  }
  else screen::init_display(0);
  if(map1->load(argv[1])) {printf("Error loading %s!\n",argv[1]);return(1);}

#ifdef SDL
  SDL_Thread *input_thread;

   input_thread = SDL_CreateThread((void*)keyboard_init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }
#endif

#ifdef SDL_MIXER
  SDL_Thread *audio_thread;

  audio_init();
  audio_thread = SDL_CreateThread((void*)audio_update, NULL);
  if ( audio_thread == NULL) {
     fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
     fprintf(stderr, "Audio will not be used\n");
  }
#endif
  mapengine::map_engine(map1);
#ifdef SDL
  fprintf(stderr, "Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);
#endif

#ifdef SDL_MIXER
  if (audio_thread != NULL) {
    SDL_KillThread(audio_thread);
    audio_cleanup();
  }
#endif
  //  delete map1;
#ifdef SDL
  SDL_Quit();
#endif
return(0);
}
