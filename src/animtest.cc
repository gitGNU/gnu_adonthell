// $Id
/*
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
  animation anim;

  screen::init_display(0);

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

  anim.load_frame("test1.pnm");
  anim.set_delay(0,20);
  anim.load_frame("test2.pnm");
  anim.set_delay(1,20);
  anim.load_frame("test3.pnm");
  anim.set_delay(2,20);
  anim.loop=false;
  anim.reverse=true;
  anim.mask_on=true;
  anim.alpha_on=false;
  anim.alpha=0;
  while(1)
    {
      if(keyboard::is_pushed(Escape_Key)) break;
      // Clear the screen
      screen::drawbox(0,0,320,200,0xFFFFFF);
      anim.update();
      anim.draw(0,0);
      screen::show();
    }

  SDL_Quit();
  return(0);
}
